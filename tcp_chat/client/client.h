#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDialog>
#include "chatdialog.h"

class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT
public:
    Client(chatdialog *chatDialog, QObject *parent = 0);
    ~Client();
public slots:
    void newConnect();
private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketReadyRead();
    void sendData();

private:
    chatdialog *chatDialog;
    QTcpSocket *socket;
};

#endif // CLIENT_H
