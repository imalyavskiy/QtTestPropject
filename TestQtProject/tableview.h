#ifndef __TABLE_VIEW_H__
#define __TABLE_VIEW_H__

#include <QtWidgets\QTableView>

class CTableView
	: public QTableView
{
	Q_OBJECT;

protected:
	void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
};

#endif // __TABLE_VIEW_H__