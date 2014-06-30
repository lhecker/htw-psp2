/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "mainwindow.h"

#include <QKeyEvent>
#include <QSqlQuery>

#include "cataloguewidget.h"
#include "issuewidget.h"
#include "mainwindow_p.h"
#include "readerswidget.h"


#define STATIC_TABS 2


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	MainWindowTabWidget *centralWidget = new MainWindowTabWidget(this);
	centralWidget->setStyleSheet("QTabWidget::pane{border:0}");
	centralWidget->setTabsClosable(true);

	connect(centralWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));

	{
		CatalogueWidget *widget = new CatalogueWidget(centralWidget);
		int index = centralWidget->addTab(widget, tr("Catalogue"));
		centralWidget->hideTabButton(index);
	}

	{
		ReadersWidget *widget = new ReadersWidget(centralWidget);
		int index = centralWidget->addTab(widget, tr("Readers"));
		centralWidget->hideTabButton(index);

		connect(widget, SIGNAL(readerActivated(uint)), this, SLOT(readerActivated(uint)));
	}

	this->setCentralWidget(centralWidget);
	this->setMinimumSize(640, 480);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	MainWindowTabWidget *centralWidget = qobject_cast<MainWindowTabWidget *>(this->centralWidget());
	int index = centralWidget->currentIndex();

	if (event->matches(QKeySequence::Close) && index >= STATIC_TABS) {
		event->accept();
		this->tabCloseRequested(index);
	}
}

void MainWindow::tabCloseRequested(int index) {
	QTabWidget *centralWidget = qobject_cast<QTabWidget*>(this->centralWidget());

	// tab 0 and 1 are static tabs -- see constructor above
	if (index >= STATIC_TABS) {
		centralWidget->removeTab(index);
	}
}

void MainWindow::readerActivated(uint id) {
	MainWindowTabWidget *centralWidget = qobject_cast<MainWindowTabWidget *>(this->centralWidget());

	QSqlQuery query;
	query.prepare("SELECT surname || ', ' || forename FROM readers WHERE id = ?");
	query.addBindValue(id);

	if (query.exec() && query.first()) {
		IssueWidget *widget = new IssueWidget(id, centralWidget);
		int index = centralWidget->addTab(widget, query.value(0).toString());
		centralWidget->setCurrentIndex(index);
	}
}
