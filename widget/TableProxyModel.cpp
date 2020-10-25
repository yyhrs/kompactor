//------------------------------------------------------------------------------
//
// TableProxyModel.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include <QRegularExpression>

#include "TableProxyModel.hpp"

TableProxyModel::TableProxyModel(QObject *parent):
	QSortFilterProxyModel{parent}
{
}

void TableProxyModel::clearFilter()
{
	m_columnPatterns.clear();
	m_rows.clear();
	invalidateFilter();
}

void TableProxyModel::addFilterRows(const QSet<int> &rows)
{
	m_rows = rows;
	m_filter |= !rows.isEmpty();
	invalidateFilter();
}

void TableProxyModel::addFilterPattern(int column, const QString &pattern)
{
	m_columnPatterns[column] = pattern;
	invalidateFilter();
}

void TableProxyModel::setFilter(bool on)
{
	m_filter = on;
	invalidateFilter();
}

bool TableProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if (!m_filter)
		return true;
	else if (!m_rows.isEmpty() && !m_rows.contains(sourceRow))
		return false;
	for (auto key: m_columnPatterns.keys())
	{
		auto &pattern{m_columnPatterns[key]};

		if (!pattern.isEmpty() && !sourceModel()->index(sourceRow, key, sourceParent).data().toString().contains(QRegExp{pattern, Qt::CaseInsensitive}))
			return false;
	}
	return true;
}
