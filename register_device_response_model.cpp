#include "register_device_response_model.h"

RegisterDeviceResponseModel::RegisterDeviceResponseModel(const bool is_success, const QString &auth_login, const QString &auth_password,
                                                         const QString & organization_id):
    _is_success(is_success)
  , _auth_login(auth_login)
  , _auth_password(auth_password)
  , _organization_id(organization_id)
{
}

RegisterDeviceResponseModel::~RegisterDeviceResponseModel()
{
}

bool RegisterDeviceResponseModel::is_success() const
{
    return _is_success;
}

QString RegisterDeviceResponseModel::auth_login() const
{
    return _auth_login;
}

QString RegisterDeviceResponseModel::auth_password() const
{
    return _auth_password;
}

QString RegisterDeviceResponseModel::organization_id() const
{
    return _organization_id;
}
