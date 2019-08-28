#include "summerinject.h"
#include "ProcessTable.h"
#include "InjectHistory.h"
#include "summerInjectAppSetting.h"
#include "qinjecthistorywidget.h"

#include "qevent.h"
#include <QUrl>
#include <QMessageBox>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QSplitter>

#include "util/StringList.h"
#include "util/StringEx.h"
#include "util/File.h"
#include "util/Path.h"

#include "include/XTrack.h"
#include "include/XInject.h"

#include <psapi.h>

#include <string>
using std::wstring;
using std::string;

using namespace XYSLib;

XInject g_xInject;

summerInject::summerInject(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	setupView();
	makeConnections();
}

summerInject::~summerInject()
{
}

void summerInject::CheckUseTrampolineChanged( int state )
{
	//Qt::Unchecked 0 The item is unchecked. 
	//	Qt::PartiallyChecked 1 The item is partially checked. Items in hierarchical models may be partially checked if some, but not all, of their children are checked. 
	//	Qt::Checked 2 The item is checked. 
	if (state == Qt::Unchecked)
	{
		ui.lineEdit_TrampolineProcessName->setEnabled(false);
		summerInjectAppSetting::getInstance().setAppSetting("UseTrampolineInject","no");
	}else{
		ui.lineEdit_TrampolineProcessName->setEnabled(true);
		summerInjectAppSetting::getInstance().setAppSetting("UseTrampolineInject","yes");
	}
}

void summerInject::dragEnterEvent( QDragEnterEvent* event )
{
	event->acceptProposedAction();
}

void summerInject::dropEvent( QDropEvent* event )
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
	{
		return;
	}

	QString fileName = urls.first().toLocalFile();
	if (fileName.isEmpty())
	{
		return;
	}

	ui.lineEdit_dllPath->setText(fileName);
}

void summerInject::fillHistoryWidget()
{
	listWidget_history->clear();
	StringList strlist = InjectHistory::getInstance().getHistory();
	for (StringList::reverse_iterator it = strlist.rbegin(); it != strlist.rend(); it ++)
	{
		if (it == strlist.rbegin())
		{
			ui.lineEdit_dllPath->setText(QString::fromLocal8Bit(it->c_str()));
		}
		listWidget_history->addItem(QString::fromLocal8Bit(it->c_str()));
	}
}

void summerInject::historyWidgetItemDbClicked( QListWidgetItem * item )
{
	ui.lineEdit_dllPath->setText(item->text());
}

void summerInject::inject()
{
	QString strText = ui.lineEdit_dllPath->text();
	if (!strText.isEmpty())
	{
		InjectHistory::getInstance().add(strText.toLocal8Bit().data());
		fillHistoryWidget();
	}

	QModelIndexList selectList = _processesTable->selectionModel()->selectedRows(0);
	if (selectList.size() == 0)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("必须选择一个目标进程"),QMessageBox::Ok);
		return;
	}

	QModelIndex selectedModelIndex = selectList[0];
	string strLastInjectProcessName = string(selectedModelIndex.data().toString().toLocal8Bit().data());
	XTRACE << "try to inject :" << strLastInjectProcessName.c_str() << dbg;
	summerInjectAppSetting::getInstance().setAppSetting("LastInjectProcessName",strLastInjectProcessName);


	QModelIndexList selectList2 = _processesTable->selectionModel()->selectedRows(1);

	unsigned int targetPID = 0;
	wchar_t szDllPath[MAX_PATH] = {0};

	if (selectList2.size() == 0){
		return;
	}else{
		QModelIndex selectedModelIndex = selectList2[0];
		XTRACE << selectedModelIndex.data().toString().toLocal8Bit().data() << dbg;

		QString strPid = selectedModelIndex.data().toString();
		targetPID = strPid.toUInt();
		if (targetPID == 0)
		{
			return;
		}
		else{
			MultiByteToWideChar(CP_ACP,0,ui.lineEdit_dllPath->text().toLocal8Bit(),-1,szDllPath,_countof(szDllPath));
		}
	}

	if (summerInjectAppSetting::getInstance().getAppSetting("UseTrampolineInject") == "yes")
	{
		TrampolineInject(targetPID,wstring(szDllPath));
	}
	else
	{
		DirectInject(targetPID,wstring(szDllPath));
	}

	refreshDllList(targetPID);
}

void summerInject::onclose()
{
	InjectHistory::getInstance().save();
	summerInjectAppSetting::getInstance().save();
}

void summerInject::closeEvent( QCloseEvent *event )
{
	onclose();
}

void summerInject::resizeEvent( QResizeEvent * event )
{
	QSize cur_size = event->size();
	summerInjectAppSetting::getInstance().setAppSetting("WindowWidth",String::fromNumber(cur_size.width()));
	summerInjectAppSetting::getInstance().setAppSetting("WindowHigh",String::fromNumber(cur_size.height()));
}

void summerInject::TrampolineProcessChanged( const QString& text )
{
	//TrampolineProcessName
	summerInjectAppSetting::getInstance().setAppSetting("TrampolineProcessName",text.toStdString());
}

void summerInject::openFile()
{
	string strLastOpenDir = summerInjectAppSetting::getInstance().getAppSetting("LastOpenDir");
	if (strLastOpenDir == "")
	{
		strLastOpenDir = ".";
	}
	
	string strOpenFileFilter = summerInjectAppSetting::getInstance().getAppSetting("OpenFileFilter");
	if (strOpenFileFilter == "")
	{
		strOpenFileFilter = "Dll Files(*.dll *.exe *.usc)";
	}

	QString path = QFileDialog::getOpenFileName(this, 
												tr("Open File"), 
												QString::fromLocal8Bit(strLastOpenDir.c_str()), 
												QString::fromLocal8Bit(strOpenFileFilter.c_str())); 

	if(path.length() == 0) { 
		QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files.")); 
	} else { 
		File dllfile(path.toLocal8Bit().data());
		summerInjectAppSetting::getInstance().setAppSetting("LastOpenDir",dllfile.getPath());
		ui.lineEdit_dllPath->setText(path);
	} 
}

bool summerInject::TrampolineInject(unsigned int targetPID,const wstring& dllPath)
{
	unsigned int trampolineID = g_xInject.GetProcessIDByName(ui.lineEdit_TrampolineProcessName->text().toStdWString());
	if (trampolineID == 0)
	{
		QMessageBox::information(this,QString::fromLocal8Bit("错误"),QString::fromLocal8Bit("无法找到跳板进程"),QMessageBox::Ok);
		return false;
	}
	XTRACE<<"Use TrampolineInject! target " << targetPID << "trampolineID " << trampolineID <<"dllPath " << dllPath.c_str()<< dbg; 
	return g_xInject.TrampolineInject(targetPID,trampolineID,dllPath.c_str());
}

bool summerInject::DirectInject(unsigned int targetPID,const wstring& dllPath)
{
	XTRACE<<"Use DirectInject!" << dbg;
	return g_xInject.InjectDll(targetPID,dllPath);
}

void summerInject::refreshDllList( unsigned int PID )
{
	listView_dllList->clear();
 	HMODULE hMods[1024];
 	HANDLE hProcess;
 	DWORD cbNeeded;
 	unsigned int i;
	XTRACE << "Process ID:" << PID << dbg;
 	// Get a list of all the modules in this process.
 	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE, PID );
 	if (NULL == hProcess)
 		return;
 	if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
 	{
		//转化路径中的'/'为'\'
		string stdstrLineEditDllPath = File::convertPathSeparators(ui.lineEdit_dllPath->text().toLocal8Bit().data());
		QString qsLineEditDllPath = QString::fromLocal8Bit(stdstrLineEditDllPath.c_str());
		QListWidgetItem* itemSelect = NULL;

 		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
 		{
 			char szModName[MAX_PATH];
 			// Get the full path to the module's file.
 			if ( GetModuleFileNameExA(hProcess, hMods[i], szModName,sizeof(szModName)))
 			{
				QString strDllFullPath = QString::fromLocal8Bit(szModName);
				QListWidgetItem *aitem = NULL;

				//XTRACE << strDllFullPath.toLocal8Bit().data() << "----" <<stdstrLineEditDllPath.c_str() << dbg;

				if (strDllFullPath != qsLineEditDllPath)
				{
					aitem = new QListWidgetItem(strDllFullPath);
				}
				else{
					XTRACE << "---fucker---" << dbg;
					aitem = new QListWidgetItem(QIcon(":/summerInject/Resources/flashget.png"),strDllFullPath);
					aitem->setBackgroundColor(QColor(0,255,0));
					itemSelect = aitem;
				}
				
				listView_dllList->addItem(aitem);
 				//_tprintf(TEXT("\t%s (0x%08X)\n"),szModName, hMods[i]);
 			}
 		}
		if (itemSelect)
		{
			listView_dllList->setItemSelected(itemSelect,true);
			listView_dllList->scrollToItem(itemSelect);
		}
		
 	}
 	CloseHandle( hProcess );
}

void summerInject::OnActionTerminateProcess()
{
	QModelIndexList selectList = _processesTable->selectionModel()->selectedRows(1);
	unsigned int targetPID = 0;
	
	if (selectList.size() == 0)
	{
		return;
	}else{
		QModelIndex selectModelIndex = selectList[0];
		QString strPid = selectModelIndex.data().toString();
		targetPID = strPid.toUInt();
		if (targetPID == 0)
		{
			return;
		}
		HANDLE hProcess = OpenProcess( PROCESS_TERMINATE,FALSE, targetPID );
		if (hProcess == NULL)
		{
			QMessageBox::information(this,tr("error"),tr("open process faied!"),QMessageBox::Ok);
			return;
		}

		if (!TerminateProcess(hProcess,1))
		{
			QMessageBox::information(this,tr("error"),tr("terminate process faied!"),QMessageBox::Ok);
			return;
		}
		_processesTable->refresh();
	}
}

void summerInject::show_contextmenu( const QPoint& pt)
{
	if (_tableMenu)
	{	
		_tableMenu->exec(QCursor::pos());
		_processesTable->refresh();
	}
}

void summerInject::OnActionRefreshProcessList()
{
	_processesTable->refresh();
}

void summerInject::showOrHideDllList(bool showOrHide)
{
	XTRACE<<"show or hide dll list" << showOrHide << dbg;
	if (showOrHide)
	{
	//ui.pushButton_showHideDllList->setText(QString::fromLocal8Bit("隐藏进程dll列表"));
	listView_dllList->show();
	resize(size().width(),size().height() + 200);
	summerInjectAppSetting::getInstance().setAppSetting("DllListShow","yes");
	}else{
		//ui.pushButton_showHideDllList->setText(QString::fromLocal8Bit("显示进程dll列表"));
		QSize lsize = listView_dllList->size();
		listView_dllList->hide();
		if (_processesTable->isHidden() && listWidget_history->isHidden())
		{
			resize(size().width(),minimumSize().height());
		}
		else
			resize(size().width(),size().height() - lsize.height());
		summerInjectAppSetting::getInstance().setAppSetting("DllListShow","no");
	}
}


void summerInject::showOrHideProcessList( bool showOrHide )
{
	QSize TableSize = _processesTable->size();
	if (showOrHide)
	{
		//show
		if (listWidget_history->isHidden())
		{
			resize(size().width(), size().height()+250);
		}else{
			resize(size().width()+listWidget_history->size().width(), size().height());
		}
		_processesTable->show();
		summerInjectAppSetting::getInstance().setAppSetting("ProcessListShow","yes");
	}else{
		//hide
		_processesTable->hide();
		if (listWidget_history->isHidden())
		{
			resize(size().width(), size().height()-TableSize.height());
		}else{
			resize(size().width()-TableSize.width(), size().height());
		}
		summerInjectAppSetting::getInstance().setAppSetting("ProcessListShow","no");
	}
}

void summerInject::ShowHistoryList( bool showOrHide )
{
	QSize ListSize = listWidget_history->size();
	if (showOrHide)
	{
		//show
		if (_processesTable->isHidden())
		{
			resize(size().width(),size().height()+200);
		}else{
			resize(size().width()+_processesTable->size().width(),size().height());
		}
		listWidget_history->show();
		summerInjectAppSetting::getInstance().setAppSetting("HistoryListShow","yes");
	}
	else{
		//hide
		if (_processesTable->isHidden())
		{
			if (listView_dllList->isHidden())
			{
				resize(size().width(),minimumSize().height());
			}
			else
				resize(size().width(),size().height()-ListSize.height());
		}else{
			resize(size().width()-ListSize.width(),size().height());
		}
		listWidget_history->hide();
		
		summerInjectAppSetting::getInstance().setAppSetting("HistoryListShow","no");
	}
	
}

void summerInject::setupView()
{
	splitter = new QSplitter(Qt::Horizontal);
	ui.verticalLayout->addWidget(splitter,1);

	_processesTable = new ProcessTable(ui.centralWidget);
	_processesTable->setObjectName(QString::fromUtf8("_processesTable"));
	_processesTable->setToolTip(QApplication::translate("summerInjectClass", "\345\217\214\345\207\273\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));
	_processesTable->setWhatsThis(QApplication::translate("summerInjectClass", "\345\217\214\345\207\273\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));

	splitter->addWidget(_processesTable);

	listWidget_history = new QInjectHistoryWidget(ui.centralWidget);
	listWidget_history->setObjectName(QString::fromUtf8("listWidget_history"));
	listWidget_history->setToolTip(QApplication::translate("summerInjectClass", "\351\200\211\344\270\255\346\214\211del\351\224\256\345\210\240\351\231\244", 0, QApplication::UnicodeUTF8));
	splitter->addWidget(listWidget_history);


	listView_dllList = new QListWidget(ui.centralWidget);
	listView_dllList->setObjectName(QString::fromUtf8("listView_dllList"));
	listView_dllList->setMaximumSize(QSize(16777215, 200));
	ui.verticalLayout->addWidget(listView_dllList);

	ui.lineEdit_TrampolineProcessName->setAcceptDrops(false);

	setAcceptDrops(true);
	//ui.checkBox_Trampoline->setBuddy(ui.lineEdit_TrampolineProcessName);
	InjectHistory::getInstance().load();

	if (summerInjectAppSetting::getInstance().getAppSetting("UseTrampolineInject") == "yes")
	{
		ui.checkBox_Trampoline->setCheckState(Qt::Checked);
		ui.lineEdit_TrampolineProcessName->setEnabled(true);
	}else{
		ui.checkBox_Trampoline->setCheckState(Qt::Unchecked);
		ui.lineEdit_TrampolineProcessName->setEnabled(false);
	}

	string strWndTitle;
	if ((strWndTitle = summerInjectAppSetting::getInstance().getAppSetting("WindowTitle")) != "")
	{
		setWindowTitle(QString::fromLocal8Bit(strWndTitle.c_str()));
	}

	string TrampolineProcessName;
	if ((TrampolineProcessName = summerInjectAppSetting::getInstance().getAppSetting("TrampolineProcessName")) != "")
	{
		ui.lineEdit_TrampolineProcessName->setText(QString::fromStdString(TrampolineProcessName));
	}

	fillHistoryWidget();

	QString strWindowWidth = QString::fromStdString(summerInjectAppSetting::getInstance().getAppSetting("WindowWidth"));
	QString strWindowHigh = QString::fromStdString(summerInjectAppSetting::getInstance().getAppSetting("WindowHigh"));
	if (strWindowWidth.toULong() != 0 && strWindowHigh.toULong() != 0)
	{
		resize(strWindowWidth.toULong(),strWindowHigh.toULong());
	}

	XTRACE << "EnableDebugPrivilege" << dbg;

	g_xInject.EnableDebugPrivilege(true);
	if (summerInjectAppSetting::getInstance().getAppSetting("DllListShow") == "yes")
	{
		listView_dllList->show();
		ui.action_showHideDllList->setChecked(true);
		//ui.pushButton_showHideDllList->setText(QString::fromLocal8Bit("隐藏进程dll列表"));
	}else{
		listView_dllList->hide();
		ui.action_showHideDllList->setChecked(false);
		//ui.pushButton_showHideDllList->setText(QString::fromLocal8Bit("显示进程dll列表"));
	}

	if (summerInjectAppSetting::getInstance().getAppSetting("ProcessListShow") == "yes")
	{
		_processesTable->show();
		ui.actionShowHideProcessList->setChecked(true);
	}else{
		_processesTable->hide();
		ui.actionShowHideProcessList->setChecked(false);
	}

	if (summerInjectAppSetting::getInstance().getAppSetting("HistoryListShow") == "yes")
	{
		listWidget_history->show();
		ui.actionHide_History->setChecked(true);
	}else{
		listWidget_history->hide();
		ui.actionHide_History->setChecked(false);
	}

	_tableMenu = new QMenu(this); 

	_terminateProcessAction = _tableMenu->addAction(QString::fromLocal8Bit("结束进程&E"));
	_refreshProcessListAction = _tableMenu->addAction(QString::fromLocal8Bit("刷新列表&"));
	_processesTable->setContextMenuPolicy(Qt::CustomContextMenu);

	setMinimumHeight(140);
}

void summerInject::makeConnections()
{
	connect(_terminateProcessAction, SIGNAL(triggered(bool)), this, SLOT(OnActionTerminateProcess()));
	connect(_refreshProcessListAction, SIGNAL(triggered(bool)), this, SLOT(OnActionRefreshProcessList()));
	connect(ui.checkBox_Trampoline,SIGNAL(stateChanged(int)),this,SLOT(CheckUseTrampolineChanged(int)));
	connect(listWidget_history,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(historyWidgetItemDbClicked(QListWidgetItem*)));
	connect(ui.actionInject,SIGNAL(triggered()),this,SLOT(inject()));
	connect(ui.lineEdit_TrampolineProcessName,SIGNAL(textChanged(const QString&)),this,SLOT(TrampolineProcessChanged(const QString&)));
	connect(ui.action_OpenFile,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(ui.action_showHideDllList,SIGNAL(triggered(bool)),this,SLOT(showOrHideDllList(bool)));
	connect(ui.actionShowHideProcessList,SIGNAL(triggered(bool)),this,SLOT(showOrHideProcessList(bool)));
	connect(_processesTable,SIGNAL(selectProcessChanged(unsigned int)),this,SLOT(refreshDllList(unsigned int)));
	connect(_processesTable,SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(show_contextmenu(const QPoint&)));
	connect(ui.actionHide_History,SIGNAL(triggered(bool)),this,SLOT(ShowHistoryList(bool)));
}
