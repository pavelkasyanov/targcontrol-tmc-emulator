#include "targcontrol_repository.h"

#include "logger.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

TargControlRepository::TargControlRepository(const QString &cloud_url):
    _cloud_url(cloud_url)
  , _auth_login("")
  , _auth_password()
  , _targcontrrol_lite_client_id("clientIdPassword")
  , _targcontrrol_lite_client_secret("secret")
  , _http_client(new httplib::Client(cloud_url.toStdString().c_str()))
  , _web_socket(new QWebSocket())
{
    _http_client->enable_server_certificate_verification(false);

    connect(_web_socket.data(), SIGNAL(connected()), this, SLOT(on_connected()));
    connect(_web_socket.data(), SIGNAL(disconnected()), this, SLOT(closed()));
}

TargControlRepository::~TargControlRepository()
{

}

void TargControlRepository::set_auth_data(const QString &auth_login, const QString &auth_password,
                                          const QString & organization_id, const QString & device_id)
{
    _auth_login = auth_login;
    _auth_password = auth_password;
    _organization_id = organization_id;
    _device_id = device_id;
}

std::unique_ptr<RegisterDeviceResponseModel> TargControlRepository::register_device(const QString &auth_login, const QString &auth_password,
                                                                                    const QString &device_id, const QString &name, const QString &device_type)
{
    try {
        auto registration_url_t = (_cloud_url + "/api/v1/devices-registration");
        QByteArray ba = registration_url_t.toLocal8Bit();
        const char *registration_url = ba.data();

        std::unique_ptr<ServerAuthResultData> auth_data = _auth_in_server(auth_login, auth_password);

        _http_client->set_bearer_token_auth(auth_data->access_token.toLocal8Bit().data());

        auto data = nlohmann::json {
            { "deviceId", device_id.toStdString()},
            { "name", name.toStdString()},
            { "deviceType", device_type.toStdString()}
            };
        auto res_js = data.dump();

        auto res = _http_client->Post(registration_url, res_js, "application/json");

        auto j = json::parse(res.value().body);
        auto login = QString::fromStdString(j.value("login", ""));
        auto password = QString::fromStdString(j.value("password", ""));
        auto org_id = _get_organization_id_from_auth_token(auth_data->access_token);

        return std::make_unique<RegisterDeviceResponseModel>(true, login, password, org_id);

    } catch (const std::exception &ex) {
        CRITICAL_LOG( "TargControlRepository - exception in register: "  << ex.what())
        return std::make_unique<RegisterDeviceResponseModel>(false, "", "", "");
    }
}

void TargControlRepository::open_web_socket()
{
    QUrl cloud_url_t(_cloud_url);
    auto socket_prefix = cloud_url_t.scheme() == "https" ? QString("wss") : QString("ws");
    auto websocket_url = socket_prefix  + "://" + cloud_url_t.authority() + "/socket/websocket";

    _web_socket->open(QUrl(websocket_url));
}

void TargControlRepository::on_connected()
{
    connect(_web_socket.data(), SIGNAL(textMessageReceived(const QString &)), this, SLOT(on_text_message_received(const QString &)));

    QScopedPointer<StompMessage> connect_msg(new StompMessage("CONNECT"));
    connect_msg->insert_header("accept-version", "1.1");
    connect_msg->insert_header("heart-beat", "10000,10000");

    auto res = StompMessageSerializer::serialize(connect_msg.data());
    _web_socket->sendTextMessage(res);

    QScopedPointer<StompMessage> sub_rfid(new StompMessage("SUBSCRIBE"));
    sub_rfid->insert_header("id", "sub-" + get_uuid());
    sub_rfid->insert_header("destination",
                            QString("/topic/%1/verify_employee_by_rfid/%2").arg(_organization_id).arg(_device_id));

    res = StompMessageSerializer::serialize(sub_rfid.data());
    _web_socket->sendTextMessage(res);

    QScopedPointer<StompMessage> sub_finger(new StompMessage("SUBSCRIBE"));
    sub_finger->insert_header("id", "sub-" + get_uuid());
    sub_finger->insert_header("destination",
                            QString("/topic/%1/verify_employee_by_finger/%2").arg(_organization_id).arg(_device_id));

    res = StompMessageSerializer::serialize(sub_finger.data());
    _web_socket->sendTextMessage(res);


}

void TargControlRepository::on_text_message_received(const QString & message)
{
   auto server_cmd = StompMessageSerializer::desirialize(message);

   DEBUG_LOG("TargControlRepository - StompMessage: " << server_cmd.to_string())

   if ( !server_cmd.body().isNull() && !server_cmd.body().isEmpty() ) {
        auto dest = server_cmd.headers()["destination"];
        if (dest.contains("verify_employee_by_rfid") || dest.contains("verify_employee_by_finger")) {
            auto cmd_name = QString("verify_employee_by_finger_response");
            if (dest.contains("verify_employee_by_rfid")) {
                cmd_name = QString("verify_employee_by_rfid_response");
            }

            auto j = json::parse(server_cmd.body().toStdString());

            auto data = nlohmann::json {
                { "result", true},
                { "commandId", j.value("id", "")}
            };
            auto res_js = data.dump();

            QScopedPointer<StompMessage> result_msg(new StompMessage("SEND", QString::fromStdString(res_js)));
            result_msg->insert_header("content-type", "application/json");
            result_msg->insert_header("destination", QString("/app/%1/%2/%3").arg(_organization_id).arg(cmd_name).arg(_device_id));

            auto res = StompMessageSerializer::serialize(result_msg.data());

            QThread::currentThread()->sleep(3);

            _web_socket->sendTextMessage(res);
        }
   }
}

std::unique_ptr<ServerAuthResultData> TargControlRepository::_auth_in_server(const QString &auth_login, const QString &auth_password)
{
    try {
        auto auth_url_t = _cloud_url + "/oauth/token";
        QByteArray ba = auth_url_t.toLocal8Bit();
        const char *auth_url = ba.data();

        _http_client->set_basic_auth(_targcontrrol_lite_client_id.c_str(),
                                     _targcontrrol_lite_client_secret.c_str());

        httplib::MultipartFormDataItems items = {
            { "grant_type", "password", "", "" },
            { "username", auth_login.toStdString(), "", ""  },
            { "password", auth_password.toStdString(), "", ""  }
        };

        auto res = _http_client->Post(auth_url, items);

        auto j = json::parse(res.value().body);
        auto access_token = QString::fromStdString(j.value("access_token", ""));
        auto refresh_token = QString::fromStdString(j.value("refresh_token", ""));
        auto expires_in = j.value<int>("expires_in", 0);

        _http_client->set_basic_auth("", "");

        return std::make_unique<ServerAuthResultData>(true, access_token, refresh_token, expires_in, QString());

    } catch (const std::exception &ex) {
        CRITICAL_LOG( "TargControlRepository - exception in _auth_in_server: "  << ex.what())
        return std::make_unique<ServerAuthResultData>(false, QString(), QString(), 0, QString());
    }
}

QString TargControlRepository::_get_organization_id_from_auth_token(const QString &auth_token)
{

    auto t = auth_token.split('.');
    auto data =  QString(QByteArray::fromBase64(t[1].toUtf8()));
    auto j = json::parse(data.toStdString());

    auto organization_id = QString::fromStdString(j.value("organizationId", ""));

    return organization_id;
}
