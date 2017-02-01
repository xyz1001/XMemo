#include "Settings.h"
#include <QSettings>
#include <QFile>

Settings::Settings()
{
    autorunOnBoot = false;
    autoHide = true;
}

Settings::~Settings()
{
    // TODO
}

void Settings::setAutorunOnBoot(bool autorunOnBoot)
{
    this->autorunOnBoot = autorunOnBoot;
}

void Settings::setAutoHide(bool autoHide)
{
    this->autoHide = autoHide;
}

bool Settings::isAutorunOnBoot() const
{
    return autorunOnBoot;
}

bool Settings::isAutoHide() const
{
    return autoHide;
}

bool Settings::load()
{
    QSettings settings(".xmemo.cfg", QSettings::IniFormat);
    if(settings.contains("exists"))
    {
        autorunOnBoot = settings.value("autorunOnBoot").toBool();
        autoHide = settings.value("autoHide").toBool();
        return true;
    }
    else
        return false;
}

bool Settings::save()
{
    QSettings settings(".xmemo.cfg", QSettings::IniFormat);
    if(settings.isWritable())
    {
        settings.setValue("exists", true);
        settings.setValue("autorunOnBoot", autorunOnBoot);
        settings.setValue("autoHide", autoHide);
        settings.sync();
        return true;
    }
    else
        return false;
}
