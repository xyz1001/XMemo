/******************************************************************************
* DbOperator.h - 数据库操作类:
*
* Copyright (c) 20017-2017  Zix
*
* DESCRIPTION: -
*   负责对数据库进行操作
*
* Author: Zhang Fan@2017/01/27
*
* modification history:
*
******************************************************************************/

#ifndef DBOPERATOR_H
#define DBOPERATOR_H

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "MemoInfo.h"

class DbOperator
{
private:
    QString lastError;

    DbOperator();

public:
    /**
     * 单例
     */
    DbOperator(const DbOperator&) = delete;
    DbOperator &operator=(const DbOperator&) = delete;

    ~DbOperator();
    static DbOperator& getInstance()
    {
        static DbOperator instance;
        return instance;
    }


    QString getLastError();

    bool init();    //初始化数据库操作
    bool read(QList<MemoInfo *> &memosList);    //读取数据库信息至memosList
    bool add(const MemoInfo &memoInfo);   //添加一条便签数据
    bool remove(const MemoInfo &memoInfo);  //删除一条便签数据
    bool modifyContent(const MemoInfo &memoInfo);   //修改一条便签的内容
    bool modifyColor(const MemoInfo &memoInfo); //修改一条便签的颜色
    bool modifyVisibility(const MemoInfo &memoInfo);    //修改一条便签的可见性
    bool modifyPosition(const MemoInfo &memoInfo);  //修改一条便签的位置
};

#endif // DBOPERATOR_H
