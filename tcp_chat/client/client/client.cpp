#include <QtGui>
#include <QtNetwork>
#include <QDialog>
#include "client.h"

Client::Client(chatdialog *chat, QObject  *parent)
    : QObject(parent)
{
    chatDialog = chat;
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 49999);
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

int Client::recv_msg(int sock, msg_info &msg)
{
    char buf[BUF_SIZE + 13];
    bzero(buf, sizeof(buf));

    //先读取id字段和长度字段
    ssize_t size = 0;
    do{
        int len = recv(sock, buf + size, 10-size, 0);
        if(len < 0){
            log_error("recv error");
            return len;
        }
        size += len;
    } while(size < 10);

    if(buf[0] != '%' || buf[1] != '#'){
        log_error("recv error");
        //我们应该尝试恢复数据(找到下一个以%#开头的字符序列)， 为了测试，我们直接退出
        return -1;
    }
    int msglen = atoi(buf + 6);
    bzero(buf + 6, 4);
    int id = atoi(buf+2);
    bzero(buf, 6);

    //再读取消息字段
    size = 0;
    do{
        int len = recv(sock, buf + size, msglen - size + 2, 0);
        if(len < 0){
            log_error("recv error");
            return len;
        }
        size += len;
    } while(size < msglen + 2);

    if(buf[msglen] != '%' || buf[msglen+1] != '#'){
        log_error("recv error!");
        return -1;
    }
    buf[msglen] = '\0';
    buf[msglen + 1] = '\0';

    msg._head._id = id;
    msg._head._msglen = msglen;
    msg._msg = buf;

    return msglen;
}

int server::send_msg(int sock, const msg_info &msg)
{
    string str;
    char buf[BUF_SIZE + 13];
    bzero(buf , sizeof(buf));
    //加上长度字段,和特殊标志，用以检查错误
    sprintf(buf, "%%#");
    sprintf(buf + 2, "%4d", msg._head._id);
    sprintf(buf+6, "%4d", msg._head._msglen);
    sprintf(buf+10, "%s", msg._msg.c_str());
    sprintf(buf+10+msg._msg.length(), "%%#");

    return send(sock, buf, msg._head._msglen+12, 0);
}


int Client::proc_msg(msg_info &msg)
{
    int conn = 0;
    int cmd = data._cmd;
    switch(cmd){
        case CMD_MSG:
            data_info::val_to_str(data, msg._msg);
            msg._head._id = _id;
            msg._head._msglen = msg._msg.length();
            broadcast(msg);
            break;
        case CMD_REGISTER:
            {
                int i = 4;
                char buf[5];
                client_info cli;
                //获取连接的套结字,server模块会将套接字描述符添加到data._data的尾部(占4个字符)
                bzero(buf, sizeof(buf));
                for(; i < 4; i++){
                    buf[i] = data._data[data._data.size() - 4 + i];
                    data._data[data._data.size() - 4 + i] = 0;
                }
                conn = atoi(buf);

                client_info::str_to_val(data._data, cli);

                //将该用户注册到好友列表中
                if(register_user(cli) < 0){
                    log_error("register_user error");
                    return -1;
                }
                log_message("register_user success, user:%s", data._data.c_str());
                //在这里我们应该关闭该套接字，以使客户端重新登陆，为了方便，我们直接
                //将其添加到在线列表中
                //close(conn)
                if(add_online_user(conn, cli) < 0){
                    close(conn);
                    return -1;
                }


                //将其加入到epoll事件中
                if(add_event(conn) < 0){
                    close(conn);
                    return -1;
                }
                get_user_list(msg);
                server::send_msg(conn, msg);
            }
            break;
        case CMD_ONLINE:
            {
                client_info cli;
                int i = 4;
                char buf[5];
                bzero(buf, sizeof(buf));
                for(; i < 4; i++){
                    buf[i] = data._data[data._data.size() - 4 + i];
                    data._data[data._data.size() - 4 + i] = 0;
                }
                conn = atoi(buf);

                client_info::str_to_val(data._data, cli);
                if(add_online_user(conn, cli) < 0){
                    close(conn);
                    return -1;
                }
                //将其加入到epoll事件中
                if(add_event(conn) < 0){
                    close(conn);
                    return -1;
                }
                get_user_list(msg);
                server::send_msg(conn, msg);
            }
            break;
        default:
            break;
    }

    return 0;
}

