#include <qstring.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qvariant.h>
#include <qcoreapplication.h>
#include <qdebug.h>
#include <filesystem>
#include "DBManager.h"



DBManager::DBManager(QString t) : type(t), year(-1) {}


DBManager::~DBManager()
{
	clearData();
	if(db.isOpen())
		db.close();
}

bool DBManager::databaseExists()
{
	QString path = QCoreApplication::applicationDirPath() + "/data.db";
	return std::experimental::filesystem::is_regular_file(path.toStdString());
}

bool DBManager::connectToDatabase()
{
	QString path = QCoreApplication::applicationDirPath() + "/data.db";
	db = QSqlDatabase::addDatabase(type);
	db.setDatabaseName(path);
	return db.isValid();
}

void DBManager::openConnection()
{
	db.open();
}

void DBManager::initialize()
{
	//Create table 'event'
	db.exec("create table event (id integer primary key,"
		"date text, title text, description text)");
}

void DBManager::fetchData(int year)
{
	//Fetch data from db from current, previous and next year (3 years total);
	if (this->year != year)
	{
		//Clear old data before inserting new one
		clearData();
		QDate temp(year - 1, 1, 1);
		QDate endDate(year + 1, 12, 31);
		while (temp != endDate)
		{
			QSqlQuery query = db.exec("select * from event where date = '" + temp.toString(Qt::ISODate) + "'");
			QVector<EventItem*> vect;

			while (query.next())
			{
				int id = query.value("id").toInt();
				QString title = query.value("title").toString();
				QString desc = query.value("description").toString();
				EventItem* item = new EventItem(id, temp, title, desc);
				item->setText(title);

				vect.push_back(item);
			}

			if(!vect.empty())
				data.insert(temp, vect);
			temp = temp.addDays(1);
		}
		this->year = year;
	}
}

bool DBManager::insert(EventItem* record)
{
	QSqlQuery query(db);
	//Prepare with addBindValue is used here to prevent SQL injection
	//Placing NULL value in primary key slot causes it to be chosen automatically (autoincrement)
	query.prepare("insert into event (id, date, title, description) values (NULL, ?, ?, ?)");
	query.addBindValue(record->date.toString(Qt::ISODate));
	query.addBindValue(record->title);
	query.addBindValue(record->desc);

	//Because we want to avoid clearing and fetching data, we instead update only id of newly created item/record
	bool result = query.exec();
	record->id = query.lastInsertId().toInt();
	return result;
}

bool DBManager::update(EventItem* record)
{
	QSqlQuery query(db);
	//Prepare with addBindValue is used here to prevent SQL injection
	query.prepare("update event set title = ?, description = ? where id = ?");	
	query.addBindValue(record->title);
	query.addBindValue(record->desc);
	query.addBindValue(record->id);
	return query.exec();
}

void DBManager::clearData()
{
	//All EventItems have been allocated dynamically so here we delete them
	for (auto vector : data)
	{
		for (auto el : vector)
			delete el;
		vector.clear();
	}
	data.clear();
}

bool DBManager::remove(EventItem* record)
{
	QSqlQuery query(db);
	query.prepare("delete from event where id = ?");
	query.addBindValue(record->id);

	//Update hashtable
	QDate date = record->date;
	auto& vect = data[date];
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i] == record)
		{
			vect.remove(i);
			delete record;
			break;
		}
	}
	if (vect.empty())
		data.remove(date);

	return query.exec();
}
