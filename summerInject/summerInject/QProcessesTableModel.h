#ifndef QPROCESSESTABLEMODEL_H
#define QPROCESSESTABLEMODEL_H

#include <QAbstractTableModel>
#include "ProcessInformation.h"
#include <vector>
using std::vector;
#include "windows.h"

class QTimer;
class QProcessesTableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	QProcessesTableModel(QObject *parent);
	~QProcessesTableModel();
	
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;
	virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	virtual void  sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
	void refresh();
	int getRowWithProcessName(const QString& processName);
public Q_SLOTS:
	void RefreshProcessList();
private:
	vector<ProcessInformation> _ProcessInfoList;
	void getProcessInfoImm();
	QTimer* _refreshTimer;
};

#endif // QPROCESSESTABLEMODEL_H
