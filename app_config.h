#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>
#include <QScopedPointer>
#include <QSettings>

#include <singleton.h>

class AppConfig : public QObject, public Singleton<AppConfig>
{
    Q_OBJECT
public:
    AppConfig();

    QVariant value(const QString& key, const QVariant& default_value = QVariant()) const;
    int int_value(const QString &key, const int &default_value = 0) const;
    QString str_value(const QString &key, const QString& default_value = QString()) const;
    bool bool_value(const QString &key, const int &default_value = false) const;

    void set_value(const QString &key, const QVariant &value);
private:
    QScopedPointer<QSettings> _settings;
};

#endif // APPCONFIG_H
