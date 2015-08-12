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
    login.cpp

HEADERS  += chatdialog.h \
    client.h \
    login.h

FORMS    += chatdialog.ui \
    login.ui
