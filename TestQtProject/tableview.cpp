#include "tableview.h"
#include <QtGui\QMouseEvent>
#include <iostream>
#include <set>

void 
CTableView::keyReleaseEvent(QKeyEvent* event)
{
	if (event && Qt::Key_Delete == event->key())
	{
		QItemSelectionModel* selectionModel = CTableView::selectionModel();
		const QItemSelection selection = selectionModel->selection();
		const QModelIndexList selectedIndexes = selection.indexes();

		// —обираем выделенные строки
		std::set<int> selectedRows;
		for (QModelIndex selected : selectedIndexes)
		{
			if (true == selectionModel->isRowSelected(selected.row(), selected.parent()) 
				&& selectedRows.end() == std::find(selectedRows.begin(), selectedRows.end(), selected.row()))
				selectedRows.insert(selected.row());
		}

		if(selectedRows.empty()) // ≈сли нет выделенных строк - очищаем содержимое выеленных €чеек
			for (QModelIndex selected : selectedIndexes)
				model()->setData(selected, QString(), Qt::DisplayRole);
		else					 // в противном случае - удал€ем строки, но не очищаем €чейки 
								 // в неполностью выделенных строках
			for (std::set<int>::reverse_iterator _it = selectedRows.rbegin(); _it != selectedRows.rend(); ++_it)
				model()->removeRows(*_it, 1);

		// ћагические пассы - при очищении €чеек надо перевыделить выделенное
		// что бы отобразилось отсутствие данных
		selectionModel->select(selection, QItemSelectionModel::Deselect);
		selectionModel->select(selection, QItemSelectionModel::Select);
	}

	QTableView::keyReleaseEvent(event);
}

