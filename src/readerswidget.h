/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef READERSWIDGET_H
#define READERSWIDGET_H

#include <QWidget>


class QLineEdit;
class QModelIndex;
class QSqlTableModel;
class QTableView;

/*
 * The widget for managing readers.
 *
 * Only a single instance will be created in MainWindow.
 */
class ReadersWidget : public QWidget {
	Q_OBJECT

public:
	explicit ReadersWidget(QWidget *parent = 0);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	QLineEdit *searchInput;
	QSqlTableModel *model;
	QTableView *tableView;

signals:
	void readerActivated(uint id);

private slots:
	void tableDoubleClicked(const QModelIndex &index);
	void addRow();
	void searchChanged(const QString &text);
};

#endif
