#include "qinjecthistorywidget.h"
#include "QKeyEvent"
#include "InjectHistory.h"
#include "include/Xinject.h"

QInjectHistoryWidget::QInjectHistoryWidget(QWidget *parent)
	: QListWidget(parent)
{

}

QInjectHistoryWidget::~QInjectHistoryWidget()
{

}

void QInjectHistoryWidget::keyPressEvent( QKeyEvent * event )
{
	if (event->key() == Qt::Key_Delete)
	{
		QList<QListWidgetItem*> items =  selectedItems();
		for (QList<QListWidgetItem*>::iterator it = items.begin(); it != items.end(); ++it)
		{
			QString str = (*it)->text();
			InjectHistory::getInstance().del(str.toLocal8Bit().data());
			delete *it;
		}
	}
	return QWidget::keyPressEvent(event);
}