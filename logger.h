#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>

#ifdef QT_DEBUG
    #define DEBUG_LOG(x) (qDebug() << x);
#else
    #define DEBUG_LOG(x)
#endif

#define INFO_LOG(x) (qInfo() << x);
#define WARN_LOG(x) (qWarning() << x);
#define CRITICAL_LOG(x) (qCritical() << x);

#endif // LOGGER_H
