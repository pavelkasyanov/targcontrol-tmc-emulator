QT -= gui
QT += websockets

CONFIG += c++14 console
CONFIG -= app_bundle

LIBS += -lssl -lcrypto

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        app_config.cpp \
        kernel_service.cpp \
        main.cpp \
        register_device_response_model.cpp \
        targcontrol_repository.cpp \
        uuid.cpp \
        web_socket/stomp_message.cpp \
        web_socket/stomp_message_serializer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    app_config.h \
    httplib.h \
    kernel_service.h \
    logger.h \
    nlohmann/json.hpp \
    register_device_response_model.h \
    singleton.h \
    targcontrol_repository.h \
    uuid.h \
    web_socket/stomp_message.h \
    web_socket/stomp_message_serializer.h

DISTFILES += \
    config.ini

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else {
    DESTDIR = build/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
