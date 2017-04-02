#include "tableview.h"
#include <QMouseEvent>
#include <iostream>

void 
CTableView::keyReleaseEvent(QKeyEvent* event)
{
	if (event && Qt::Key_Delete == event->key())
	{
		QItemSelectionModel* selectionModel = CTableView::selectionModel();
		const QItemSelection selection = selectionModel->selection();
		const QModelIndexList selectedIndexes = selection.indexes();

		QSet<int> selectedRows;
		for (QModelIndex selected : selectedIndexes)
		{
			if (true == selectionModel->isRowSelected(selected.row(), selected.parent()) && selectedRows.end() == selectedRows.find(selected.row()))
				selectedRows.insert(selected.row());
		}

		if(selectedRows.isEmpty())
			for (QModelIndex selected : selectedIndexes)
				model()->setData(selected, QString(), Qt::DisplayRole);
		else
			for (QSet<int>::reverse_iterator _it = selectedRows.rbegin(); _it != selectedRows.rend(); ++_it)
				model()->removeRows(*_it, 1);

		selectionModel->select(selection, QItemSelectionModel::Deselect);
		selectionModel->select(selection, QItemSelectionModel::Select);
	}

	QTableView::keyReleaseEvent(event);
}

