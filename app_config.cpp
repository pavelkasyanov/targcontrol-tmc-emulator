#include "app_config.h"

AppConfig::AppConfig():
     _settings(new QSettings("settings.ini", QSettings::IniFormat))
{
}

QVariant AppConfig::value(const QString &key, const QVariant &default_value) const
{
    return _settings->value(key, default_value);
}

int AppConfig::int_value(const QString &key, const int &default_value) const
{
    return value(key, default_value).value<int>();
}

QString AppConfig::str_value(const QString &key, const QString &default_value) const
{
    return value(key, default_value).value<QString>();
}

bool AppConfig::bool_value(const QString &key, const int &default_value) const
{
    return value(key, default_value).value<bool>();
}

void AppConfig::set_value(const QString &key, const QVariant &value)
{
    _settings->setValue(key, value);
    _settings->sync();
}
