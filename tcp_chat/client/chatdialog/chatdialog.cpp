#include <QtGui>
#include <QTextTable>
#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "client.h"

chatdialog::chatdialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    lineEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    listWidget->setFocusPolicy(Qt::NoFocus);

    QObject::connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

    tableFormat.setBorder(0);
}

void chatdialog::newParticipant(const QString &nick)
{
    static int flag = 1;
    if(flag){
        flag = 0;
        if(nick.isEmpty()){
            return;
        }
        QColor color = textEdit->textColor();
        textEdit->setTextColor(Qt::gray);
        textEdit->append(tr("* %1 has joined").arg(nick));
        textEdit->setTextColor(color);
        listWidget->addItem(" langzi@mingyue ");
    }
}


void chatdialog::participantLeft(const QString &nick)
{
    if(nick.isEmpty()){
        return;
    }
}


void chatdialog::returnPressed()
{

    QString text = lineEdit->text();
    if (text.isEmpty())
        return;

    newParticipant("<mingyue@langi>");
    appendMessage("mingyue", text);

    lineEdit->clear();
}

void chatdialog::appendMessage(const QString &from, const QString &message)
{
    if(from.isEmpty() || message.isEmpty()){
        return;
    }

    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

chatdialog::~chatdialog()
{
}
