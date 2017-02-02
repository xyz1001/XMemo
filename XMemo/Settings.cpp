#include "Settings.h"
#include <QSettings>
#include <QFile>
#include <QApplication>

Settings::Settings()
{
    autorunOnBoot = false;
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

bool Settings::load()
{
    QString path = qApp->applicationDirPath();
    QSettings settings(path + "/.xmemo.cfg", QSettings::IniFormat);
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
    QString path = qApp->applicationDirPath();
    QSettings settings(path + "/.xmemo.cfg", QSettings::IniFormat);

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
