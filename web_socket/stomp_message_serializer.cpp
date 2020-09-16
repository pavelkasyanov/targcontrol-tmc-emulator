#include "stomp_message_serializer.h"

QString StompMessageSerializer::serialize(StompMessage *msg)
{
    QString result;

    result += QString("%1\n").arg(msg->command());
    auto headers = msg->headers();
    if (headers.size() > 0) {
        foreach (QString key, headers.keys()) {
            auto val = headers.value(key);
            result += QString("%1:%2\n").arg(key).arg(val);
        }
    }
    result += "\n";
    result += msg->body();
    result += '\0';

    return result;
}

StompMessage StompMessageSerializer::desirialize(const QString &msg)
{
    QTextStream reader(msg.toUtf8());

    auto cmd = reader.readLine();

    QMap<QString, QString> headers;
    auto header = reader.readLine();
    while (!header.isNull() && !header.isEmpty()) {
        auto split = header.split(':');
        if (split.length() == 2) {
            auto key = split[0].trimmed();
            auto value = split[1].trimmed();
            headers[key] = value;

            header = reader.readLine();
        }
    }

    auto body = reader.readAll();
    if (!body.isNull() && !body.isEmpty()) {
        body = body.trimmed().replace('\r', "").replace('\0', "").replace('\n', "");
    }

    return StompMessage(cmd, body, headers);
}
