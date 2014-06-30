/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include <QApplication>

#include "mainwindow.h"
#include "main_p.h"


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	if (!open_database() || !update_database_if_needed()) {
		return 1;
	}

	MainWindow window;
	window.show();

	return app.exec();
}
