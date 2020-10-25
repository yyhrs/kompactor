//------------------------------------------------------------------------------
//
// TableModel.cpp created by Yyhrs 2019-12-30
//
//------------------------------------------------------------------------------

#include "TableModel.hpp"

TableModel::TableModel(QObject *parent):
	QAbstractTableModel{parent}
{
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal && section < m_horizontalHeaders.count())
			return m_horizontalHeaders.value(section);
		else if (orientation == Qt::Vertical && section < m_verticalHeaders.count())
			return m_verticalHeaders.value(section);
	}
	return QAbstractTableModel::headerData(section, orientation, role);
}

int TableModel::columnCount(const QModelIndex & /*parent*/) const
{
	return m_horizontalHeaders.count();
}

QList<int> TableModel::rows() const
{
	QList<int> rows;

	for (int row = 0; row < rowCount(); ++row)
		rows << row;
	return rows;
}

int TableModel::rowCount(const QModelIndex & /*parent*/) const
{
	return m_storage.count();
}

void TableModel::addRows(int count)
{
	beginInsertRows({}, rowCount(), rowCount() + count - 1);
	while (count--)
		m_storage.append(Row{columnCount()});
	endInsertRows();
}

void TableModel::addRows(const QList<Row> &rows)
{
	beginInsertRows({}, rowCount(), rowCount() + rows.count() - 1);
	m_storage << rows;
	endInsertRows();
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	Row row;

	row.resize(columnCount());
	beginInsertRows(parent, position, position + rows - 1);
	m_storage.insert(position, row);
	endInsertRows();
	return true;
}

void TableModel::insertRows(const QList<QPair<int, Row>> &rows)
{
	int count{1};

	if (rows.isEmpty())
		return ;
	while (count < rows.length() && rows.at(count - 1).first == rows.at(count).first - 1)
		++count;
	beginInsertRows({}, rows.first().first, rows.at(count - 1).first);
	for (const auto &row: rows.mid(0, count))
		m_storage.insert(row.first, row.second);
	endInsertRows();
	if (count < rows.count())
		insertRows(rows.mid(count, rows.count() - count));
}

void TableModel::removeRows(const QList<int> &rows)
{
	int count{1};

	if (rows.isEmpty())
		return ;
	while (count < rows.length() && rows.at(count - 1) == rows.at(count) + 1)
		++count;
	beginRemoveRows({}, rows.at(count - 1), rows.first());
	for (const auto &row: rows.mid(0, count))
		m_storage.removeAt(row);
	endRemoveRows();
	if (count < rows.count())
		removeRows(rows.mid(count, rows.count() - count));
}

void TableModel::takeRows(const QList<int> &rows, QList<Row> &result)
{
	int count{1};

	if (rows.isEmpty())
		return ;
	while (count < rows.length() && rows.at(count - 1) == rows.at(count) + 1)
		++count;
	beginRemoveRows({}, rows.at(count - 1), rows.first());
	for (const auto &row: rows.mid(0, count))
		result << m_storage.takeAt(row);
	endRemoveRows();
	if (count < rows.count())
		takeRows(rows.mid(count, rows.count() - count), result);
}

void TableModel::clear()
{
	if (m_storage.count())
	{
		beginRemoveRows({}, 0, m_storage.count() - 1);
		m_storage.clear();
		endRemoveRows();
	}
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
	return data(index.row(), index.column(), role);
}

QVariant TableModel::data(int row, int column, int role) const
{
	return m_storage[row][column][role == Qt::EditRole ? Qt::DisplayRole : role][s_universal];
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return setData(index.row(), index.column(), value, role);
}

bool TableModel::setData(int row, int column, const QVariant &value, int role)
{
	if (m_storage[row][column][role == Qt::EditRole ? Qt::DisplayRole : role][s_universal] != value)
	{
		m_storage[row][column][role == Qt::EditRole ? Qt::DisplayRole : role][s_universal] = value;
		emit dataChanged(index(row, column), index(row, column), {role});
	}
	return true;
}

const TableModel::Row &TableModel::rowData(int row) const
{
	return m_storage[row];
}

QMap<int, QVariant> TableModel::itemData(const QModelIndex &index) const
{
	QMap<int, QVariant> result;

	for (auto role: m_storage[index.row()][index.column()].keys())
		result.insert(role, QVariant::fromValue(m_storage[index.row()][index.column()][role]));
	return result;
}

bool TableModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
	for (auto role: roles.keys())
		m_storage[index.row()][index.column()][role] = roles[role].toMap();
	return true;
}
