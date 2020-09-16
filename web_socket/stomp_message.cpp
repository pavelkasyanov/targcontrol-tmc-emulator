#include "stomp_message.h"


StompMessage::StompMessage(const QString &command, const QString &body,
                           QMap<QString, QString> headers):
    _cmd(command)
  , _body(body)
  , _headers(headers)
{

}

StompMessage::StompMessage(const StompMessage &other):
    _cmd(other._cmd)
  , _body(other._body)
  , _headers(other.headers())
{
}

StompMessage::~StompMessage()
{
}

QString StompMessage::command() const
{
    return _cmd;
}

QString StompMessage::body() const
{
    return _body;
}

QMap<QString, QString> StompMessage::headers() const
{
    return _headers;
}

void StompMessage::insert_header(const QString &key, const QString &value)
{
    _headers.insert(key, value);
}

void StompMessage::delete_header(const QString &key)
{
    _headers.remove(key);
}

QString StompMessage::get_header(const QString &key)
{
    return _headers.value(key, QString());
}

QString StompMessage::to_string()
{
    QString res = "";

    res += QString("Command: %1 \n").arg(_cmd);
    res += QString("Headers: \n");
    foreach (QString key, _headers.keys()) {
        auto val = _headers.value(key);
        res += QString("%1:%2\n").arg(key).arg(val);
    }
    res += QString("Data:\n%1").arg(_body);

    return res;
}
