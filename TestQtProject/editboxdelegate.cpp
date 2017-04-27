#include "editboxdelegate.h"

#include <QLineEdit>
#include <QTextEdit>

CEditBoxDelegate::CEditBoxDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QWidget*
CEditBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	
	if(0 == index.column())
	{
		QLineEdit *lineEdit = new QLineEdit(parent);
		lineEdit->setFrame(false);
		return lineEdit;
	}
	if(1 == index.column())
	{
		QTextEdit* textEdit = new QTextEdit(parent);
		return textEdit;
	}
	
	return nullptr;
}

void 
CEditBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString contents = index.model()->data(index, Qt::DisplayRole).toString();

	if(0 == index.column())
	{
		QLineEdit *lineEdit = dynamic_cast<QLineEdit*>(editor);
		lineEdit->setText(contents);
	}
	else
	if(1 == index.column())
	{
		QTextEdit *textEdit = dynamic_cast<QTextEdit*>(editor);
		textEdit->setText(contents);
	}
}

void 
CEditBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QString contents;

	if(0 == index.column()){
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		contents = lineEdit->text();
	}
	else
	if(1 == index.column())
	{
		QTextEdit *textEdit = static_cast<QTextEdit*>(editor);
		contents = textEdit->toPlainText();
	}

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