#ifndef QINJECTHISTORYWIDGET_H
#define QINJECTHISTORYWIDGET_H

#include <QListWidget>

class QInjectHistoryWidget : public QListWidget
{
	Q_OBJECT

public:
	QInjectHistoryWidget(QWidget *parent);
	~QInjectHistoryWidget();
protected:
	void  keyPressEvent ( QKeyEvent * event );
private:
	
};

#endif // QINJECTHISTORYWIDGET_H
