#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <qsqldatabase.h>
#include <qsqlquerymodel.h>
#include <qhash.h>
#include <qdatetime.h>
#include <eventitem.h>

class DBManager
{
public:
	DBManager(QString t); //Takes desired type of db (QSQLITE)
	~DBManager();

	bool databaseExists(); //Check if .db file already exists
	bool connectToDatabase(); //Creates connection to database
	void openConnection(); //Opens connection to db
	void initialize(); //Creates database structure

	void fetchData(int year); //Loads data from db to hashtable 'data'

	//These 3 methods not only manage data in database but also keep hashtable updated
	bool insert(EventItem* record); //Inserts new row to db
	bool update(EventItem* record); //Updates existing row in db
	bool remove(EventItem* record); //Removes row from db

	QHash<QDate, QVector<EventItem*>> data; //Hashtable object stores data from 3 years range

private:
	void clearData(); //Clears hashtable deallocating memory

	QString type; //Type of db (in this case it is always 'QSQLITE')
	QSqlDatabase db;
	int year; //Current year for which data is stored in hashtable
};

#endif // DBMANAGER_H

