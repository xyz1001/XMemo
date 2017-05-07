#include "Settings.h"
#include <QSettings>
#include <QFile>
#include <QDir>

Settings::Settings()
{
    autorunOnBoot = false;
    configPath = QDir::homePath()+"/.config/xmemo/";
    if (!QDir(configPath).exists())
    {
        QDir().mkdir(configPath);
    }
}

Settings::~Settings()
{

}

void Settings::setAutorunOnBoot(bool autorunOnBoot)
{
    this->autorunOnBoot = autorunOnBoot;
}

bool Settings::isAutorunOnBoot() const
{
    return autorunOnBoot;
}

void Settings::setConfigPath(QString configPath)
{
    this->configPath = configPath;
}

QString Settings::getConfigPath() const
{
    return this->configPath;
}

bool Settings::load()
{
    QSettings settings(configPath + "/xmemo.cfg", QSettings::IniFormat);
    if(settings.contains("exists"))
    {
        autorunOnBoot = settings.value("autorunOnBoot").toBool();
        return true;
    }
    else
        return false;
}

bool Settings::save()
{
    QSettings settings(configPath + "/xmemo.cfg", QSettings::IniFormat);

    if(settings.isWritable())
    {
        settings.setValue("exists", true);
        settings.setValue("autorunOnBoot", autorunOnBoot);
        settings.sync();
        return true;
    }
    else
        return false;
}
