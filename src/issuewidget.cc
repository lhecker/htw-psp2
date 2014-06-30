/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "issuewidget.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QMimeData>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableView>
#include <QtEvents>

#include "issuewidget_p.h"


static const char *kDefaultCatalogueQuery = "SELECT c.id, t.name, c.year, c.title, c.author FROM catalogue c, types t WHERE t.id = c.type AND c.issued_to IS NULL";



IssueWidget::IssueWidget(uint id, QWidget *parent) : QWidget(parent) {
	QHBoxLayout *layout = new QHBoxLayout();

	{
		QGroupBox *box = new QGroupBox(tr("issued"));
		QVBoxLayout *sublayout = new QVBoxLayout();

		{
			QSqlQuery query("SELECT c.id, c.issued_on, t.name, c.year, c.title, c.author FROM catalogue c, types t WHERE t.id = c.type AND c.issued_to = ?");
			query.addBindValue(id);
			query.exec();

			IssueReaderModel *model = new IssueReaderModel(id, this);
			model->setQuery(query);
			model->setHeaderData(1, Qt::Horizontal, tr("Issued On"));
			model->setHeaderData(2, Qt::Horizontal, tr("Type"));
			model->setHeaderData(3, Qt::Horizontal, tr("Year"));
			model->setHeaderData(4, Qt::Horizontal, tr("Title"));
			model->setHeaderData(5, Qt::Horizontal, tr("Author"));

			QTableView *tableView = new QTableView(this);
			tableView->setModel(model);
			tableView->hideColumn(0);
			tableView->verticalHeader()->setHidden(true);
			tableView->setHorizontalScrollMode(QTableView::ScrollPerPixel);
			tableView->setSelectionBehavior(QTableView::SelectRows);
			tableView->setDragDropMode(QTableView::DragDrop);
			tableView->setDragEnabled(true);
			tableView->setAcceptDrops(true);
			tableView->setDropIndicatorShown(true);

			sublayout->addWidget(tableView);

			this->issueModel = model;

			connect(model, SIGNAL(rowsDropped()), this, SLOT(rowsDropped()));
		}

		box->setLayout(sublayout);
		layout->addWidget(box);
	}

	{
		QGroupBox *box = new QGroupBox(tr("catalogue"));
		QVBoxLayout *sublayout = new QVBoxLayout();

		{
			QLineEdit *searchInput = new QLineEdit(this);
			searchInput->setPlaceholderText(tr("Search..."));
			sublayout->addWidget(searchInput);

			connect(searchInput, SIGNAL(textChanged(QString)), this, SLOT(searchChanged(QString)));
		}

		{
			IssueCatalogueModel *model = new IssueCatalogueModel(id, this);
			model->setQuery(kDefaultCatalogueQuery);
			model->setHeaderData(1, Qt::Horizontal, tr("Type"));
			model->setHeaderData(2, Qt::Horizontal, tr("Year"));
			model->setHeaderData(3, Qt::Horizontal, tr("Title"));
			model->setHeaderData(4, Qt::Horizontal, tr("Author"));

			QTableView *tableView = new QTableView(this);
			tableView->setModel(model);
			tableView->hideColumn(0);
			tableView->verticalHeader()->setHidden(true);
			tableView->setHorizontalScrollMode(QTableView::ScrollPerPixel);
			tableView->setSelectionBehavior(QTableView::SelectRows);
			tableView->setDragDropMode(QTableView::DragDrop);
			tableView->setDragEnabled(true);
			tableView->setAcceptDrops(true);
			tableView->setDropIndicatorShown(true);

			sublayout->addWidget(tableView);

			this->catalogueModel = model;
			this->searchChanged(QString());

			connect(model, SIGNAL(rowsDropped()), this, SLOT(rowsDropped()));
		}

		box->setLayout(sublayout);
		layout->addWidget(box);
	}


	this->setLayout(layout);
}

void IssueWidget::searchChanged(const QString &text) {
	QString value = text.simplified();
	QString q(kDefaultCatalogueQuery);
	bool ok = false;

	if (text.isEmpty()) {
		QSqlQuery query(q);
		ok = query.exec();
		this->catalogueModel->setQuery(query);
	} else {
		bool isNumeric;
		uint id = value.toUInt(&isNumeric);

		q.append(" AND ");

		if (isNumeric) {
			q.append("c.id = ?");

			QSqlQuery query;
			query.prepare(q);
			query.addBindValue(id);
			ok = query.exec();
			this->catalogueModel->setQuery(query);
		} else {
			q.append("c.title LIKE ? AND c.author LIKE ?");

			value = '%' % value % '%';

			QSqlQuery query;
			query.prepare(q);
			query.addBindValue(value);
			query.addBindValue(value);
			ok = query.exec();
			this->catalogueModel->setQuery(query);
		}
	}

	if (!ok) {
		QMessageBox::warning(
			this,
			tr("Error"),
			tr("Failed to filter catalogue. Reason:\n%1").arg(this->catalogueModel->lastError().text())
		);
	}
}

void IssueWidget::rowsDropped() {
	QSqlQuery q1 = this->issueModel->query();
	QSqlQuery q2 = this->catalogueModel->query();

	q1.exec();
	q2.exec();

	this->issueModel->setQuery(q1);
	this->catalogueModel->setQuery(q2);
}
