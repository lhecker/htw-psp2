/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef CATALOGUEWIDGET_H
#define CATALOGUEWIDGET_H

#include <QWidget>


class QLineEdit;
class QSqlRelationalTableModel;
class QTableView;

/*
 * The widget for managing the catalogue.
 *
 * Only a single instance is created in MainWindow.
 */
class CatalogueWidget : public QWidget {
	Q_OBJECT

public:
	explicit CatalogueWidget(QWidget *parent = 0);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QLineEdit *searchInput;
	QSqlRelationalTableModel *model;
	QTableView *tableView;

private slots:
	void addRow();
	void searchChanged(const QString &text);
};

#endif
