/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "interviewmodel.h"

#include <QIcon>
#include <QPixmap>


//////////////////////////////////////////////////////////////////////////
// CInterviewItem
//////////////////////////////////////////////////////////////////////////
CInterviewRow::CInterviewRow(const int items, CInterviewRow *parent)
	: parent(parent)
	, m_itemsInTheRow(items, QString())
{
	;
}

CInterviewRow::~CInterviewRow()
{
}

bool 
CInterviewRow::operator ==(const CInterviewRow& other) const
{
	return (this == &other);
}

//////////////////////////////////////////////////////////////////////////
// CInterviewModel
//////////////////////////////////////////////////////////////////////////
CInterviewModel::CInterviewModel(const int rows, const int columns, QObject *parent)
    : QAbstractItemModel(parent)
	, m_numberOfRows(rows)
	, m_numberOfColumns(columns)
	, m_tree(rows, CInterviewRow(columns))
{
	for (int cRow = 0; cRow < rows; ++cRow)
	{
		for (int cCol = 0; cCol < columns; ++cCol)
		{
			QString& str = m_tree[cRow].m_itemsInTheRow[cCol];
			str.clear();
			str = QString::number(cRow) + QString(":") + QString::number(cCol);
		}
	}
}

CInterviewModel* 
CInterviewModel::createInstance(const int rows, const int columns, QObject *parent)
{
	CInterviewModel* model = new CInterviewModel(rows, columns, parent);
	if (model)
		model->setData();

	return model;
}

CInterviewModel::~CInterviewModel()
{
}

void 
CInterviewModel::setData()
{
	for (int cRow = 0; cRow < m_numberOfRows; ++cRow)
	{
		for (int cCol = 0; cCol < m_numberOfColumns; ++cCol)
		{
			QString& str = m_tree[cRow].m_itemsInTheRow[cCol];
			str.clear();
			str = QString::number(cRow) + QString(":") + QString::number(cCol);
		}
	}
}

QModelIndex 
CInterviewModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid())
		return QModelIndex();

    if (row < m_numberOfRows && row >= 0 && column < m_numberOfColumns && column >= 0) 
	{
        CInterviewRow* parentItem = static_cast<CInterviewRow*>(parent.internalPointer());
        CInterviewRow* item = (row < m_tree.size()) 
			? const_cast<CInterviewRow*>(&m_tree[row]) // хак
			: nullptr;

        if (item)
            return createIndex(row, column, item);
    }
    return QModelIndex();
}

QModelIndex 
CInterviewModel::parent(const QModelIndex &child) const
{
    if (child.isValid()) {
        CInterviewRow *item = static_cast<CInterviewRow*>(child.internalPointer());
        CInterviewRow *parentItem = parent(item);
        if (parentItem)
            return createIndex(row(parentItem), 0, parentItem);
    }
    return QModelIndex();
}

int 
CInterviewModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() && parent.column() != 0) ? 0 : m_numberOfRows;
}

int 
CInterviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_numberOfColumns;
}

QVariant 
CInterviewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_numberOfRows || index.column() >= m_numberOfColumns)
        return QVariant();

	if (role == Qt::DisplayRole)
		return QVariant(m_tree[index.row()].m_itemsInTheRow[index.column()]);

    return QVariant();
}

QVariant 
CInterviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QString::number(section);
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool 
CInterviewModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return false;
    return m_numberOfRows > 0 && m_numberOfColumns > 0;
}

Qt::ItemFlags 
CInterviewModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;
	return Qt::ItemIsDragEnabled|QAbstractItemModel::flags(index);
}

CInterviewRow *
CInterviewModel::parent(CInterviewRow *child) const
{
    return child ? child->parent : 0;
}

int 
CInterviewModel::row(const CInterviewRow *node) const
{
	if (!node)
		return -1;

	return m_tree.indexOf(*node); // линейный проход от начала
}
