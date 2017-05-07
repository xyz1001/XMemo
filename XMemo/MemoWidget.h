/******************************************************************************
* MemoWidget.h - 便签窗体类:
*
* Copyright (c) 20017-2017  Zix
*
* DESCRIPTION: -
*   显示便签信息的窗体
*
* Author: Zhang Fan@2017/01/27
*
* modification history:
*
******************************************************************************/

#ifndef MEMOWIDGET_H
#define MEMOWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QFrame>
#include <QHash>
#include "MemoInfo.h"

const int WIDTH = 200;
const int HEIGHT = 300;
const int BUTTON_WIDTH = 30;
const int BUTTON_HEIGHT = 30;
const int COLOR_BUTTON_COUNT = 5;

enum Mode{EDIT, VIEW, SELECT};

class MemoInfo;

//颜色字符串表，便于通过颜色数值找到该颜色的字符串并进一步对应qss文件
const QHash<int, QString> COLOR_TABLE{
    {0, "red"},
    {1, "purple"},
    {2, "blue"},
    {3, "green"},
    {4, "brown"}
};

class MemoWidget : public QWidget
{
    Q_OBJECT
private:
    MemoInfo *memoInfo; //便签窗体对应的便签信息
    QPushButton *closeBtn;
    QPushButton *newBtn;
    QPushButton *pinBtn;
    QPushButton *editBtn;
    QPushButton *stayOnTopBtn;
    QPlainTextEdit *contentEditor;
    QFrame *colorBtnsFame;  //颜色切换按钮的底部背景
    QPushButton *colorBtns[COLOR_BUTTON_COUNT];
    QTextBrowser *contentView;

    int color = 0;
    QPoint relativePos;
    bool isMoving = false;
    bool isPinned = false;
    bool isStayOnTop = false;
    QHash<QObject *, int> colorBtns2Color;

    void createCloseBtn();
    void createNewBtn();
    void createEditBtn();
    void createPinBtn();
    void createStayOnTopBtn();
    void createContentEditor();
    void createColorBtns();
    void createContentView();

    void closeEvent(QCloseEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    bool eventFilter(QObject *watched, QEvent *event);

    void setTopBtnVisibility(bool visibility);  //设置顶部按钮的显示状态
    void setEditWidgetVisibility(bool visibility);  //设置编辑部件的显示状态
    void save();    //保存修改

    void loadStyleSheet(const QString colorName);

public:
    explicit MemoWidget(MemoInfo *memoInfo, bool isEditMode, QWidget *parent = 0);
    ~MemoWidget();

    MemoInfo *getMemoInfo() const;
    QString getContent() const;
    void setContent(QString content);
    int getColor() const;

    void setMode(Mode mode);    //切换便签显示模式， VIEW：窗口未激活时的便签查看模式；SELECT：窗口激活但未进入编辑状态模式；EDIT：编辑模式

signals:
    void createMemo();
    void closeMemo(MemoInfo *memoInfo);
    void memoChanged();
    void memoMoved(QPoint point);
private slots:
    void onNewBtnClicked();
    void onEditBtnClicked();
    void onPinBtnClicked();
    void onStayOnTopBtnClicked();
    void onColorBtnClicked();
};

#endif // MEMOWIDGET_H
