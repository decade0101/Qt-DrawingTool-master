/****************************************************************************
** Meta object code from reading C++ file 'diagramscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../diagramscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'diagramscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DiagramScene_t {
    QByteArrayData data[20];
    char stringdata0[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DiagramScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DiagramScene_t qt_meta_stringdata_DiagramScene = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DiagramScene"
QT_MOC_LITERAL(1, 13, 12), // "itemInserted"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "DiagramItem*"
QT_MOC_LITERAL(4, 40, 4), // "item"
QT_MOC_LITERAL(5, 45, 12), // "itemSelected"
QT_MOC_LITERAL(6, 58, 14), // "QGraphicsItem*"
QT_MOC_LITERAL(7, 73, 12), // "textInserted"
QT_MOC_LITERAL(8, 86, 16), // "DiagramTextItem*"
QT_MOC_LITERAL(9, 103, 11), // "textChanged"
QT_MOC_LITERAL(10, 115, 13), // "arrowInserted"
QT_MOC_LITERAL(11, 129, 13), // "scaleChanging"
QT_MOC_LITERAL(12, 143, 5), // "delta"
QT_MOC_LITERAL(13, 149, 7), // "setMode"
QT_MOC_LITERAL(14, 157, 4), // "Mode"
QT_MOC_LITERAL(15, 162, 4), // "mode"
QT_MOC_LITERAL(16, 167, 11), // "setItemType"
QT_MOC_LITERAL(17, 179, 24), // "DiagramItem::DiagramType"
QT_MOC_LITERAL(18, 204, 4), // "type"
QT_MOC_LITERAL(19, 209, 15) // "editorLostFocus"

    },
    "DiagramScene\0itemInserted\0\0DiagramItem*\0"
    "item\0itemSelected\0QGraphicsItem*\0"
    "textInserted\0DiagramTextItem*\0textChanged\0"
    "arrowInserted\0scaleChanging\0delta\0"
    "setMode\0Mode\0mode\0setItemType\0"
    "DiagramItem::DiagramType\0type\0"
    "editorLostFocus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DiagramScene[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       7,    1,   65,    2, 0x06 /* Public */,
       9,    0,   68,    2, 0x06 /* Public */,
      10,    0,   69,    2, 0x06 /* Public */,
      11,    1,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,   73,    2, 0x0a /* Public */,
      16,    1,   76,    2, 0x0a /* Public */,
      19,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 8,    4,

       0        // eod
};

void DiagramScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DiagramScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->itemInserted((*reinterpret_cast< DiagramItem*(*)>(_a[1]))); break;
        case 1: _t->itemSelected((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 2: _t->textInserted((*reinterpret_cast< DiagramTextItem*(*)>(_a[1]))); break;
        case 3: _t->textChanged(); break;
        case 4: _t->arrowInserted(); break;
        case 5: _t->scaleChanging((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setMode((*reinterpret_cast< Mode(*)>(_a[1]))); break;
        case 7: _t->setItemType((*reinterpret_cast< DiagramItem::DiagramType(*)>(_a[1]))); break;
        case 8: _t->editorLostFocus((*reinterpret_cast< DiagramTextItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DiagramTextItem* >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DiagramTextItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DiagramScene::*)(DiagramItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DiagramScene::itemInserted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DiagramScene::*)(QGraphicsItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DiagramScene::itemSelected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DiagramScene::*)(DiagramTextItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DiagramScene::textInserted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DiagramScene::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DiagramScene::textChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DiagramScene::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DiagramScene::arrowInserted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DiagramScene::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DiagramScene::scaleChanging)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DiagramScene::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_meta_stringdata_DiagramScene.data,
    qt_meta_data_DiagramScene,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DiagramScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DiagramScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DiagramScene.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int DiagramScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void DiagramScene::itemInserted(DiagramItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DiagramScene::itemSelected(QGraphicsItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DiagramScene::textInserted(DiagramTextItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DiagramScene::textChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DiagramScene::arrowInserted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void DiagramScene::scaleChanging(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
