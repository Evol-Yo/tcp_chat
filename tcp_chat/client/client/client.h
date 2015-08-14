#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDialog>
<<<<<<< HEAD
#include "chatdialog.h"
=======
#include <vector>
#include <iostream>
#include <memory>
#include "chatdialog.h"
#include "data_pool.h"
#include "my_json.h"
#include "util.h"

using namespace std;
>>>>>>> branch

class QTcpSocket;

class Client : public QObject
{
    Q_OBJECT
public:
    Client(chatdialog *chatDialog, QObject *parent = 0);
    ~Client();
<<<<<<< HEAD
public slots:
    void newConnect();
=======

    static int recv_msg(msg_info &msg);
    static int send_msg(const msg_info &msg);

private:
    void get_user_list(msg_info &msg);
    int proc_msg(const msg_info &msg);

public slots:
    void newConnect();

>>>>>>> branch
private slots:
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketReadyRead();
    void sendData();

private:
<<<<<<< HEAD
    chatdialog *chatDialog;
    QTcpSocket *socket;
=======
	typedef shared_ptr<client_info> client_shared_ptr;
    typedef vector<client_shared_ptr> client_array_t;

    chatdialog *chatDialog;
    QTcpSocket *socket;

    int             _channel_id;
    string          _name;
    client_array_t  _clients;
>>>>>>> branch
};

#endif // CLIENT_H
