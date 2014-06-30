/*
 * Name: Leonard Hecker
 * Studiengruppe: 13/041/61
 * MatrNr: XXXXX
 */

#include "basicvalidatingitemdelegate.h"

#include <QLineEdit>
#include <QValidator>


QWidget *BasicValidatingItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	QWidget *widget = QSqlRelationalDelegate::createEditor(parent, option, index);
	QLineEdit *editor = qobject_cast<QLineEdit *>(widget);

	if (editor) {
		switch (index.data().type()) {
		case QVariant::Int:
		case QVariant::UInt:
		case QVariant::LongLong:
		case QVariant::ULongLong:
			editor->setValidator(new QIntValidator(editor));
			break;
		case QVariant::String:
			editor->setValidator(new QRegExpValidator(QRegExp(".+"), editor));
			break;
		default:
			break;
		}
	}

	return widget;
}
