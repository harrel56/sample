#include "mycalendar.h"
#include <qdebug.h>
#include <qpainter.h>

MyCalendar::MyCalendar(QWidget *parent)
	: QCalendarWidget(parent)
{

}

void MyCalendar::repaintCells()
{
	updateCells();
}

void MyCalendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
	//Call base class method to draw all the dates
	QCalendarWidget::paintCell(painter, rect, date);
	//Draw blue ellipse on current date
	if (date == QDate::currentDate())
	{
		painter->setBrush(QColor(94, 132, 240, 125));
		painter->setPen(Qt::NoPen);
		painter->drawEllipse(rect.center(), rect.width() / 2 - 5, rect.height() / 2 - 5);
	}
	//Draw red marker on dates with planned events
	if (manager->data.contains(date))
	{
		painter->setBrush(Qt::NoBrush);
		painter->setPen(Qt::red);
		painter->drawEllipse(rect.center(), rect.width() / 2 - 5, rect.height() / 2 - 5);
	}
}