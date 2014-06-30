/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "issuewidget_p.h"

#include <QList>
#include <QStringList>
#include <QMimeData>
#include <QSqlQuery>
#include <set>


static const char *kCatalogueIdMime = "application/x-catalogue-id";


IssueProtoModel::IssueProtoModel(uint id, QObject *parent) : QSqlQueryModel(parent), readerId(id) {
}

Qt::DropActions IssueProtoModel::supportedDropActions() const {
	return Qt::MoveAction;
}

Qt::ItemFlags IssueProtoModel::flags(const QModelIndex &index) const {
	Qt::ItemFlags flags = Qt::ItemIsDropEnabled | QSqlQueryModel::flags(index);

	if (index.isValid()) {
		flags |= Qt::ItemIsDragEnabled;
	}

	return flags;
}

QStringList IssueProtoModel::mimeTypes() const {
	return QStringList(kCatalogueIdMime);
}

QMimeData *IssueProtoModel::mimeData(const QModelIndexList &indexes) const {
	int summedLength = 0;
	std::set<QByteArray> idSet;
	QByteArray ids;

	for (const QModelIndex &index : indexes) {
		const QByteArray id = index.sibling(index.row(), 0).data().toByteArray();

		if (idSet.emplace(id).second) {
			summedLength += id.size() + 1;
		}
	}

	ids.reserve(summedLength);

	for (const QByteArray &id : idSet) {
		if (!ids.isEmpty()) {
			ids.append(',');
		}

		ids.append(id);
	}

	QMimeData *data = new QMimeData();
	data->setData(kCatalogueIdMime, ids);
	return data;
}

bool IssueProtoModel::dropMimeData(const QMimeData *data, Qt::DropAction, int, int, const QModelIndex &) {
	QList<QByteArray> bytearrayList = data->data(kCatalogueIdMime).split(',');
	QList<uint> ids;
	ids.reserve(bytearrayList.size());

	for (const QByteArray &bytearray : bytearrayList) {
		bool ok;
		int id = bytearray.toInt(&ok, 10);

		if (ok) {
			ids << id;
		}
	}

	return this->drop(ids);
}


IssueReaderModel::IssueReaderModel(uint id, QObject *parent) : IssueProtoModel(id, parent) {
}

bool IssueReaderModel::drop(const QList<uint> &ids) {
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query;
	query.prepare("UPDATE catalogue SET issued_to = ?, issued_on = DATE('now') WHERE id = ?");
	query.bindValue(0, readerId);

	for (const uint id : ids) {
		query.bindValue(1, id);

		if (!query.exec()) {
			db.rollback();
			return false;
		}
	}

	if (!db.commit()) {
		db.rollback();
		return false;
	}

	emit rowsDropped();
	return true;
}


IssueCatalogueModel::IssueCatalogueModel(uint id, QObject *parent) : IssueProtoModel(id, parent) {
}

bool IssueCatalogueModel::drop(const QList<uint> &ids) {
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	QSqlQuery query;
	query.prepare("UPDATE catalogue SET issued_to = NULL, issued_on = NULL WHERE issued_to = ? AND id = ?");
	query.bindValue(0, readerId);

	for (const uint id : ids) {
		query.bindValue(1, id);

		if (!query.exec()) {
			db.rollback();
			return false;
		}
	}

	if (!db.commit()) {
		db.rollback();
		return false;
	}

	emit rowsDropped();
	return true;
}
