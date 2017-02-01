/******************************************************************************
* XMemo.h - 主面板类:
*
* Copyright (c) 20017-2017  Zix
*
* DESCRIPTION: -
*   负责提供对便签的显示/隐藏，删除等管理
*
* Author: Zhang Fan@2017/01/27
*
* modification history:
*
******************************************************************************/
#ifndef XMEMO_H
#define XMEMO_H

#include <QMainWindow>
#include <QList>
#include <QTableWidget>
#include <QPushButton>
#include <QHash>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QAction>
#include <QTableWidgetItem>
#include <QCheckBox>
#include "MemoInfo.h"

struct TableWidgetItemInfo
{
    QTableWidgetItem *tableWidgetItem;
    QCheckBox *checkBox;
};

class XMemo : public QMainWindow
{
    Q_OBJECT
private:
    QList<MemoInfo *> memosList; //便签信息列表
    QTableWidget *memosTableWidget;   //管理便签的表格
    QPushButton *deleteBtn;
    QHash<QObject *, MemoInfo *> visibilityCheckBoxHashMap; //用于通过控制可见性的QCheckBox的指针寻找到其对应的MemoInfo
    QHash<uint, TableWidgetItemInfo> tableWidgetItemHashMap; //用于通过便签编号寻找到其在管理面板表格中对应的那一行的items

public:
    XMemo(QWidget *parent = 0);
    ~XMemo();

    void init();    //初始化，载入设置，导入数据库以及显示桌面便签
    void deleteMemos(QList<int> selectedRows); //删除选择的便签
    void hideMemos(QList<int> selectedRows);   //隐藏选择的便签
    void showMemos(QList<int> selectedRows);   //显示选择的便签
    void setMemoVisibility(MemoInfo *memoInfo, bool visibility);  //设置某个便签的隐藏/显示状态

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *newAction;
    QAction *quitAction;
    QAction *deleteMemosAction;
    QAction *hideMemosAction;
    QAction *showMemosAction;

    void createManagePanel();   //创建便签管理面板
    void createTrayIcon();
    void addMemo(MemoInfo *memoInfo);
    void createMemoWidget(MemoInfo *memoInfo, bool isEditMode);
    void closeEvent(QCloseEvent *e);
    QList<int> getSelectedRows();

private slots:
    void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

    void onNewMemoTriggered();
    void onDeleteMemosTriggered();
    void onHideMemosTriggered();
    void onShowMemosTriggered();

public slots:
    void updateMemo(uint id);
    void onVisibilityCheckBoxClicked(bool checked); //点击visibilityCheckBox后显/隐Memo窗体
    void onMemoWidgetClosed(MemoInfo *memoInfo);    //关闭Memo窗体时间处理
};

#endif // XMEMO_H
