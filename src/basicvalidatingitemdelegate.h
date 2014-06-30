/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#ifndef BASICVALIDATINGITEMDELEGATE_H
#define BASICVALIDATINGITEMDELEGATE_H

#include <QSqlRelationalDelegate>

/*
 * A custom QItemDelegate, which supports
 * - QSqlRelationalTableModel (displaying drop down menus when foreign data must be selected)
 * - validation with QIntValidator for INTEGER columns
 * - checking VARCHAR, TEXT, etc. columns for a length greater than zero
 */
class BasicValidatingItemDelegate : public QSqlRelationalDelegate {
public:
	explicit BasicValidatingItemDelegate(QObject * parent = 0) : QSqlRelationalDelegate(parent) {}

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
