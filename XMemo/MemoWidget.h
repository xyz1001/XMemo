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
#include "MemoInfo.h"

const int WIDTH = 200;
const int HEIGHT = 300;
const int BUTTON_WIDTH = 20;
const int BUTTON_HEIGHT = 20;
const int COLOR_BUTTON_COUNT = 10;

enum Mode{EDIT, VIEW, SELECT};

class MemoInfo;

class MemoWidget : public QWidget
{
    Q_OBJECT
private:
    MemoInfo *memoInfo; //便签窗体对应的便签信息
    QPushButton *closeBtn;
    QPushButton *newBtn;
    QPushButton *pinBtn;
    QPushButton *editBtn;
    QPlainTextEdit *contentEditor;
    QPushButton *colorBtn[COLOR_BUTTON_COUNT];
    QTextBrowser *contentView;

    Qt::GlobalColor color = Qt::yellow;

    QPoint relativePos;
    bool isMoving = false;
    bool isPinned = false;

    void closeEvent(QCloseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    bool eventFilter(QObject *watched, QEvent *event);

    void setTopBtnVisibility(bool visibility);
    void setEditWidgetVisibility(bool visibility);
    void save();

public:
    explicit MemoWidget(MemoInfo *memoInfo, bool isEditMode, QWidget *parent = 0);
    ~MemoWidget();

    MemoInfo *getMemoInfo() const;
    QString getContent() const;
    void setContent(QString content);
    Qt::GlobalColor getColor() const;
    void setColor(Qt::GlobalColor color);

    void setMode(Mode mode);

signals:
    void createMemo();
    void closeMemo(MemoInfo *memoInfo);
    void memoChanged();
    void memoMoved(QPoint point);
private slots:
    void onNewBtnClicked();
    void onEditBtnClicked();
    void onPinBtnClicked();
    void onColorBtnClicked();
};

#endif // MEMOWIDGET_H
