/****************************************************************************
** Meta object code from reading C++ file 'chatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "chatdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_chatdialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x0a,
      41,   12,   11,   11, 0x0a,
      79,   66,   11,   11, 0x0a,
     110,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_chatdialog[] = {
    "chatdialog\0\0nick\0newParticipant(QString)\0"
    "participantLeft(QString)\0from,message\0"
    "appendMessage(QString,QString)\0"
    "returnPressed()\0"
};

void chatdialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        chatdialog *_t = static_cast<chatdialog *>(_o);
        switch (_id) {
        case 0: _t->newParticipant((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->participantLeft((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->appendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->returnPressed(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData chatdialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject chatdialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_chatdialog,
      qt_meta_data_chatdialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &chatdialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *chatdialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *chatdialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_chatdialog))
        return static_cast<void*>(const_cast< chatdialog*>(this));
    if (!strcmp(_clname, "Ui::chatdialog"))
        return static_cast< Ui::chatdialog*>(const_cast< chatdialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int chatdialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
