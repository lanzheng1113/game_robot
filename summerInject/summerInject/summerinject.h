#ifndef SUMMERINJECT_H
#define SUMMERINJECT_H

#include <QtGui/QMainWindow>
#include "ui_summerinject.h"
#include <string>
using std::wstring;

class QListWidgetItem;
class QListWidget;
class ProcessTable;
class QMenu;
class QInjectHistoryWidget;
class QSplitter;
class summerInject : public QMainWindow
{
	Q_OBJECT

public:
	summerInject(QWidget *parent = 0, Qt::WFlags flags = 0);
	~summerInject();
public Q_SLOTS:
	void CheckUseTrampolineChanged(int state);
	void historyWidgetItemDbClicked( QListWidgetItem * item );
	void inject();
	void onclose();
	void TrampolineProcessChanged(const QString& text);
	void openFile();
	void showOrHideDllList(bool showOrHide);
	void showOrHideProcessList(bool showOrHide);
	void refreshDllList(unsigned int PID);
	void OnActionTerminateProcess();
	void show_contextmenu(const QPoint& pt);
	void OnActionRefreshProcessList();
	void ShowHistoryList(bool showOrHide);
protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	void closeEvent(QCloseEvent *event);
	void resizeEvent ( QResizeEvent * event );
private:
	void fillHistoryWidget();
	bool TrampolineInject(unsigned int targetPID,const wstring& dllPath);
	bool DirectInject(unsigned int targetPID,const wstring& dllPath);
	void setupView();
	void makeConnections();
	Ui::summerInjectClass ui;
	//ProcessTable* _processesTable;
	QMenu* _tableMenu;
	QAction* _terminateProcessAction;
	QAction* _refreshProcessListAction;
	ProcessTable *_processesTable;
	QInjectHistoryWidget *listWidget_history;
	QSplitter* splitter;
	QListWidget *listView_dllList;
};

#endif // SUMMERINJECT_H
