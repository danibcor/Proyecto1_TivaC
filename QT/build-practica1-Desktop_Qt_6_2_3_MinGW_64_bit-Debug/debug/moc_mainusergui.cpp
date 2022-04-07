/****************************************************************************
** Meta object code from reading C++ file 'mainusergui.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../practica1-Qt-2021/mainusergui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainusergui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainUserGUI_t {
    const uint offsetsAndSize[40];
    char stringdata0[276];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainUserGUI_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainUserGUI_t qt_meta_stringdata_MainUserGUI = {
    {
QT_MOC_LITERAL(0, 11), // "MainUserGUI"
QT_MOC_LITERAL(12, 10), // "cambiaLEDs"
QT_MOC_LITERAL(23, 0), // ""
QT_MOC_LITERAL(24, 17), // "tivaStatusChanged"
QT_MOC_LITERAL(42, 6), // "status"
QT_MOC_LITERAL(49, 7), // "message"
QT_MOC_LITERAL(57, 15), // "messageReceived"
QT_MOC_LITERAL(73, 7), // "uint8_t"
QT_MOC_LITERAL(81, 4), // "type"
QT_MOC_LITERAL(86, 5), // "datos"
QT_MOC_LITERAL(92, 10), // "cambiaMODO"
QT_MOC_LITERAL(103, 20), // "colorWheel_cambiaRGB"
QT_MOC_LITERAL(124, 4), // "arg1"
QT_MOC_LITERAL(129, 12), // "cambiaBrillo"
QT_MOC_LITERAL(142, 5), // "value"
QT_MOC_LITERAL(148, 20), // "on_runButton_clicked"
QT_MOC_LITERAL(169, 41), // "on_serialPortComboBox_current..."
QT_MOC_LITERAL(211, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(233, 20), // "on_ADCButton_clicked"
QT_MOC_LITERAL(254, 21) // "on_pingButton_clicked"

    },
    "MainUserGUI\0cambiaLEDs\0\0tivaStatusChanged\0"
    "status\0message\0messageReceived\0uint8_t\0"
    "type\0datos\0cambiaMODO\0colorWheel_cambiaRGB\0"
    "arg1\0cambiaBrillo\0value\0on_runButton_clicked\0"
    "on_serialPortComboBox_currentIndexChanged\0"
    "on_pushButton_clicked\0on_ADCButton_clicked\0"
    "on_pingButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainUserGUI[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    2,   81,    2, 0x08,    2 /* Private */,
       6,    2,   86,    2, 0x08,    5 /* Private */,
      10,    0,   91,    2, 0x08,    8 /* Private */,
      11,    1,   92,    2, 0x08,    9 /* Private */,
      13,    1,   95,    2, 0x08,   11 /* Private */,
      15,    0,   98,    2, 0x08,   13 /* Private */,
      16,    1,   99,    2, 0x08,   14 /* Private */,
      17,    0,  102,    2, 0x08,   16 /* Private */,
      18,    0,  103,    2, 0x08,   17 /* Private */,
      19,    0,  104,    2, 0x08,   18 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    4,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QByteArray,    8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,   12,
    QMetaType::Void, QMetaType::Double,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainUserGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainUserGUI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->cambiaLEDs(); break;
        case 1: _t->tivaStatusChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->messageReceived((*reinterpret_cast< uint8_t(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 3: _t->cambiaMODO(); break;
        case 4: _t->colorWheel_cambiaRGB((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 5: _t->cambiaBrillo((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->on_runButton_clicked(); break;
        case 7: _t->on_serialPortComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_pushButton_clicked(); break;
        case 9: _t->on_ADCButton_clicked(); break;
        case 10: _t->on_pingButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject MainUserGUI::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainUserGUI.offsetsAndSize,
    qt_meta_data_MainUserGUI,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainUserGUI_t
, QtPrivate::TypeAndForceComplete<MainUserGUI, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<uint8_t, std::false_type>, QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QColor &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<double, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MainUserGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainUserGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainUserGUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainUserGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
