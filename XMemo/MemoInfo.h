/******************************************************************************
* MemoInfo.h - 便签信息类:
*
* Copyright (c) 20017-2017  Zix
*
* DESCRIPTION: -
*   用于保存便签的信息及编辑便签信息
*
* Author: Zhang Fan@2017/01/27
*
* modification history:
*
******************************************************************************/

#ifndef MEMOINFO_H
#define MEMOINFO_H

#include <QString>
#include <QObject>
#include "MemoWidget.h"

class MemoWidget;

class MemoInfo : public QObject
{
    Q_OBJECT
private:
    unsigned int id;    //便签ID，用时间戳表示
    QString content;    //便签内容
    Qt::GlobalColor color;  //便签颜色
    MemoWidget *memoWidget = nullptr; //便签对应窗体
    bool visibility;    //是否在桌面显示
    QPoint pos = QPoint(0, 0);
public:
    MemoInfo();
    MemoInfo(unsigned int id, QString content, Qt::GlobalColor color, bool visibility, QPoint pos);
    ~MemoInfo();

    QString getDate() const;
    unsigned int getId() const;
    QString getContent() const;
    void setContent(const QString &content);
    Qt::GlobalColor getColor() const;
    void setColor(Qt::GlobalColor color);
    MemoWidget *getMemoWidget() const;  //窗体隐藏时返回nullptr
    bool isVisible() const;
    QPoint getPosition() const;
    void setPosition(QPoint pos);

    void createWidget(bool isEditMode);
    void removeWidget();

signals:
    void memoChanged(uint id);
public slots:
    void onMemoChanged();
    void onMemoMoved(QPoint point);
};

#endif // MEMOINFO_H
