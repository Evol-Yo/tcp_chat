#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "ui_chatdialog.h"
#include <QDialog>

namespace Ui {
class chatdialog;
}

class chatdialog : public QDialog, public Ui::chatdialog
{
    Q_OBJECT

public:
    explicit chatdialog(QWidget *parent = 0);
    ~chatdialog();
public slots:
    //增加用户信息
    void newParticipant(const QString &nick);
    //删除用户信息
    void participantLeft(const QString &nick);
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();

private:
    QTextTableFormat tableFormat;
};

#endif // CHATDIALOG_H
