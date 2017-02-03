#include "XMemo.h"
#include <QApplication>
#include <QCheckBox>
#include <QStringList>
#include <QtAlgorithms>
#include <QToolBar>
#include <QDebug>
#include <QHeaderView>
#include <QPalette>
#include "DbOperator.h"
#include "Settings.h"

XMemo::XMemo(QWidget *parent)
    : QMainWindow(parent)
{    
    memosTableWidget = new QTableWidget(0, 2);
    setCentralWidget(memosTableWidget);
    QStringList tableWidgetHeaders;
    tableWidgetHeaders<<tr("Show")<<tr("Memo");
    memosTableWidget->setHorizontalHeaderLabels(tableWidgetHeaders);
    memosTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    memosTableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);   //可多选

    init();
    createTrayIcon();

    memosTableWidget->setFrameShape(QFrame::NoFrame); //设置边框
    //memosTableWidget->setShowGrid(false); //设置不显示格子线
    memosTableWidget->setFocusPolicy(Qt::NoFocus); //去除选中虚线框
    memosTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑

    //设置表头字体加粗
    QFont font = memosTableWidget->horizontalHeader()->font();
    font.setBold(true);
    memosTableWidget->horizontalHeader()->setFont(font);
    memosTableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    memosTableWidget->horizontalHeader()->setSectionsClickable(false);
    memosTableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    memosTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    memosTableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    memosTableWidget->setAlternatingRowColors(true);    //颜色交替

    //没有便签信息时打开管理面板
    if(memosList.isEmpty())
    {
        this->show();
    }

    setWindowTitle("XMemo");
    setWindowIcon(QIcon(":/image/xmemo.png"));
}

XMemo::~XMemo()
{
    for(auto &i : memosList)
    {
        delete i;
        i = nullptr;
    }
}

//初始化动作，载入设置和数据库信息，加载便签
void XMemo::init()
{
    createAction();

    Settings::getInstance().load();

    DbOperator::getInstance().init();
    DbOperator::getInstance().read(memosList);

    for(const auto i: memosList)
    {
        addMemo(i);

        if(i->isVisible())
        {
            createMemoWidget(i, false);
        }
    }
}

/**
 * @brief XMemo::addMemo 在管理面板中添加便签信息
 * @param memoInfo
 */
void XMemo::addMemo(MemoInfo *memoInfo)
{
    int count = memosTableWidget->rowCount();
    memosTableWidget->insertRow(count);
    QCheckBox *visibilityCheckBox = new QCheckBox;
    visibilityCheckBox->resize(BUTTON_WIDTH, BUTTON_HEIGHT);
    connect(visibilityCheckBox, &QCheckBox::clicked, this, &XMemo::onVisibilityCheckBoxClicked);
    QString shownContent = memoInfo->getContent().length()>30?memoInfo->getContent().left(30)+"...":memoInfo->getContent();
    QTableWidgetItem *contentTableWidgetItem = new QTableWidgetItem(shownContent);
    memosTableWidget->setItem(count, 1, contentTableWidgetItem);
    memosTableWidget->setCellWidget(count, 0, visibilityCheckBox);

    visibilityCheckBoxHashMap[static_cast<QObject*>(visibilityCheckBox)]=memoInfo;
    visibilityCheckBox->setChecked(memoInfo->isVisible());

    tableWidgetItemHashMap[memoInfo->getId()] = TableWidgetItemInfo{contentTableWidgetItem, visibilityCheckBox};
}

void XMemo::newMemo()
{
    MemoInfo *memoInfo = new MemoInfo();
    createMemoWidget(memoInfo, true);
    memosList.push_back(memoInfo);
    DbOperator::getInstance().add(*memoInfo);

    addMemo(memoInfo);
}

void XMemo::deleteMemos(QList<int> selectedRows)
{
    /**
     * 从大到小排序，保证每次删除最靠后的一行，不影响之前的行号
     **/
    qSort(selectedRows.begin(), selectedRows.end(), [](int a, int b){
        return a > b;
    });
    for(const auto i : selectedRows)
    {
        QObject *checkBoxObj = static_cast<QObject *>(memosTableWidget->cellWidget(i, 0));
        MemoInfo *memoInfo = visibilityCheckBoxHashMap.value(checkBoxObj);
        DbOperator::getInstance().remove(*memoInfo);

        visibilityCheckBoxHashMap.remove(static_cast<QObject *>(memosTableWidget->cellWidget(i, 0)));
        tableWidgetItemHashMap.remove(memoInfo->getId());
        delete memosTableWidget->cellWidget(i, 0);
        memosTableWidget->removeRow(i);

        delete memoInfo;
        memosList.removeOne(memoInfo);
        memoInfo = nullptr;
    }
}

void XMemo::setMemosVisibility(QList<int> selectedRows, bool visibility)
{
    for(const auto i : selectedRows)
    {
        static_cast<QCheckBox *>(memosTableWidget->cellWidget(i, 0))->setChecked(visibility);
        QObject *checkBoxObj = static_cast<QObject *>(memosTableWidget->cellWidget(i, 0));
        MemoInfo *memoInfo = visibilityCheckBoxHashMap.value(checkBoxObj);
        setMemoVisibility(memoInfo, visibility);
    }
}

void XMemo::setMemoVisibility(MemoInfo *memoInfo, bool visibility)
{
    if(visibility)
    {
        if(memoInfo->getMemoWidget() == nullptr)
            createMemoWidget(memoInfo, false);
    }
    else
    {
        disconnect(memoInfo->getMemoWidget(), &MemoWidget::closeMemo, this, &XMemo::onMemoWidgetClosed);
        memoInfo->removeWidget();
        tableWidgetItemHashMap.value(memoInfo->getId()).checkBox->setChecked(false);
    }
    DbOperator::getInstance().modifyVisibility(*memoInfo);
}

void XMemo::createAction()
{
    quitAction = new QAction(QIcon(":/image/widget/close.png"), tr("Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    newAction = new QAction(this);
    newAction->setIcon(QIcon(":/image/widget/new.png"));
    connect(newAction, &QAction::triggered, this, &XMemo::onNewMemoTriggered);

    deleteMemosAction = new QAction(this);
    deleteMemosAction->setIcon(QIcon(":/image/widget/delete.png"));
    connect(deleteMemosAction, &QAction::triggered, this, &XMemo::onDeleteMemosTriggered);

    hideMemosAction = new QAction(this);
    hideMemosAction->setIcon(QIcon(":/image/widget/Invisible.png"));
    connect(hideMemosAction, &QAction::triggered, this, &XMemo::onHideMemosTriggered);

    showMemosAction = new QAction(this);
    showMemosAction->setIcon(QIcon(":/image/widget/Visiblet.png"));
    connect(showMemosAction, &QAction::triggered, this, &XMemo::onShowMemosTriggered);

    QToolBar *toolBar = addToolBar(tr("&Tools"));
    toolBar->setMovable(false);
    toolBar->addAction(newAction);
    toolBar->addAction(deleteMemosAction);
    toolBar->addAction(hideMemosAction);
    toolBar->addAction(showMemosAction);
}

void XMemo::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(QIcon(":/image/xmemo.png"), this);
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon->show();
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &XMemo::onTrayIconClicked);
}

void XMemo::createMemoWidget(MemoInfo *memoInfo, bool isEditMode)
{
    memoInfo->createWidget(isEditMode);
    connect(memoInfo->getMemoWidget(), &MemoWidget::closeMemo, this, &XMemo::onMemoWidgetClosed);
    connect(memoInfo, &MemoInfo::memoChanged, this, &XMemo::updateMemoContent);
    connect(memoInfo->getMemoWidget(), &MemoWidget::createMemo, this, &XMemo::onNewMemoTriggered);
}

/**
 * @brief 点击关闭按钮时后台运行
 */
void XMemo::closeEvent(QCloseEvent *e)
{
    if(trayIcon->isVisible())
    {
        hide();
        e->ignore();
    }
}

QList<int> XMemo::getSelectedRows()
{
    QList<QTableWidgetItem *> selectedItems = memosTableWidget->selectedItems();
    QList<int> selectedRows;
    for(const auto &i : selectedItems)
    {
        int itemRow = memosTableWidget->row(i);
        selectedRows.push_back(itemRow);
    }
    return selectedRows;
}

void XMemo::onTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    //托盘点击方式
    switch(reason)
    {
        //单击
        case QSystemTrayIcon::Trigger:
            //双击
        case QSystemTrayIcon::DoubleClick:
            //恢复窗口显示
            this->setWindowState(Qt::WindowActive);
            this->show();
            break;
        default:
            break;
    }
}

void XMemo::onNewMemoTriggered()
{
    newMemo();
}

void XMemo::onDeleteMemosTriggered()
{
    deleteMemos(getSelectedRows());
}

void XMemo::onHideMemosTriggered()
{
    setMemosVisibility(getSelectedRows(), false);
}

void XMemo::onShowMemosTriggered()
{
    setMemosVisibility(getSelectedRows(), true);
}

void XMemo::updateMemoContent(uint id)
{
    MemoInfo *memoInfo = static_cast<MemoInfo *>(sender());
    tableWidgetItemHashMap.value(id).tableWidgetItem->setText(memoInfo->getContent());
}

void XMemo::onVisibilityCheckBoxClicked(bool checked)
{
    MemoInfo *memoInfo = visibilityCheckBoxHashMap.value(sender());
    setMemoVisibility(memoInfo, checked);
}

void XMemo::onMemoWidgetClosed(MemoInfo *memoInfo)
{
    disconnect(memoInfo->getMemoWidget(), &MemoWidget::closeMemo, this, &XMemo::onMemoWidgetClosed);
    memoInfo->removeWidget();
    tableWidgetItemHashMap.value(memoInfo->getId()).checkBox->setChecked(false);
}
