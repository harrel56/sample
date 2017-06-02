#ifndef EVENTITEM_H
#define EVENTITEM_H
#include <QListWidgetItem>
#include <qdatetime.h>

class EventItem : public QListWidgetItem //Inherits from QListWidgetItem in order to be easily put in the QListWidget object
{

public:
	EventItem(const EventItem&) = delete; //Copying EventItem is not desired
	EventItem(QListWidget * parent = 0);
	EventItem(int i, QDate dt, QString t, QString d);

	//Fields representing columns
	int id;
	QDate date;
	QString title;
	QString desc;

private:

	
};

#endif // EVENTITEM_H