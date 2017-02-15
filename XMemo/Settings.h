/******************************************************************************
* Settings.h - 设置信息类:
*
* Copyright (c) 20017-2017  Zix
*
* DESCRIPTION: -
*   负责导入，保存程序的设置信息
*
* Author: Zhang Fan@2017/01/27
*
* modification history:
*
******************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
private:
    bool autorunOnBoot; //开机自启动
    QString configPath;

    Settings();
public:
    /**
     * 单例
     **/
    Settings(const Settings &) = delete;
    Settings &operator=(const Settings &) = delete;

    ~Settings();
    static Settings& getInstance()
    {
        static Settings instance;
        return instance;
    }

    void setAutorunOnBoot(bool autorunOnBoot);
    bool isAutorunOnBoot() const;
    void setConfigPath(QString configPath);
    QString getConfigPath() const;

    bool load();    //载入设置
    bool save();    //保存设置
};

#endif // SETTINGS_H
