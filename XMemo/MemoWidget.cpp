#include "MemoWidget.h"
#include <DbOperator.h>

MemoWidget::MemoWidget(MemoInfo *memoInfo, bool isEditMode, QWidget *parent) : QWidget(parent)
{
    this->memoInfo = memoInfo;
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(WIDTH, HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    this->installEventFilter(this);

    closeBtn = new QPushButton("x", this);
    closeBtn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    closeBtn->move(this->width() - BUTTON_WIDTH, 0);
    connect(closeBtn, &QPushButton::clicked, this, &MemoWidget::close);
    closeBtn->hide();

    newBtn = new QPushButton("+", this);
    newBtn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    newBtn->move(0, 0);
    connect(newBtn, &QPushButton::clicked, this, &MemoWidget::onNewBtnClicked);
    newBtn->hide();

    editBtn = new QPushButton("*", this);
    editBtn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    editBtn->move(BUTTON_WIDTH, 0);
    connect(editBtn, &QPushButton::clicked, this, &MemoWidget::onEditBtnClicked);
    editBtn->hide();

    pinBtn = new QPushButton("!", this);
    pinBtn->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
    pinBtn->move(BUTTON_WIDTH * 2, 0);
    connect(pinBtn, &QPushButton::clicked, this, &MemoWidget::onPinBtnClicked);
    pinBtn->hide();

    contentEditor = new QPlainTextEdit(this);
    contentEditor->setFixedSize(WIDTH, HEIGHT - BUTTON_HEIGHT * 2);
    contentEditor->move(0, 0);
    contentEditor->setPlainText(this->memoInfo->getContent());
    contentEditor->hide();

    for(int i=0; i<COLOR_BUTTON_COUNT; ++i)
    {
        colorBtn[i] = new QPushButton(this);
        colorBtn[i]->setFixedSize(BUTTON_WIDTH, BUTTON_HEIGHT);
        colorBtn[i]->move(i * BUTTON_WIDTH, this->height() - int(BUTTON_HEIGHT * 1.5));
        colorBtn[i]->hide();
    }

    contentView = new QTextBrowser(this);
    contentView->setFixedSize(WIDTH, HEIGHT - BUTTON_HEIGHT);
    contentView->move(0, BUTTON_HEIGHT);
    contentView->setAttribute(Qt::WA_TransparentForMouseEvents);
    contentView->setPlainText(this->memoInfo->getContent());
    contentView->hide();

    if(isEditMode)
    {
        setMode(EDIT);
    }
    else
    {
        setMode(VIEW);
    }

    move(this->memoInfo->getPosition());
    show();
}

void MemoWidget::closeEvent(QCloseEvent *e)
{
    emit closeMemo(this->memoInfo);
}

void MemoWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p(e->globalPos() - relativePos);
    if (!isPinned && e->buttons() & Qt::LeftButton && isMoving)
    {
        move(e->globalPos() - relativePos);
    }
}

void MemoWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        relativePos = e->globalPos() - pos();
        isMoving = true;
    }
}

void MemoWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        isMoving = false;
        emit memoMoved(this->pos());
    }
}

void MemoWidget::setTopBtnVisibility(bool visibility)
{
    closeBtn->setVisible(visibility);
    newBtn->setVisible(visibility);
    editBtn->setVisible(visibility);
    pinBtn->setVisible(visibility);
}

void MemoWidget::setEditWidgetVisibility(bool visibility)
{
    contentEditor->setVisible(visibility);
    for(auto &i : colorBtn)
    {
        i->setVisible(visibility);
    }
    if(visibility)
    {
        //切换激活窗口并置顶之，windows下可能无效
        this->show();
        this->activateWindow();
        this->raise();
        contentEditor->setFocus();
    }

    contentView->setVisible(!visibility);
}

bool MemoWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this)
    {
        if(event->type() == QEvent::WindowActivate)
        {
            if(memoInfo->getContent().isEmpty())
                setMode(EDIT);
            else
                setMode(SELECT);
            return true;
        }
        else if(event->type() == QEvent::WindowDeactivate)
        {
            save();
            setMode(VIEW);
            return true;
        }
    }
    return false;
}


MemoWidget::~MemoWidget()
{
    // TODO
}

MemoInfo *MemoWidget::getMemoInfo() const
{
    return memoInfo;
}

QString MemoWidget::getContent() const
{
    return contentEditor->toPlainText();
}

void MemoWidget::setContent(QString content)
{
    this->contentEditor->setPlainText(content);
    this->contentView->setPlainText(content);
}

Qt::GlobalColor MemoWidget::getColor() const
{
    return color;
}

void MemoWidget::setColor(Qt::GlobalColor color)
{
    this->color = color;
    // TODO 颜色切换
}

void MemoWidget::save()
{
    contentView->setPlainText(contentEditor->toPlainText());
    emit memoChanged();
}

void MemoWidget::setMode(Mode mode)
{
    switch (mode)
    {
        case EDIT:
            setTopBtnVisibility(false);
            setEditWidgetVisibility(true);
            break;
        case VIEW:
            setTopBtnVisibility(false);
            setEditWidgetVisibility(false);
            break;
        case SELECT:
            setTopBtnVisibility(true);
            setEditWidgetVisibility(false);
        default:
            break;
    }
}

void MemoWidget::onNewBtnClicked()
{
    emit createMemo();
}

void MemoWidget::onEditBtnClicked()
{
    setMode(EDIT);
}

void MemoWidget::onPinBtnClicked()
{
    isPinned = !isPinned;
}

void MemoWidget::onColorBtnClicked()
{

}
