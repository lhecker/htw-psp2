/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "readerswidget.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlError>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>

#include "basicvalidatingitemdelegate.h"


ReadersWidget::ReadersWidget(QWidget *parent) : QWidget(parent) {
	QVBoxLayout *layout = new QVBoxLayout();

	{
		QHBoxLayout *toolLayout = new QHBoxLayout();

		{
			QPushButton *addRowButton = new QPushButton(tr("Add Reader"), this);

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

	QSqlTableModel *model = new QSqlTableModel(this);
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setTable("readers");
	model->setHeaderData(0, Qt::Horizontal, tr("ID"));
	model->setHeaderData(1, Qt::Horizontal, tr("Surname"));
	model->setHeaderData(2, Qt::Horizontal, tr("Forename"));
	model->select();

	QTableView *tableView = new QTableView(this);
	tableView->setModel(model);
	tableView->setItemDelegate(new BasicValidatingItemDelegate(this));
	tableView->verticalHeader()->setHidden(true);
	tableView->setHorizontalScrollMode(QTableView::ScrollPerPixel);

	connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tableDoubleClicked(QModelIndex)), Qt::QueuedConnection);

	layout->addWidget(tableView);


	this->setLayout(layout);

	this->model = model;
	this->tableView = tableView;
}

void ReadersWidget::keyPressEvent(QKeyEvent *event) {
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

void ReadersWidget::tableDoubleClicked(const QModelIndex &index) {
	if (index.column() == 0) {
		// abort editing
		this->tableView->closePersistentEditor(index);

		emit readerActivated(index.data().toUInt());
	}
}

void ReadersWidget::addRow() {
	this->searchInput->setText("");

	/*
	 * If an empty/dirty row already exists at the passed row nothing will be done.
	 * --> No additional checks needed.
	 */
	this->model->insertRow(0);

	QModelIndex index = this->model->index(0, 1);
	this->tableView->setCurrentIndex(index);
	this->tableView->edit(index);
}

void ReadersWidget::searchChanged(const QString &text) {
	QString value = text.simplified();
	QString filter("");

	if (value.count() > 0) {
		/*
		 * This RegExp either matches a number (i.e. ID),
		 * or a surname,
		 * or a surname and forename seperated by a non-word/non-whitespace character.
		 */
		QRegExp r("(\\d+)|(?:(\\w[\\w ]*)(?:[^\\w ]+(\\w[\\w ]*))?\\W*)");

		if (r.exactMatch(value)) {
			const QStringList m = r.capturedTexts();
			int matchCount = m.count() - 1;

			while (m[matchCount].isEmpty()) {
				matchCount--;
			}

			Q_ASSERT(matchCount >= 1 && matchCount <= 3);

			if (matchCount == 1) {
				filter = "id=" % m[1];
			} else {
				const QString &m2 = m[2];

				if (matchCount == 2) {
					filter = "surname LIKE '" % m2 % "%'";
				} else {
					const QString &m3 = m[3];

					filter = "surname LIKE '" % m2 % "' AND forename LIKE '" % m3 % "%'";
				}
			}
		}
	}

	if (this->model->filter().compare(filter) != 0) {
		this->model->setFilter(filter);
	}
}
