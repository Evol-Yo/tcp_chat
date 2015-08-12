#include <QApplication>
#include <sys/socket.h>
#include "chatdialog.h"
#include "client.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    chatdialog w;

    Client cli(&w);

    w.show();

    return a.exec();
}
