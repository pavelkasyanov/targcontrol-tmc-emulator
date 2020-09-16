#ifndef STOMP_MESSAGE_SERIALIZER_H
#define STOMP_MESSAGE_SERIALIZER_H

#include <QString>
#include <QTextStream>

#include "stomp_message.h"

class StompMessageSerializer {
public:
    static QString serialize(StompMessage* msg);

    static StompMessage desirialize(const QString & msg);
};

#endif // STOMP_MESSAGE_SERIALIZER_H
