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

#include <QtGui\QIcon>
#include <QtGui\QPixmap>
#include <QtCore\QEvent>


//////////////////////////////////////////////////////////////////////////
// CInterviewItem
//////////////////////////////////////////////////////////////////////////
CDataTableRow::CDataTableRow(const QVector<QString>& strings, const QVector<Qt::ItemFlags>& flags, CDataTableRow *parent)
	: parent(parent)
	, m_strings(strings)
	, m_flags(flags)
{
	Q_ASSERT(strings.size() == flags.size());
}

CDataTableRow::~CDataTableRow()
{
}

bool 
CDataTableRow::operator ==(const CDataTableRow& other) const
{
	return (this == &other);
}

bool 
CDataTableRow::isEmpty() const
{
	for (int cItem = 0; cItem < m_strings.size(); ++cItem)
		if (!m_strings.at(cItem).isEmpty())
			return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CInterviewModel
//////////////////////////////////////////////////////////////////////////
CDataTableModel::CDataTableModel(const int columns, QObject *parent)
    : QAbstractItemModel(parent)
	, COLUMNS(columns)
{
	;
}

CDataTableModel* 
CDataTableModel::createInstance(const int rows, const int columns, QObject *parent)
{
	CDataTableModel* model = new CDataTableModel(columns, parent);
	if (model)
		model->initData(rows, columns);

	return model;
}

void 
CDataTableModel::initData(const int rows, const int columns)
{
	for (int cRow = 0; cRow < rows; ++cRow)
	{
		QVector<QString> strings;
		QVector<Qt::ItemFlags> flags;
		for (int cCol = 0; cCol < columns; ++cCol)
		{
			strings.append(QString::number(cRow) + QString(":") + QString::number(cCol));
			flags.append(Qt::ItemIsEditable);
		}
		m_dataRows.append(CDataTableRow(strings, flags));
	}

	checkFooter();
}

QModelIndex 
CDataTableModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid())
		return QModelIndex();

    if (row < m_dataRows.size() && row >= 0 && column < m_dataRows.at(row).m_strings.size() && column >= 0)
	{
		CDataTableRow* parentItem = static_cast<CDataTableRow*>(parent.internalPointer());
		CDataTableRow* item = (row < m_dataRows.size()) ? const_cast<CDataTableRow*>(&m_dataRows[row]) : nullptr;

        if (item)
            return createIndex(row, column, item);
    }
    return QModelIndex();
}

QModelIndex 
CDataTableModel::parent(const QModelIndex &child) const
{
    if (child.isValid()) {
        CDataTableRow *item = static_cast<CDataTableRow*>(child.internalPointer());
        CDataTableRow *parentItem = parent(item);
        if (parentItem)
            return createIndex(row(parentItem), 0, parentItem);
    }
    return QModelIndex();
}

int 
CDataTableModel::rowCount(const QModelIndex &parent) const
{
    return (parent.isValid() && parent.column() != 0) ? 0 : m_dataRows.size();
}

int 
CDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
	return m_dataRows.isEmpty() ? 0 : m_dataRows.at(0).m_strings.size();
}

QVariant 
CDataTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_dataRows.size() || index.column() >= m_dataRows.at(0).m_strings.size())
        return QVariant();

	if (role == Qt::DisplayRole)
		return QVariant(m_dataRows[index.row()].m_strings[index.column()]);

    return QVariant();
}

QVariant 
CDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        return QString::number(section);
    return QAbstractItemModel::headerData(section, orientation, role);
}

bool 
CDataTableModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return false;
    return !m_dataRows.isEmpty();
}

Qt::ItemFlags 
CDataTableModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	CDataTableRow* item = static_cast<CDataTableRow*>(index.internalPointer());

	return item->m_flags.at(index.column()) | QAbstractItemModel::flags(index);
}

CDataTableRow *
CDataTableModel::parent(CDataTableRow *child) const
{
    return child ? child->parent : 0;
}

int 
CDataTableModel::row(const CDataTableRow *node) const
{
	if (!node)
		return -1;

	return m_dataRows.indexOf(*node); // медленно - линейный проход от начала
}

bool 
CDataTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() && role != Qt::DisplayRole)
		return false;

	CDataTableRow* item = static_cast<CDataTableRow*>(index.internalPointer());
	Q_ASSERT(item);
	
	item->m_strings[index.column()] = value.toString();

	checkFooter();

	return true;
}

bool 
CDataTableModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
	return QAbstractItemModel::setItemData(index, roles);
}

void CDataTableModel::checkFooter()
{
	auto funcInsertBlank = [this](const int pos) 
	{
		Q_ASSERT(pos <= m_dataRows.size());

		beginInsertRows(QModelIndex(), pos, pos);

			const QVector<QString> columnData(COLUMNS, QString());
			const QVector<Qt::ItemFlags> columnRoles(COLUMNS, Qt::ItemIsEditable);

			m_dataRows.insert(pos, CDataTableRow(columnData, columnRoles));

		endInsertRows();
	};

	auto funcInsertSpecial = [this](const int pos) {
		Q_ASSERT(pos == m_dataRows.size());

		beginInsertRows(QModelIndex(), pos, pos);

			QVector<QString> columnData;
			QVector<Qt::ItemFlags> columnRoles;
			for (int cColumn = 0; cColumn < COLUMNS; ++cColumn)
			{
				columnData.append((0 == cColumn) ? QString("DEFAULT") : QString("NOTHING"));
				columnRoles.append(Qt::ItemNeverHasChildren | ((0 == cColumn) ? Qt::NoItemFlags : Qt::ItemIsEditable));
			}

			m_dataRows.insert(pos, CDataTableRow(columnData, columnRoles));

		endInsertRows();
	};

	
	if (0 >= m_dataRows.size())
	{							// если нет содержимого - добавим одну единствуенню строку - пустую
		funcInsertBlank(0);
	}
	else if (1 == m_dataRows.size() && !isRowBlank(0))
	{							// если закончилось редактирование одной есдинственной строки
		funcInsertBlank(1);		//	- добавим пустую строку
		funcInsertSpecial(2);	//	- следом за ней дефолтную
	}
	else if(!isRowBlank(m_dataRows.size() - 2))
								// закончилось редактивание строки - добавим на месте дефолтной пустую, 
								// дефолтная сдвинется
		funcInsertBlank(m_dataRows.size() - 1);
}

bool 
CDataTableModel::removeRows(int row, int count, const QModelIndex &parent /* = QModelIndex() */)
{
	if (0 > row || row >= m_dataRows.size())
		return false;

	// Нельзя удалить единственную пустую строку
	if (1 == m_dataRows.size() && isRowBlank(0))
		return false;

	// Нельзя удалить предпосленюю(пустую) и последнюю дефолтную строки
	if (1 < m_dataRows.size() && (row >= m_dataRows.size() - 2))
		return false;
	
	beginRemoveRows(QModelIndex(), row, row);
		m_dataRows.removeAt(row);
	endRemoveRows();

	// Удалено всё. Пустая строка встала в начало, дефолтная за ней
	if (2 == m_dataRows.size() && isRowBlank(0))
	{
		// Убираем дефолтную
		beginRemoveRows(QModelIndex(), 1, 1);
			m_dataRows.removeAt(1);
		endRemoveRows();
	}

	Q_ASSERT(m_dataRows.size() == 1 || m_dataRows.size() >= 3);
	// Строк должно быть либо одна, либо 3 и более

	return true;
}
bool
CDataTableModel::isRowBlank(const int pos)
{
	return (pos < m_dataRows.size()) ? m_dataRows.at(pos).isEmpty() : true;
};
