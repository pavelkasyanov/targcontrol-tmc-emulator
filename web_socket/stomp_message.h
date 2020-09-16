#ifndef STOMP_MESSAGE_H
#define STOMP_MESSAGE_H

#include <QObject>
#include <QMap>

class StompMessage: public QObject {
    Q_OBJECT
public:
    StompMessage(const QString & command, const QString & body = QString(),
                 QMap<QString, QString> headers = QMap<QString, QString>());
    StompMessage(const StompMessage & other);
    ~StompMessage();

    QString command() const;
    QString body() const;
    QMap<QString, QString> headers() const;

    void insert_header(const QString & key, const QString & value);
    void delete_header(const QString & key);
    QString get_header(const QString & key);

    QString to_string();
private:
    QString  _cmd;
    QString _body;
    QMap<QString, QString> _headers;
};

#endif // STOMP_MESSAGE_H
