#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QCalendarWidget>
#include "DBManager.h"

class MyCalendar : public QCalendarWidget
{
	Q_OBJECT

public:
	MyCalendar(QWidget *parent = nullptr);

	DBManager* manager; //In order to have days with events marked we need to have information about hashtable containing all data

	void repaintCells(); //Accessor for repainting method
protected:
	void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const; //Overloaded paint method, which is needed to mark dates with events
private:
	
};

#endif // MYCALENDAR_H
