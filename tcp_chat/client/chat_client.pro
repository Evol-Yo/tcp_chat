#-------------------------------------------------
#
# Project created by QtCreator 2015-08-09T14:42:07
#
#-------------------------------------------------

QT       += core gui
QT       += network
LIBS		+= -ljsoncpp
INCLUDEPATH		+= /program/tcp_chat/client/client
INCLUDEPATH		+= /program/tcp_chat/client/login
INCLUDEPATH		+= /program/tcp_chat/client/chatdialog
INCLUDEPATH		+= /program/tcp_chat/client/util
INCLUDEPATH		+= /program/tcp_chat/client/data_pool 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chat_client
TEMPLATE = app


SOURCES += main.cpp\
<<<<<<< HEAD
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
=======
    util/udp_data.cpp \
    util/util.cpp \
    chatdialog/chatdialog.cpp \
    client/client.cpp \
    login/login.cpp \

HEADERS  += chatdialog/chatdialog.h \
>>>>>>> branch
    data_pool/data_pool.h \
    util/my_json.h \
    util/udp_data.h \
    util/util.h \
<<<<<<< HEAD
    ui_chatdialog.h \
    ui_login.h \
    chatdialog/chatdialog.h \
=======
>>>>>>> branch
    chatdialog/ui_chatdialog.h \
    client/client.h \
    login/login.h \
    login/ui_login.h

FORMS    += ui/chatdialog.ui \
    ui/login.ui
