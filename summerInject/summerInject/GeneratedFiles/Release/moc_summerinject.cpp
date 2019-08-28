/****************************************************************************
** Meta object code from reading C++ file 'summerinject.h'
**
** Created: Thu Dec 13 16:11:23 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../summerinject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'summerinject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_summerInject[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x0a,
      56,   51,   13,   13, 0x0a,
     101,   13,   13,   13, 0x0a,
     110,   13,   13,   13, 0x0a,
     125,  120,   13,   13, 0x0a,
     159,   13,   13,   13, 0x0a,
     181,  170,   13,   13, 0x0a,
     205,  170,   13,   13, 0x0a,
     237,  233,   13,   13, 0x0a,
     258,   13,   13,   13, 0x0a,
     288,  285,   13,   13, 0x0a,
     313,   13,   13,   13, 0x0a,
     342,  170,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_summerInject[] = {
    "summerInject\0\0state\0CheckUseTrampolineChanged(int)\0"
    "item\0historyWidgetItemDbClicked(QListWidgetItem*)\0"
    "inject()\0onclose()\0text\0"
    "TrampolineProcessChanged(QString)\0"
    "openFile()\0showOrHide\0showOrHideDllList(bool)\0"
    "showOrHideProcessList(bool)\0PID\0"
    "refreshDllList(uint)\0OnActionTerminateProcess()\0"
    "pt\0show_contextmenu(QPoint)\0"
    "OnActionRefreshProcessList()\0"
    "ShowHistoryList(bool)\0"
};

const QMetaObject summerInject::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_summerInject,
      qt_meta_data_summerInject, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &summerInject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *summerInject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *summerInject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_summerInject))
        return static_cast<void*>(const_cast< summerInject*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int summerInject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: CheckUseTrampolineChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: historyWidgetItemDbClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: inject(); break;
        case 3: onclose(); break;
        case 4: TrampolineProcessChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: openFile(); break;
        case 6: showOrHideDllList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: showOrHideProcessList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: refreshDllList((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 9: OnActionTerminateProcess(); break;
        case 10: show_contextmenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 11: OnActionRefreshProcessList(); break;
        case 12: ShowHistoryList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
