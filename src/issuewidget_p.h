/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef ISSUEWIDGET_P_H
#define ISSUEWIDGET_P_H

#include <QSqlQueryModel>


/*
 * The abstract custom drag'n'drop model prototype.
 * See below for full implementations.
 */
class IssueProtoModel : public QSqlQueryModel {
	Q_OBJECT

public:
	explicit IssueProtoModel(uint id, QObject *parent = 0);

	virtual Qt::DropActions supportedDropActions() const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QStringList mimeTypes() const;
	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
	virtual bool dropMimeData(const QMimeData *data, Qt::DropAction, int, int, const QModelIndex &);
	virtual bool drop(const QList<uint> &ids) = 0;

protected:
	const uint readerId;

signals:
	void rowsDropped();
};


/*
 * The drag'n'drop model for the reader side in the IssueWidget.
 */
class IssueReaderModel : public IssueProtoModel {
	Q_OBJECT

public:
	explicit IssueReaderModel(uint id, QObject *parent = 0);

	bool drop(const QList<uint> &ids);
};

/*
 * The drag'n'drop model for the catalogue side in the IssueWidget.
 */
class IssueCatalogueModel : public IssueProtoModel {
	Q_OBJECT

public:
	explicit IssueCatalogueModel(uint id, QObject *parent = 0);

	bool drop(const QList<uint> &ids);
};

#endif
