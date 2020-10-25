//------------------------------------------------------------------------------
//
// TableProxyModel.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef TABLEPROXYMODEL_HPP
#define TABLEPROXYMODEL_HPP

#include <QSet>
#include <QSortFilterProxyModel>

class TableProxyModel: public QSortFilterProxyModel
{
	Q_OBJECT

public:
	explicit TableProxyModel(QObject *parent = nullptr);

	void	clearFilter();
	void	addFilterRows(const QSet<int> &rows);
	void	addFilterPattern(int column, const QString &pattern);
	void	setFilter(bool on);

protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

	QSet<int>			m_rows;
	QMap<int, QString>	m_columnPatterns;
	bool				m_filter;
};

#endif // TABLEPROXYMODEL_HPP
