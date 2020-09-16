#ifndef TARGCONTROL_REPOSITORY_H
#define TARGCONTROL_REPOSITORY_H

#include <memory>

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QThread>

#include <QtWebSockets/QWebSocket>

#include "register_device_response_model.h"

#include "web_socket/stomp_message.h"
#include "web_socket/stomp_message_serializer.h"
#include "uuid.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

class ServerAuthResultData{
public:
    bool is_success;
    QString access_token;
    QString refresh_token;
    int expires_in;
    QString server_device_id;

    ServerAuthResultData(const bool is_success, const QString &access_token,
                         const QString &refresh_token, const int expires_in, const QString &server_device_id) {
        this->is_success = is_success;
        this->access_token = access_token;
        this->refresh_token = refresh_token;
        this->expires_in = expires_in;
        this->server_device_id = server_device_id;
    }
};

class TargControlRepository: public QObject {
     Q_OBJECT
public:
    TargControlRepository(const QString &cloud_url);
    ~TargControlRepository();

    void set_auth_data(const QString &auth_login, const QString &auth_password,
                       const QString & organization_id, const QString & device_id);

    std::unique_ptr<RegisterDeviceResponseModel> register_device(const QString &auth_login, const QString &auth_password,
                                                                 const QString &device_id, const QString &name, const QString &device_type);

    void open_web_socket();

signals:
    void closed();

private slots:
    void on_connected();
    void on_text_message_received(const QString & message);

private:
    QString _cloud_url;
    QString _auth_login;
    QString _auth_password;

    QString _organization_id;
    QString _device_id;

    std::string _targcontrrol_lite_client_id;
    std::string _targcontrrol_lite_client_secret;
    std::unique_ptr<ServerAuthResultData> _auth_data;

    std::unique_ptr<httplib::Client> _http_client;

    QScopedPointer<QWebSocket> _web_socket;

    std::unique_ptr<ServerAuthResultData> _auth_in_server(const QString & auth_login, const QString & auth_password);

    QString _get_organization_id_from_auth_token(const QString & auth_token);
};

#endif // TARGCONTROL_REPOSITORY_H
