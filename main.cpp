#include <memory>

#include <QObject>
#include <QCoreApplication>
#include <QThread>

#include "app_config.h"
#include "logger.h"

#include "targcontrol_repository.h"
#include "kernel_service.h"

#ifdef QT_DEBUG
#define DEFAULT_CLOUD_URL "http://195.222.65.56:55555"
#else
#define DEFAULT_CLOUD_URL "https://cloud.targcontrol.com"
#endif

#define DEFAULT_DEVICE_ID "e434470f-8ae8-47d4-9eec-af3df80b3da5"
#define DEVICE_TYPE "USB"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    auto cloud_url = AppConfig::instance().str_value("Settings/cloud_url", DEFAULT_CLOUD_URL);
    auto device_id = AppConfig::instance().str_value("Settings/device_id", DEFAULT_DEVICE_ID);
    auto login = AppConfig::instance().str_value("Settings/login", "");
    auto password = AppConfig::instance().str_value("Settings/password", "");
    auto organization_id = AppConfig::instance().str_value("Settings/organization_id", "");

   QScopedPointer<TargControlRepository> targ_repo(new TargControlRepository(cloud_url));

    auto is_device_register = AppConfig::instance().bool_value("Settings/is_device_register");
    if ( !is_device_register ) {
        INFO_LOG( "device not register, need register device" )

        QTextStream s(stdin);

        INFO_LOG( "enter login:" )
        //login = s.readLine();
        login = "rfe.oleg@gmail.com";

        INFO_LOG( "enter password:" )
        //password = s.readLine();
        password = "password";

        auto reg_res = targ_repo->register_device(login, password, device_id, "TEST_USB_1", DEVICE_TYPE);
        if ( !reg_res->is_success() ) {
            INFO_LOG( "ERROR register device" )
            return 1;
        } else {
            INFO_LOG( "SUCCESS register device" )

            AppConfig::instance().set_value("Settings/is_device_register", true);
            AppConfig::instance().set_value("Settings/login", reg_res->auth_login());
            AppConfig::instance().set_value("Settings/password", reg_res->auth_password());
            AppConfig::instance().set_value("Settings/organization_id", reg_res->organization_id());
            AppConfig::instance().set_value("Settings/device_id", device_id);

            login = reg_res->auth_login();
            password = reg_res->auth_password();
            organization_id = reg_res->organization_id();
        }

    } else {
        INFO_LOG( "device is register not need register device" )
    }

    targ_repo->set_auth_data(login, password, organization_id, device_id);

    QScopedPointer<KernelService> kernel_service(new KernelService(targ_repo.data()));
    kernel_service->do_work();

    return a.exec();
}
