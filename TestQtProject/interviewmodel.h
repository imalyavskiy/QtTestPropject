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

#ifndef MODEL_H
#define MODEL_H

#include <QAbstractItemModel>
#include <QFileIconProvider>
#include <QIcon>
#include <QVector>

class CInterviewRow
{
public:
	CInterviewRow(const int items = 0, CInterviewRow *parent = nullptr);
	
	~CInterviewRow();

	bool operator ==(const CInterviewRow& other) const;

//protected:
	CInterviewRow *parent;
	QVector<QString> m_data;			// Данные элементов таблицы в текущей строке
	QVector<Qt::ItemFlags> m_flags;		// Флаги элементов таблицы в текущей строке
};


class CInterviewModel : public QAbstractItemModel
{
    Q_OBJECT
protected:
	CInterviewModel(const int rows, const int columns, QObject *parent = 0);

public:
	static CInterviewModel* createInstance(const int rows, const int columns, QObject *parent);
	~CInterviewModel();

	void setData();

    QModelIndex index(int row, int column, const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &child) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    bool hasChildren(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

private:
    CInterviewRow *parent(CInterviewRow *child) const;
    int row(const CInterviewRow *node) const;

    int m_numberOfRows;
    int m_numberOfColumns;
    QVector<CInterviewRow> m_tree;
    QFileIconProvider iconProvider;
};

#endif // MODEL_H
