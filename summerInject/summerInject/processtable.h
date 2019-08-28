#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QTableView>
class QProcessesTableModel;
class ProcessTable : public QTableView
{
	Q_OBJECT

public:
	ProcessTable(QWidget *parent);
	~ProcessTable();
public:
	void refresh();
Q_SIGNALS:
	void selectProcessChanged(unsigned int PID);
protected:
	virtual void  mouseDoubleClickEvent ( QMouseEvent * event );
	virtual void  selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected ) ;
private:
	QProcessesTableModel* _model;
};

#endif // PROCESSTABLE_H
