#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T14:42:07
#
#-------------------------------------------------

QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat_client
TEMPLATE = app


SOURCES += main.cpp\
        chatdialog.cpp \
    client.cpp \
    login.cpp \
    util/udp_data.cpp \
    util/util.cpp \
    moc_chatdialog.cpp \
    moc_client.cpp \
    moc_login.cpp \
    chatdialog/chatdialog.cpp \
    chatdialog/moc_chatdialog.cpp \
    client/client.cpp \
    client/moc_client.cpp \
    login/login.cpp \
    login/moc_login.cpp

HEADERS  += chatdialog.h \
    client.h \
    login.h \
    data_pool/data_pool.h \
    util/my_json.h \
    util/udp_data.h \
    util/util.h \
    ui_chatdialog.h \
    ui_login.h \
    chatdialog/chatdialog.h \
    chatdialog/ui_chatdialog.h \
    client/client.h \
    login/login.h \
    login/ui_login.h

FORMS    += chatdialog.ui \
    login.ui
