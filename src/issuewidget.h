/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef ISSUEWIDGET_H
#define ISSUEWIDGET_H

#include <QWidget>


class IssueCatalogueModel;
class IssueReaderModel;
class QSqlQueryModel;

/*
 * The issuing widget.
 *
 * Will be created for a specific reader with the ID passed
 * as the first argument and added as a new tab in MainWindow.
 */
class IssueWidget : public QWidget {
	Q_OBJECT

public:
	explicit IssueWidget(uint id, QWidget *parent = 0);

private:
	IssueReaderModel *issueModel;
	IssueCatalogueModel *catalogueModel;

private slots:
	void searchChanged(const QString &text);
	void rowsDropped();
};

#endif
