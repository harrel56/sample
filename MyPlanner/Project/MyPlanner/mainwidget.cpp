
#include <qdebug.h>
#include <qdir.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include <qsqlrecord.h>
#include "mainwidget.h"



MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent), manager("QSQLITE")
{

	//Set geometry and basic properties of main widget
	this->setMinimumWidth(800);
	this->setMinimumHeight(380);
	this->resize(800, 380);
	this->setWindowTitle("Planner");
	
	//Initialize all GUI elements
	mainLayout = new QHBoxLayout(this);
	calendar = new MyCalendar();
	listGroup = new QGroupBox(calendar->selectedDate().toString());
	listLayout = new QVBoxLayout();
	list = new QListWidget();
	addButton = new QPushButton("Add new event");
	removeButton = new QPushButton("Remove event");
	detailsLayout = new QVBoxLayout();
	titleGroup = new QGroupBox("Title");
	titleLayout = new QVBoxLayout();
	titleEdit = new QLineEdit();
	descriptionGroup = new QGroupBox("Description");
	descriptionLayout = new QVBoxLayout();
	descriptionEdit = new QTextEdit();

	//Put everything into scalable layouts
	listLayout->addWidget(list);
	listLayout->addWidget(addButton);
	listLayout->addWidget(removeButton);
	listGroup->setLayout(listLayout);

	titleLayout->addWidget(titleEdit);
	titleGroup->setLayout(titleLayout);

	descriptionLayout->addWidget(descriptionEdit);
	descriptionGroup->setLayout(descriptionLayout);

	detailsLayout->addWidget(titleGroup);
	detailsLayout->addWidget(descriptionGroup);

	mainLayout->addWidget(calendar, 6);
	mainLayout->addWidget(listGroup, 3);
	mainLayout->addLayout(detailsLayout, 4);
	
	//Manage database - also create it from scratch if needed
	bool exists = manager.databaseExists();
	if (!manager.connectToDatabase())
	{
		QMessageBox::critical(this, "Planner database error", "Database could not be created properly. Check if SQLite drivers are installed.");
		QTimer::singleShot(500, this, SLOT(close()));
	}
	else
	{
		manager.openConnection();
		if (!exists)
			manager.initialize();
	}

	titleEdit->setDisabled(true);
	descriptionEdit->setDisabled(true);
	removeButton->setDisabled(true);
	calendar->manager = &manager;
	//Connect signals and slots - event handling
	connect(calendar, SIGNAL(selectionChanged()), this, SLOT(onDateChanged()));
	connect(calendar, &MyCalendar::currentPageChanged, [&](int y) { manager.fetchData(y); });
	connect(titleEdit, SIGNAL(textChanged(QString)), this, SLOT(onTitleEditChanged(const QString&)));
	connect(list, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(onEventItemChanged(QListWidgetItem*, QListWidgetItem*)));
	connect(addButton, SIGNAL(clicked(bool)), this, SLOT(onAddEventClicked()));
	connect(removeButton, SIGNAL(clicked(bool)), this, SLOT(onRemoveEventClicked()));

	//Emit signal from calendar to repaint it with new data from db manager
	emit calendar->selectionChanged();
}

void MainWidget::onDateChanged()
{
	const QDate& selectedDate = calendar->selectedDate();
	listGroup->setTitle(selectedDate.toString("dddd d MMMM yyyy"));
	//Clearing list - not using method 'clear' because it automatically deallocates memory
	while (list->count() > 0)
	{
		list->takeItem(0);
	}
	manager.fetchData(selectedDate.year());
	//Populate list
	if (manager.data.contains(selectedDate))
	{
		for (auto item : manager.data[selectedDate])
			list->addItem(item);
	}
	//Select first item
	if (list->count())
		list->setCurrentRow(0);
}

void MainWidget::onEventItemChanged(QListWidgetItem* cur, QListWidgetItem* prev)
{
	//If something was previously selected - check if updating data is needed
	if (prev != nullptr)
	{
		EventItem* prevItem = dynamic_cast<EventItem*>(prev);
		if (prevItem && (prevItem->title != titleEdit->text() || prevItem->desc != descriptionEdit->toPlainText()))
		{
			prevItem->title = titleEdit->text();
			prevItem->desc = descriptionEdit->toPlainText();
			manager.update(prevItem);
		}
	}
	//If selection changed to nothing - clear the data fields
	if (cur == nullptr)
	{
		titleEdit->clear();
		descriptionEdit->clear();
		titleEdit->setDisabled(true);
		descriptionEdit->setDisabled(true);
		removeButton->setDisabled(true);
	}
	//If selection changed to existing item - update data fields
	else
	{
		EventItem* curItem = dynamic_cast<EventItem*>(cur);
		if (curItem)
		{
			titleEdit->setText(curItem->title);
			descriptionEdit->setText(curItem->desc);
			titleEdit->setDisabled(false);
			descriptionEdit->setDisabled(false);
			removeButton->setDisabled(false);
		}
	}
}

void MainWidget::onAddEventClicked()
{
	const QDate& selectedDate = calendar->selectedDate();
	QString title("New event");
	QString desc("Description...");
	EventItem* newItem = new EventItem(-1, selectedDate, title, desc);
	newItem->setText(title);
	if (!manager.data.contains(selectedDate))
		manager.data.insert(selectedDate, QVector<EventItem*>{ newItem });
	else
		manager.data[selectedDate].push_back(newItem);

	//Add newly created item to db and list and select it
	manager.insert(newItem);
	list->addItem(newItem);
	list->setCurrentItem(newItem);

	calendar->repaintCells();
}

void MainWidget::onRemoveEventClicked()
{
	EventItem* curItem = dynamic_cast<EventItem*>(list->currentItem());
	if (curItem)
	{
		//Remove from list and from db
		list->takeItem(list->currentRow());
		manager.remove(curItem);
	}
	//Select other item on list if exists
	if (list->count())
		list->setCurrentRow(0);

	calendar->repaintCells();
}

void MainWidget::onTitleEditChanged(const QString& str)
{
	EventItem* curItem = dynamic_cast<EventItem*>(list->currentItem());
	if (curItem)
		curItem->setText(str);
}

void MainWidget::closeEvent(QCloseEvent* e)
{
	//Before closing check if there's unsaved data
	EventItem* curItem = dynamic_cast<EventItem*>(list->currentItem());
	if (curItem && (curItem->title != titleEdit->text() || curItem->desc != descriptionEdit->toPlainText()))
	{
		curItem->title = titleEdit->text();
		curItem->desc = descriptionEdit->toPlainText();
		manager.update(curItem);
	}
}



