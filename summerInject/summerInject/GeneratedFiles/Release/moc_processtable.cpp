/****************************************************************************
** Meta object code from reading C++ file 'processtable.h'
**
** Created: Thu Dec 13 16:11:23 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../processtable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'processtable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProcessTable[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   14,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ProcessTable[] = {
    "ProcessTable\0\0PID\0selectProcessChanged(uint)\0"
};

const QMetaObject ProcessTable::staticMetaObject = {
    { &QTableView::staticMetaObject, qt_meta_stringdata_ProcessTable,
      qt_meta_data_ProcessTable, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProcessTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProcessTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProcessTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProcessTable))
        return static_cast<void*>(const_cast< ProcessTable*>(this));
    return QTableView::qt_metacast(_clname);
}

int ProcessTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectProcessChanged((*reinterpret_cast< uint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ProcessTable::selectProcessChanged(unsigned int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
