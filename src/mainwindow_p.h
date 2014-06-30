/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include <QTabWidget>


/*
 * A custom QTabWidget which enables us to hide the close button.
 *
 * This subclass is created since the tabBar() accessor
 * is protected in the Qt 4 branch.
 */
class MainWindowTabWidget : public QTabWidget {
	Q_OBJECT

public:
	explicit MainWindowTabWidget(QWidget *parent = 0);

	void hideTabButton(int index);
};

#endif
