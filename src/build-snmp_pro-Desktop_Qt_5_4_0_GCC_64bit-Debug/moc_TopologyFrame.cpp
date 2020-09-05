/****************************************************************************
** Meta object code from reading C++ file 'TopologyFrame.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../snmp_pro/TopologyFrame/TopologyFrame.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TopologyFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TopologyFrame_t {
    QByteArrayData data[5];
    char stringdata[61];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TopologyFrame_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TopologyFrame_t qt_meta_stringdata_TopologyFrame = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TopologyFrame"
QT_MOC_LITERAL(1, 14, 19), // "slot_PortConfigShow"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 17), // "slot_PortShowWarn"
QT_MOC_LITERAL(4, 53, 7) // "dwDevId"

    },
    "TopologyFrame\0slot_PortConfigShow\0\0"
    "slot_PortShowWarn\0dwDevId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TopologyFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x09 /* Protected */,
       3,    1,   30,    2, 0x09 /* Protected */,
       3,    0,   33,    2, 0x29 /* Protected | MethodCloned */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

       0        // eod
};

void TopologyFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TopologyFrame *_t = static_cast<TopologyFrame *>(_o);
        switch (_id) {
        case 0: _t->slot_PortConfigShow(); break;
        case 1: _t->slot_PortShowWarn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slot_PortShowWarn(); break;
        default: ;
        }
    }
}

const QMetaObject TopologyFrame::staticMetaObject = {
    { &Widget::staticMetaObject, qt_meta_stringdata_TopologyFrame.data,
      qt_meta_data_TopologyFrame,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TopologyFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TopologyFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TopologyFrame.stringdata))
        return static_cast<void*>(const_cast< TopologyFrame*>(this));
    return Widget::qt_metacast(_clname);
}

int TopologyFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Widget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
