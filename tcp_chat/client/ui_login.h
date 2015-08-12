/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_logIn
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *logIn)
    {
        if (logIn->objectName().isEmpty())
            logIn->setObjectName(QString::fromUtf8("logIn"));
        logIn->resize(357, 278);
        buttonBox = new QDialogButtonBox(logIn);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(100, 210, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(logIn);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 70, 211, 31));
        lineEdit_2 = new QLineEdit(logIn);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(110, 130, 211, 32));
        label = new QLabel(logIn);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 80, 61, 20));
        label_2 = new QLabel(logIn);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 140, 71, 20));

        retranslateUi(logIn);
        QObject::connect(buttonBox, SIGNAL(accepted()), logIn, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), logIn, SLOT(reject()));

        QMetaObject::connectSlotsByName(logIn);
    } // setupUi

    void retranslateUi(QDialog *logIn)
    {
        logIn->setWindowTitle(QApplication::translate("logIn", "\347\231\273\351\231\206", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("logIn", "\351\242\221\351\201\223 \357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("logIn", "\347\224\250\346\210\267\345\220\215\357\274\232", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class logIn: public Ui_logIn {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
