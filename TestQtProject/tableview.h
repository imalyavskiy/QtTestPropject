#ifndef __TABLE_VIEW_H__
#define __TABLE_VIEW_H__

#include <QTableView>

class CTableView
	: public QTableView
{
	Q_OBJECT;
public:
	CTableView();
	~CTableView();

	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // __TABLE_VIEW_H__