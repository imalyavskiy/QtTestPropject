#include "tableview.h"
#include <QMouseEvent>

CTableView::CTableView()
{
	;
}

CTableView::~CTableView()
{
	;
}

void CTableView::mousePressEvent(QMouseEvent *event)
{
	QTableView::mousePressEvent(event);
}

void CTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QModelIndex index = indexAt(event->pos());
		edit(index);
	}
	QTableView::mouseDoubleClickEvent(event);
}