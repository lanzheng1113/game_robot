#include "qprocessestablemodel.h"
#include <QString>
#include <QTimer>
#include "tlhelp32.h"
#include "algorithm"
#include <functional>
#include "include/XTrack.h"
#include "util/logger.h"
//#include <boost/thread/mutex.hpp>
using namespace XYSLib;

QProcessesTableModel::QProcessesTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	getProcessInfoImm();
	_refreshTimer = new QTimer();
	QObject::connect(_refreshTimer,SIGNAL(timeout()),this,SLOT(RefreshProcessList()));
	_refreshTimer->start(1000);
}

QProcessesTableModel::~QProcessesTableModel()
{
	_refreshTimer->stop();
	delete _refreshTimer;
}

int QProcessesTableModel::rowCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	return _ProcessInfoList.size();
}

int QProcessesTableModel::columnCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
	return 3;
}

QVariant QProcessesTableModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole*/ ) const
{
	if (role == Qt::DisplayRole)
	{
		if (index.row() > _ProcessInfoList.size())
		{
			return QVariant();
		}

		ProcessInformation pi = _ProcessInfoList[index.row()];
		switch(index.column()){
			case 0:
				return QString::fromStdString(pi.ExeFileName());
			case 1:
				return QString("%1").arg(pi.PID());
			case 2:
				return QString("%1").arg(pi.ThreadCount());
			default:
				return QVariant();
		}
	}

	return QVariant();
}

QVariant QProcessesTableModel::headerData( int section, Qt::Orientation orientation,int role /*= Qt::DisplayRole*/ ) const
{
	if(role == Qt::DisplayRole)
	{
		if(orientation == Qt::Horizontal)
		{
			QList<QString> headerNames;
			headerNames<<tr("ProcessName")
				<<tr("Pid")
				<<tr("ThradCount");
			return headerNames.at(section);
		}
		else if(orientation == Qt::Vertical)
		{
			return QString("%1").arg(section);
		}
	}
	return QVariant();
}

void QProcessesTableModel::getProcessInfoImm()
{
	//定义进程信息结构 
	
	vector <ProcessInformation> piList;
	PROCESSENTRY32 pe32;  
	pe32.dwSize = sizeof(PROCESSENTRY32);  

	HANDLE hProcessShot;  
	hProcessShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //获取进程列表，最后一个参数是0  
	if (hProcessShot == INVALID_HANDLE_VALUE)  
	{  
		return;  
	}  
	// 创建系统当前进程快照  
	if (Process32First(hProcessShot,&pe32))//获取下一个进程快照  
	{  
		while (Process32Next(hProcessShot, &pe32))
		{
			ProcessInformation pi;
			pi.setPID(pe32.th32ProcessID);
			pi.setThreadCount(pe32.cntThreads);
			char aszExeFileName[MAX_PATH];
			WideCharToMultiByte(CP_ACP,0,pe32.szExeFile,-1,aszExeFileName,sizeof(aszExeFileName),NULL,NULL);
			pi.setExeFileName(string(aszExeFileName));
			piList.push_back(pi);
		}
    }
	//遍历进程快照  
	CloseHandle(hProcessShot); 

	vector<ProcessInformation> piListsort = piList;
	std::sort(piListsort.begin(),piListsort.end());

	vector<ProcessInformation> piListsavedSort = _ProcessInfoList;
	std::sort(piListsavedSort.begin(),piListsavedSort.end());

    if (piListsavedSort != piListsort){
        std::cout << "process list changed!";
		_ProcessInfoList = piList;
    }
	
	return;  
}

void QProcessesTableModel::RefreshProcessList()
{
	getProcessInfoImm();
}

bool QProcessesTableModel::insertRows(int row, int count,
							 const QModelIndex &parent /*= QModelIndex()*/)
{
	beginInsertRows(parent,row,row+count-1);
	endInsertRows();
	return true;
}

bool QProcessesTableModel::removeRows(int row, int count,
							 const QModelIndex &parent /*= QModelIndex()*/)
{
	beginRemoveRows(parent,row,row+count-1);
	endRemoveRows();
	return true;
}

void QProcessesTableModel::refresh()
{
	getProcessInfoImm();
	reset();
}

int QProcessesTableModel::getRowWithProcessName( const QString& processName )
{
	QString strUpper = processName.toUpper();
	for (int i=0; i!=rowCount(QModelIndex()); i++)
	{
		QModelIndex index = createIndex(i,0);
		QString str = data(index).toString();
		//XTRACE<< "piapiaaa::" << str.toStdString().c_str() << dbg;
		if (str.toUpper() == strUpper)
		{
			return i;
		}
	}
	return -1;	
	}

bool UDgreate(const ProcessInformation& p1,const ProcessInformation& p2){
	if (p1.ThreadCount() < p2.ThreadCount())
	{
		return true;
	}else{
		return false;
	}
}

bool UDSmall(const ProcessInformation& p1,const ProcessInformation& p2){
	return !UDgreate(p1,p2);
}

void QProcessesTableModel::sort( int column, Qt::SortOrder order /*= Qt::AscendingOrder */ )
{
	XTRACE << column << order << dbg;
	if (column == 1)
	{
		if (order == Qt::AscendingOrder)
		{
			std::sort(_ProcessInfoList.begin(),_ProcessInfoList.end());
		}else{
			std::sort(_ProcessInfoList.begin(),_ProcessInfoList.end(),std::greater<ProcessInformation>());
		}
		reset();
	}else if(column == 2){
// 		if (order == Qt::AscendingOrder)
// 		{
// 			std::sort(_ProcessInfoList.begin(),_ProcessInfoList.end(),UDgreate);
// 		}else{
// 			std::sort(_ProcessInfoList.begin(),_ProcessInfoList.end(),UDSmall);
// 		}
// 		reset();
	}
}
