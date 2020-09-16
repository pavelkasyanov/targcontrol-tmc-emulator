#include "uuid.h"

#include <QUuid>

QString get_uuid()
{
    QString uuid = QUuid::createUuid().toString();
    return uuid.mid(1,uuid.size()-2);
}
