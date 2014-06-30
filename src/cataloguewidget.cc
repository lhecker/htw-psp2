/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "cataloguewidget.h"

#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QVBoxLayout>

#include "basicvalidatingitemdelegate.h"


CatalogueWidget::CatalogueWidget(QWidget *parent) : QWidget(parent) {
	QVBoxLayout *layout = new QVBoxLayout();

	{
		QHBoxLayout *toolLayout = new QHBoxLayout();

		{
			QPushButton *addRowButton = new QPushButton(tr("Add Entry"), this);

			connect(addRowButton, SIGNAL(clicked()), this, SLOT(addRow()));

			toolLayout->addWidget(addRowButton);
		}

		{
			QLineEdit *searchInput = new QLineEdit(this);
			searchInput->setPlaceholderText(tr("Search..."));

			connect(searchInput, SIGNAL(textChanged(QString)), this, SLOT(searchChanged(QString)));

			toolLayout->addWidget(searchInput);
			this->searchInput = searchInput;
		}

		layout->addLayout(toolLayout);
	}


	QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this);
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setTable("catalogue");
	model->setRelation(1, QSqlRelation("types", "id", "name"));
	model->select();

	model->setHeaderData(1, Qt::Horizontal, tr("Type"));
	model->setHeaderData(2, Qt::Horizontal, tr("Year"));
	model->setHeaderData(3, Qt::Horizontal, tr("Title"));
	model->setHeaderData(4, Qt::Horizontal, tr("Author"));

	QTableView *tableView = new QTableView(this);
	tableView->setModel(model);
	tableView->setItemDelegate(new BasicValidatingItemDelegate(this));
	tableView->hideColumn(0);
	tableView->hideColumn(5);
	tableView->hideColumn(6);
	tableView->verticalHeader()->setHidden(true);
	tableView->setHorizontalScrollMode(QTableView::ScrollPerPixel);
	layout->addWidget(tableView);


	this->setLayout(layout);

	this->model = model;
	this->tableView = tableView;
}

void CatalogueWidget::keyPressEvent(QKeyEvent *event) {
	if (this->tableView->hasFocus() && event->matches(QKeySequence::Delete)) {
		event->accept();

		const QList<QModelIndex> indexes = this->tableView->selectionModel()->selectedIndexes();
		const auto button = QMessageBox::question(
			this,
			tr("Confirmation needed"),
			tr("Are you sure you wan't to delete %1 row(s)?").arg(indexes.size()),
			QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No
		);

		if (button == QMessageBox::Yes) {
			this->model->setEditStrategy(QSqlTableModel::OnManualSubmit);

			for (const QModelIndex &index : indexes) {
				this->model->removeRow(index.row());
			}

			if (!this->model->submitAll()) {
				QMessageBox::warning(
					this,
					tr("Error"),
					tr("Failed to remove the row(s). Reason:\n%s").arg(this->model->lastError().text())
				);
			}

			this->model->setEditStrategy(QSqlTableModel::OnFieldChange);
		}
	}
}

void CatalogueWidget::addRow() {
	/*
	 * If an empty/dirty row already exists at the passed row nothing will be done.
	 * --> No additional checks needed.
	 */
	this->model->insertRow(0);

	QModelIndex index = this->model->index(0, 1);
	this->tableView->setCurrentIndex(index);
	this->tableView->edit(index);
}

void CatalogueWidget::searchChanged(const QString &text) {
	QString value = text.simplified();
	QString filter("");

	if (!value.isEmpty()) {
		bool isNumeric;
		value.toUInt(&isNumeric);

		if (isNumeric) {
			filter = "catalogue.id=" % value;
		} else {
			value.replace('\'', "''");
			filter = "title LIKE '%" % value % "%' OR author LIKE '%" % value % "%'";
		}
	}

	if (this->model->filter().compare(filter) != 0) {
		this->model->setFilter(filter);
	}
}
