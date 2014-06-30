/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "main_p.h"

#include <array>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


bool open_database(bool removeIfExists) {
	QString path = QDir::toNativeSeparators(QDir::tempPath() % "/" % QCoreApplication::applicationName() % ".sqlite");

	if (removeIfExists) {
		QFile::remove(path);
	}

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);

	if (!db.open()) {
		QMessageBox::critical(0, QCoreApplication::translate("open_database", "Error"), QCoreApplication::translate("open_database", "Could not open Database at\n\n%1").arg(path));
		return false;
	}

	return true;
}

bool update_database_if_needed() {
	struct table_definition_t {
		const char *name;
		const char *definition;
		const char *columns;
		const char *values;
	};

	const std::array<table_definition_t, 3> tableDefinitions = {{
		{
			//.name =
				"readers",
			//.definition =
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"surname VARCHAR(255) NOT NULL,"
				"forename VARCHAR(255) NOT NULL",
			//.columns =
				"surname,forename",
			//.values =
				"('Young','Danny'),"
				"('Holand','Christine'),"
				"('Gordon','Lars'),"
				"('Robitaille','Roberto'),"
				"('Papadopoulos','Maria')"
		},
		{
			//.name =
				"types",
			//.definition =
				"id INTEGER PRIMARY KEY,"
				"name VARCHAR(255) NOT NULL",
			//.columns =
				"id,name",
			//.values =
				"(0,'CD/music'),"
				"(1,'CD/movie'),"
				"(2,'DVD/music'),"
				"(3,'DVD/movie'),"
				"(4,'BD/music'),"
				"(5,'BD/movie'),"
				"(6,'book')"
		},
		{
			//.name =
				"catalogue",
			//.definition =
				"id INTEGER PRIMARY KEY AUTOINCREMENT,"
				"type INTEGER NOT NULL,"
				"year INTEGER NOT NULL,"
				"title VARCHAR(255) NOT NULL,"
				"author VARCHAR(255) NOT NULL,"
				"issued_to INTEGER,"
				"issued_on DATE",
			//.columns =
				"type,year,title,author",
			//.values =
				"(0,2013,'Flume Deluxe Edition','Flume'),"
				"(2,2006,'Völkerball','Rammstein'),"
				"(3,2014,'Der Hobbit: Smaugs Einöde','Warner Home Video'),"
				"(5,2014,'The Wolf of Wall Street','Universal Pictures'),"
				"(6,2009,'Artificial Intelligence: A Modern Approach','Stuart Russell,Peter Norvig')"
		},
	}};

	QSqlDatabase db = QSqlDatabase::database();
	QStringList tables = db.tables();

	if (tables.size() != tableDefinitions.size()) {
		QSqlQuery query;

		for (const auto &entry : tableDefinitions) {
			QString name = entry.name;

			if (!tables.contains(name)) {
				db.transaction();

				if (query.exec("CREATE TABLE " % name % " (" % entry.definition % ")")) {
					if (!entry.columns || !entry.values || query.exec("INSERT INTO " % name % " (" % entry.columns % ") VALUES " % entry.values)) {
						if (db.commit()) {
							continue;
						}
					}
				}

				db.rollback();
				QMessageBox::critical(nullptr, QCoreApplication::translate("update_database_if_needed", "Error"), QCoreApplication::translate("update_database_if_needed", "Could not create table %1! Reason:\n\n%2").arg(name).arg(query.lastError().text()));
				return false;
			}
		}
	}

	return true;
}
