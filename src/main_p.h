/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef MAIN_P_H
#define MAIN_P_H

/*
 * Opens the default QSqlDatabase as a SQLite database
 * in <programname>.sqlite in the temp folder.
 */
bool open_database(bool removeIfExists = false);

/*
 * Checks if the database is missing tables and
 * creates them anew with demo data if need.
 */
bool update_database_if_needed();

#endif
