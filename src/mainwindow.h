/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


/*
 * The main window.
 *
 * Only a single instance will be created in main().
 */
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

protected:
	void keyPressEvent(QKeyEvent *event);

private slots:
	void tabCloseRequested(int index);
	void readerActivated(uint id);
};

#endif
