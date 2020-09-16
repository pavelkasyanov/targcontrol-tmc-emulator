#ifndef REGISTER_DEVICE_RESPONSE_MODEL_H
#define REGISTER_DEVICE_RESPONSE_MODEL_H

#include <QObject>

class RegisterDeviceResponseModel: public QObject{
    Q_OBJECT
public:
    RegisterDeviceResponseModel() = default;
    RegisterDeviceResponseModel(const bool is_success, const QString &auth_login, const QString &auth_password,
                                const QString &organization_id);
    ~RegisterDeviceResponseModel();

    bool is_success() const;
    QString auth_login() const;
    QString auth_password() const;
    QString organization_id() const;
private:
    bool _is_success;
    QString _auth_login;
    QString _auth_password;
    QString _organization_id;
};

#endif // REGISTER_DEVICE_RESPONSE_MODEL_H
