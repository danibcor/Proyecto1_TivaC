/****************************************************************************
** Meta object code from reading C++ file 'tiva_remotelink.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../practica1-Qt-2021/tiva_remotelink.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tiva_remotelink.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TivaRemoteLink_t {
    const uint offsetsAndSize[58];
    char stringdata0[396];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_TivaRemoteLink_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_TivaRemoteLink_t qt_meta_stringdata_TivaRemoteLink = {
    {
QT_MOC_LITERAL(0, 14), // "TivaRemoteLink"
QT_MOC_LITERAL(15, 13), // "statusChanged"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 6), // "status"
QT_MOC_LITERAL(37, 7), // "message"
QT_MOC_LITERAL(45, 15), // "messageReceived"
QT_MOC_LITERAL(61, 7), // "uint8_t"
QT_MOC_LITERAL(69, 12), // "message_type"
QT_MOC_LITERAL(82, 9), // "parameter"
QT_MOC_LITERAL(92, 15), // "startRemoteLink"
QT_MOC_LITERAL(108, 6), // "puerto"
QT_MOC_LITERAL(115, 11), // "sendMessage"
QT_MOC_LITERAL(127, 7), // "int32_t"
QT_MOC_LITERAL(135, 14), // "parameter_size"
QT_MOC_LITERAL(150, 26), // "processIncommingSerialData"
QT_MOC_LITERAL(177, 10), // "TivaStatus"
QT_MOC_LITERAL(188, 13), // "TivaConnected"
QT_MOC_LITERAL(202, 16), // "TivaDisconnected"
QT_MOC_LITERAL(219, 13), // "OpenPortError"
QT_MOC_LITERAL(233, 13), // "BaudRateError"
QT_MOC_LITERAL(247, 12), // "DataBitError"
QT_MOC_LITERAL(260, 11), // "ParityError"
QT_MOC_LITERAL(272, 9), // "StopError"
QT_MOC_LITERAL(282, 16), // "FlowControlError"
QT_MOC_LITERAL(299, 21), // "UnexpectedPacketError"
QT_MOC_LITERAL(321, 21), // "FragmentedPacketError"
QT_MOC_LITERAL(343, 15), // "CRCorStuffError"
QT_MOC_LITERAL(359, 17), // "ReceivedDataError"
QT_MOC_LITERAL(377, 18) // "TivaIsDisconnected"

    },
    "TivaRemoteLink\0statusChanged\0\0status\0"
    "message\0messageReceived\0uint8_t\0"
    "message_type\0parameter\0startRemoteLink\0"
    "puerto\0sendMessage\0int32_t\0parameter_size\0"
    "processIncommingSerialData\0TivaStatus\0"
    "TivaConnected\0TivaDisconnected\0"
    "OpenPortError\0BaudRateError\0DataBitError\0"
    "ParityError\0StopError\0FlowControlError\0"
    "UnexpectedPacketError\0FragmentedPacketError\0"
    "CRCorStuffError\0ReceivedDataError\0"
    "TivaIsDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TivaRemoteLink[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   76, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   50,    2, 0x06,    1 /* Public */,
       5,    2,   55,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    1,   60,    2, 0x0a,    7 /* Public */,
      11,    3,   63,    2, 0x0a,    9 /* Public */,
      11,    2,   70,    2, 0x0a,   13 /* Public */,
      14,    0,   75,    2, 0x08,   16 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QByteArray,    7,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 6, QMetaType::VoidStar, 0x80000000 | 12,    7,    8,   13,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QByteArray,    7,    8,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      15,   15, 0x0,   13,   81,

 // enum data: key, value
      16, uint(TivaRemoteLink::TivaConnected),
      17, uint(TivaRemoteLink::TivaDisconnected),
      18, uint(TivaRemoteLink::OpenPortError),
      19, uint(TivaRemoteLink::BaudRateError),
      20, uint(TivaRemoteLink::DataBitError),
      21, uint(TivaRemoteLink::ParityError),
      22, uint(TivaRemoteLink::StopError),
      23, uint(TivaRemoteLink::FlowControlError),
      24, uint(TivaRemoteLink::UnexpectedPacketError),
      25, uint(TivaRemoteLink::FragmentedPacketError),
      26, uint(TivaRemoteLink::CRCorStuffError),
      27, uint(TivaRemoteLink::ReceivedDataError),
      28, uint(TivaRemoteLink::TivaIsDisconnected),

       0        // eod
};

void TivaRemoteLink::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TivaRemoteLink *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->statusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->messageReceived((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 2: _t->startRemoteLink((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->sendMessage((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< void*(*)>(_a[2])),(*reinterpret_cast< int32_t(*)>(_a[3]))); break;
        case 4: _t->sendMessage((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 5: _t->processIncommingSerialData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TivaRemoteLink::*)(int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TivaRemoteLink::statusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TivaRemoteLink::*)(uint8_t , QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TivaRemoteLink::messageReceived)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TivaRemoteLink::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_TivaRemoteLink.offsetsAndSize,
    qt_meta_data_TivaRemoteLink,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_TivaRemoteLink_t
, QtPrivate::TypeAndForceComplete<TivaRemoteLink, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>, QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>, QtPrivate::TypeAndForceComplete<void *, std::false_type>, QtPrivate::TypeAndForceComplete<int32_t, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>, QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *TivaRemoteLink::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TivaRemoteLink::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TivaRemoteLink.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TivaRemoteLink::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TivaRemoteLink::statusChanged(int _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TivaRemoteLink::messageReceived(uint8_t _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
