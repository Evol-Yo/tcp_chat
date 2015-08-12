#include <QtGui>
#include <QtNetwork>
#include <QDialog>
#include "client.h"

Client::Client(chatdialog *chat, QObject  *parent)
    : QObject(parent)
{
    chatDialog = chat;
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 49998);
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                   this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(readyRead()),
                   this, SLOT(socketReadyRead()));
}

Client::~Client()
{}

void Client::newConnect()
{

    if(socket->state() == QAbstractSocket::UnconnectedState){
        socket->abort();
    }
}

void Client::socketStateChanged(QAbstractSocket::SocketState state)
{
    //this->updateEnabledState();
    if (state == QAbstractSocket::UnconnectedState) {
        socket->deleteLater();
        socket = 0;
    }
}

void Client::socketReadyRead()
{
    char buf[512];
    socket->readLine(buf, sizeof(buf));
    chatDialog->appendMessage("mingyue", buf);
}

void Client::sendData()
{
    QString input = "hello world";
    socket->write(input.toUtf8() + "\r\n");
}
