#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <qcalendarwidget.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include "ui_mainwidget.h"
#include "mycalendar.h"
#include "DBManager.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	//No destructor implemented because QT parent system will take care of all the memory deallocation

public slots:
	void onDateChanged(); //Called when selected day on calendar has changed
	void onEventItemChanged(QListWidgetItem* cur, QListWidgetItem* prev); //Called when selected event on list has changed
	void onAddEventClicked(); //On button 'Add' clicked
	void onRemoveEventClicked(); //On button 'Remove' clicked
	void onTitleEditChanged(const QString& str); //Called when title of event has changed

private:
	void closeEvent(QCloseEvent* e); //Called when closing app - saving data if required

	Ui::MainWidgetClass ui;

	DBManager manager; //Object providing access interface to database

	//All GUI elements including layouts
	QHBoxLayout* mainLayout;
	MyCalendar* calendar;

	QGroupBox* listGroup;
	QVBoxLayout* listLayout;
	QListWidget* list;
	QPushButton* addButton;
	QPushButton* removeButton;

	QVBoxLayout* detailsLayout;
	QGroupBox* titleGroup;
	QVBoxLayout* titleLayout;
	QLineEdit* titleEdit;
	QGroupBox* descriptionGroup;
	QVBoxLayout* descriptionLayout;
	QTextEdit* descriptionEdit;
};

#endif // MAINWIDGET_H
