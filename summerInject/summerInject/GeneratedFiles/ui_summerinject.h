/********************************************************************************
** Form generated from reading UI file 'summerinject.ui'
**
** Created: Mon Jun 25 15:19:50 2012
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUMMERINJECT_H
#define UI_SUMMERINJECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_summerInjectClass
{
public:
    QAction *actionInject;
    QAction *action_OpenFile;
    QAction *actionHide_History;
    QAction *action_showHideDllList;
    QAction *actionShowHideProcessList;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_dllPath;
    QCheckBox *checkBox_Trampoline;
    QLineEdit *lineEdit_TrampolineProcessName;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit_targetProcess;
    QMenuBar *menuBar;
    QMenu *menuTool_t;
    QMenu *menu_File;
    QMenu *menu_View;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *summerInjectClass)
    {
        if (summerInjectClass->objectName().isEmpty())
            summerInjectClass->setObjectName(QString::fromUtf8("summerInjectClass"));
        summerInjectClass->resize(475, 162);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/summerInject/Resources/MainIcon.png"), QSize(), QIcon::Normal, QIcon::Off);
        summerInjectClass->setWindowIcon(icon);
        actionInject = new QAction(summerInjectClass);
        actionInject->setObjectName(QString::fromUtf8("actionInject"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/summerInject/Resources/flashget.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionInject->setIcon(icon1);
        action_OpenFile = new QAction(summerInjectClass);
        action_OpenFile->setObjectName(QString::fromUtf8("action_OpenFile"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/summerInject/Resources/openfile.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_OpenFile->setIcon(icon2);
        actionHide_History = new QAction(summerInjectClass);
        actionHide_History->setObjectName(QString::fromUtf8("actionHide_History"));
        actionHide_History->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/summerInject/Resources/Impress.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHide_History->setIcon(icon3);
        action_showHideDllList = new QAction(summerInjectClass);
        action_showHideDllList->setObjectName(QString::fromUtf8("action_showHideDllList"));
        action_showHideDllList->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/summerInject/Resources/showhideDllList.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_showHideDllList->setIcon(icon4);
        actionShowHideProcessList = new QAction(summerInjectClass);
        actionShowHideProcessList->setObjectName(QString::fromUtf8("actionShowHideProcessList"));
        actionShowHideProcessList->setCheckable(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/summerInject/Resources/uTorrent.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowHideProcessList->setIcon(icon5);
        centralWidget = new QWidget(summerInjectClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        lineEdit_dllPath = new QLineEdit(centralWidget);
        lineEdit_dllPath->setObjectName(QString::fromUtf8("lineEdit_dllPath"));

        horizontalLayout_2->addWidget(lineEdit_dllPath);

        checkBox_Trampoline = new QCheckBox(centralWidget);
        checkBox_Trampoline->setObjectName(QString::fromUtf8("checkBox_Trampoline"));

        horizontalLayout_2->addWidget(checkBox_Trampoline);

        lineEdit_TrampolineProcessName = new QLineEdit(centralWidget);
        lineEdit_TrampolineProcessName->setObjectName(QString::fromUtf8("lineEdit_TrampolineProcessName"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit_TrampolineProcessName->sizePolicy().hasHeightForWidth());
        lineEdit_TrampolineProcessName->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(lineEdit_TrampolineProcessName);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit_targetProcess = new QLineEdit(centralWidget);
        lineEdit_targetProcess->setObjectName(QString::fromUtf8("lineEdit_targetProcess"));

        horizontalLayout->addWidget(lineEdit_targetProcess);


        verticalLayout->addLayout(horizontalLayout);

        summerInjectClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(summerInjectClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 475, 17));
        menuTool_t = new QMenu(menuBar);
        menuTool_t->setObjectName(QString::fromUtf8("menuTool_t"));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_View = new QMenu(menuBar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        summerInjectClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(summerInjectClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setCursor(QCursor(Qt::CrossCursor));
        mainToolBar->setIconSize(QSize(24, 24));
        summerInjectClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(summerInjectClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        summerInjectClass->setStatusBar(statusBar);

        menuBar->addAction(menuTool_t->menuAction());
        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_View->menuAction());
        menuTool_t->addAction(actionInject);
        menu_File->addAction(action_OpenFile);
        menu_View->addAction(actionHide_History);
        menu_View->addAction(action_showHideDllList);
        menu_View->addAction(actionShowHideProcessList);
        mainToolBar->addAction(actionInject);
        mainToolBar->addAction(action_OpenFile);
        mainToolBar->addAction(actionHide_History);
        mainToolBar->addAction(action_showHideDllList);
        mainToolBar->addAction(actionShowHideProcessList);

        retranslateUi(summerInjectClass);

        QMetaObject::connectSlotsByName(summerInjectClass);
    } // setupUi

    void retranslateUi(QMainWindow *summerInjectClass)
    {
        summerInjectClass->setWindowTitle(QApplication::translate("summerInjectClass", "summerInject", 0, QApplication::UnicodeUTF8));
        actionInject->setText(QApplication::translate("summerInjectClass", "&Inject", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionInject->setToolTip(QApplication::translate("summerInjectClass", "\346\263\250\345\205\245", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        actionInject->setWhatsThis(QApplication::translate("summerInjectClass", "\346\263\250\345\205\245\346\214\211\351\222\256", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        action_OpenFile->setText(QApplication::translate("summerInjectClass", "&Open", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_OpenFile->setToolTip(QApplication::translate("summerInjectClass", "\346\211\223\345\274\200dll\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        action_OpenFile->setWhatsThis(QApplication::translate("summerInjectClass", "\346\211\223\345\274\200dll\346\226\207\344\273\266", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        actionHide_History->setText(QApplication::translate("summerInjectClass", "hide&History", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHide_History->setToolTip(QApplication::translate("summerInjectClass", "\346\230\276\347\244\272/\351\232\220\350\227\217DLL\345\216\206\345\217\262\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_showHideDllList->setText(QApplication::translate("summerInjectClass", "showHideDllList", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_showHideDllList->setToolTip(QApplication::translate("summerInjectClass", "\346\230\276\347\244\272/\351\232\220\350\227\217\350\277\233\347\250\213\345\206\205DLL\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowHideProcessList->setText(QApplication::translate("summerInjectClass", "showHideProcessList", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowHideProcessList->setToolTip(QApplication::translate("summerInjectClass", "\346\230\276\347\244\272/\351\232\220\350\227\217\350\277\233\347\250\213\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("summerInjectClass", "dll", 0, QApplication::UnicodeUTF8));
        checkBox_Trampoline->setText(QApplication::translate("summerInjectClass", "\344\275\277\347\224\250\350\267\263\346\235\277", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("summerInjectClass", "target process", 0, QApplication::UnicodeUTF8));
        menuTool_t->setTitle(QApplication::translate("summerInjectClass", "&Tool", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("summerInjectClass", "&File", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("summerInjectClass", "&View", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class summerInjectClass: public Ui_summerInjectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUMMERINJECT_H
