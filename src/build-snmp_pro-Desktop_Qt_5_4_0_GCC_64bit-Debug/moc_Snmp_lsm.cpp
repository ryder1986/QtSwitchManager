/****************************************************************************
** Meta object code from reading C++ file 'Snmp_lsm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../snmp_pro/snmp/Snmp_lsm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Snmp_lsm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Snmp_Lsm_t {
    QByteArrayData data[6];
    char stringdata[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Snmp_Lsm_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Snmp_Lsm_t qt_meta_stringdata_Snmp_Lsm = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Snmp_Lsm"
QT_MOC_LITERAL(1, 9, 19), // "signal_SendPortWarn"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 19), // "signal_SendWarnText"
QT_MOC_LITERAL(4, 50, 6), // "strOid"
QT_MOC_LITERAL(5, 57, 6) // "strMsg"

    },
    "Snmp_Lsm\0signal_SendPortWarn\0\0"
    "signal_SendWarnText\0strOid\0strMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Snmp_Lsm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    2,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    4,    5,

       0        // eod
};

void Snmp_Lsm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Snmp_Lsm *_t = static_cast<Snmp_Lsm *>(_o);
        switch (_id) {
        case 0: _t->signal_SendPortWarn(); break;
        case 1: _t->signal_SendWarnText((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Snmp_Lsm::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Snmp_Lsm::signal_SendPortWarn)) {
                *result = 0;
            }
        }
        {
            typedef void (Snmp_Lsm::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Snmp_Lsm::signal_SendWarnText)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject Snmp_Lsm::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Snmp_Lsm.data,
      qt_meta_data_Snmp_Lsm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Snmp_Lsm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Snmp_Lsm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Snmp_Lsm.stringdata))
        return static_cast<void*>(const_cast< Snmp_Lsm*>(this));
    return QObject::qt_metacast(_clname);
}

int Snmp_Lsm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Snmp_Lsm::signal_SendPortWarn()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Snmp_Lsm::signal_SendWarnText(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
