#include "DbOperator.h"
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDriver>
#include <QApplication>

DbOperator::DbOperator()
{

}

DbOperator::~DbOperator()
{

}

QString DbOperator::getLastError()
{
    return lastError;
}

bool DbOperator::init()
{
    /**
     * @brief db 必须是局部变量，不能是全局或成员变量，否则析构时会崩溃(待解决)
     */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString path = qApp->applicationDirPath();
    db.setDatabaseName(path + "/xmemo.db");

    if(!db.open())
    {
        lastError = "Database open failed\n";
        return false;
    }

    QSqlQuery query;
    if(query.exec("CREATE TABLE IF NOT EXISTS xmemo("
                  "id INT PRIMARY KEY,"
                  "content TEXT,"
                  "color INT,"
                  "visibility INT,"
                  "position_x INT,"
                  "position_y INT)"))
        return true;
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::read(QList<MemoInfo *> &memosList)
{
    QSqlQuery query;
    if(query.exec("SELECT * FROM xmemo;"))
    {
        QSqlRecord record;
        while(query.next())
        {
            record=query.record();
            MemoInfo *memoInfo = new MemoInfo(record.value("id").toUInt(),
                                              record.value("content").toString(),
                                              record.value("color").toInt(),
                                              bool(record.value("visibility").toInt()),
                                              QPoint(record.value("position_x").toInt(),
                                              record.value("position_y").toInt()));
            memosList.push_back(memoInfo);
        }
        return true;
    }
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::add(const MemoInfo &memoInfo)
{
    QSqlQuery query;
    query.prepare("INSERT INTO xmemo VALUES(?,?,?,?,?,?)");

    query.bindValue(0, memoInfo.getId());
    query.bindValue(1, memoInfo.getContent());
    query.bindValue(2, int(memoInfo.getColor()));
    query.bindValue(3, int(memoInfo.isVisible()));
    query.bindValue(4, memoInfo.getPosition().x());
    query.bindValue(5, memoInfo.getPosition().y());

    if(query.exec())
        return true;
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::remove(const MemoInfo &memoInfo)
{
    QSqlQuery query;
    QString sql = QString("DELETE FROM xmemo where id = %1").arg(memoInfo.getId());
    if(query.exec(sql))
        return true;
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::modifyContent(const MemoInfo &memoInfo)
{
    QSqlQuery query;
    QString sql = QString("UPDATE xmemo set content = \"%1\" where id = \"%2\"")
                  .arg(memoInfo.getContent())
                  .arg(memoInfo.getId());
    if(query.exec(sql))
        return true;
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::modifyColor(const MemoInfo &memoInfo)
{
    QSqlQuery query;
    QString sql = QString("UPDATE xmemo set color = \"%1\" where id = \"%2\"")
                  .arg(memoInfo.getColor())
                  .arg(memoInfo.getId());
    if(query.exec(sql))
        return true;
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::modifyVisibility(const MemoInfo &memoInfo)
{
    QSqlQuery query;
    QString sql = QString("UPDATE xmemo set visibility = \"%1\" where id = \"%2\"")
                  .arg(int(memoInfo.isVisible()))
                  .arg(memoInfo.getId());
    if(query.exec(sql))
        return true;
    else
    {
        lastError = query.lastError().driverText();
        return false;
    }
}

bool DbOperator::modifyPosition(const MemoInfo &memoInfo)
{
    QSqlQuery query;
    QString sql = QString("UPDATE xmemo set position_x = \"%1\" where id = \"%2\"")
                  .arg(memoInfo.getPosition().x())
                  .arg(memoInfo.getId());
    if(query.exec(sql))
    {
        sql = QString("UPDATE xmemo set position_y = \"%1\" where id = \"%2\"")
                      .arg(memoInfo.getPosition().y())
                      .arg(memoInfo.getId());
        if(query.exec(sql))
            return true;
    }

    lastError = query.lastError().driverText();
    return false;
}
