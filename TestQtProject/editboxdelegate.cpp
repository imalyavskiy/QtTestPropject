#include "editboxdelegate.h"

#include <QLineEdit>

CEditBoxDelegate::CEditBoxDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QWidget*
CEditBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	QLineEdit *lineEdit = new QLineEdit(parent);
	lineEdit->setFrame(false);

	return lineEdit;
}

void 
CEditBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString contents = index.model()->data(index, Qt::DisplayRole).toString();

	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setText(contents);
}

void 
CEditBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	QString contents = lineEdit->text();

	model->setData(index, contents, Qt::DisplayRole);
}

void 
CEditBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(index);

	editor->setGeometry(option.rect);
}

bool 
CEditBoxDelegate::eventFilter(QObject *object, QEvent *event)
{
	return QStyledItemDelegate::eventFilter(object, event);
}