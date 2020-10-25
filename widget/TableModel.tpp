//------------------------------------------------------------------------------
//
// TableModel.tpp created by Yyhrs 2019-12-30
//
//------------------------------------------------------------------------------

#ifndef TABLEMODEL_TPP
#define TABLEMODEL_TPP

#include "TableModel.hpp"

template<typename Header>
TableModel::Return<Header, void> TableModel::addColumns(const QList<Header> &headers)
{
	int column{columnCount()};

	for (const auto &header: headers)
	{
		QString label{QMetaEnum::fromType<Header>().valueToKey(header)};

		setHeaderData(column++, Qt::Horizontal, label, Qt::DisplayRole);
		m_horizontalHeaders << label;
	}
}

template<typename Header>
TableModel::Return<Header, void> TableModel::addRows(const QList<Header> &headers)
{
	int row{rowCount()};

	for (const auto &header: headers)
	{
		QString label{QMetaEnum::fromType<Header>().valueToKey(header)};

		setHeaderData(row++, Qt::Vertical, label, Qt::DisplayRole);
		m_verticalHeaders << label;
	}
	addRows(headers.count());
}

#endif // TABLEMODEL_TPP
