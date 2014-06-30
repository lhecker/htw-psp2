/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "mainwindow_p.h"

#include <QTabBar>


MainWindowTabWidget::MainWindowTabWidget(QWidget *parent) : QTabWidget(parent) {
}

void MainWindowTabWidget::hideTabButton(int index) {
	QWidget *widget;

	widget = this->tabBar()->tabButton(index, QTabBar::LeftSide);
	if (widget) {
		widget->resize(0, 0);
	}

	widget = this->tabBar()->tabButton(index, QTabBar::RightSide);
	if (widget) {
		widget->resize(0, 0);
	}
}
