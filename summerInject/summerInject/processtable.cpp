#include "processtable.h"
#include "QProcessesTableModel.h"
#include "summerInjectAppSetting.h"
#include "include/XTrack.h"
#include <QFontMetrics>
#include <QHeaderView>
#include <string>
using std::string;
using namespace  XYSLib;
ProcessTable::ProcessTable(QWidget *parent)
	: QTableView(parent)
{
	_model = new QProcessesTableModel(this);
	setModel(_model);
	setAcceptDrops(true);
	setAlternatingRowColors(true);
	setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中
	setSelectionMode(QAbstractItemView::SingleSelection); //只允许选中单行

	QFontMetrics fontMetrics(font());
	//horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	verticalHeader()->setResizeMode(QHeaderView::Fixed);
	verticalHeader()->setDefaultSectionSize(fontMetrics.height()+6);

	setSortingEnabled(true);
	//this->sortByColumn(0);
}

ProcessTable::~ProcessTable()
{
	delete _model;
}

void ProcessTable::refresh()
{
	_model->refresh();
	string strLastInjectProcessName = summerInjectAppSetting::getInstance().getAppSetting("LastInjectProcessName");
	int iLst = _model->getRowWithProcessName(QString::fromStdString(strLastInjectProcessName));
	if ( iLst != -1)
	{
		selectRow(iLst);
	}
}

void ProcessTable::mouseDoubleClickEvent( QMouseEvent * event )
{
	refresh();
	//summerInjectAppSetting::getInstance().setAppSetting("LastInjectProcessName",strLastInjectProcessName);
	return QTableView::mouseDoubleClickEvent(event);
}

void ProcessTable::selectionChanged( const QItemSelection & selected, const QItemSelection & deselected )
{
	QModelIndexList IndexSels = selected.indexes();
	unsigned int PIDSelected = IndexSels[1].data().toUInt();
	XTRACE << "fucker " << PIDSelected << dbg;
    Q_EMIT selectProcessChanged(PIDSelected);
	return QTableView::selectionChanged(selected,deselected);
}
