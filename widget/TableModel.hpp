//------------------------------------------------------------------------------
//
// TableModel.hpp created by Yyhrs 2019-12-30
//
//------------------------------------------------------------------------------

#ifndef TABLEMODEL_HPP
#define TABLEMODEL_HPP

#include <QAbstractTableModel>
#include <QMetaEnum>

class TableModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	using Row = QVector<QMap<int, QVariantMap>>;

	template<typename Value, typename T>
	using Return = typename QtPrivate::QEnableIf<QtPrivate::IsQEnumHelper<Value>::Value, T>::Type;

	TableModel(QObject *parent = nullptr);

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	int                                            columnCount(const QModelIndex &parent = {}) const override;
	template<typename Header> Return<Header, void> addColumns(const QList<Header> &headers);

	QList<int>                                     rows() const;
	int                                            rowCount(const QModelIndex &parent = {}) const override;
	template<typename Header> Return<Header, void> addRows(const QList<Header> &headers);
	void                                           addRows(int count);
	void                                           addRows(const QList<Row> &rows);
	bool                                           insertRows(int position, int rows, const QModelIndex &parent) override;
	void                                           insertRows(const QList<QPair<int, Row>> &rows);

	using QAbstractTableModel::removeRows;
	void         removeRows(const QList<int> &rows);
	void         takeRows(const QList<int> &rows, QList<Row> &result);
	virtual void clear();

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant data(int row, int column, int role = Qt::DisplayRole) const;
	bool     setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	bool     setData(int row, int column, const QVariant &value, int role = Qt::EditRole);

	const Row           &rowData(int row) const;
	QMap<int, QVariant> itemData(const QModelIndex &index) const override;
	bool                setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) override;

	inline static const QString s_universal{QStringLiteral(".")};

protected:
	QList<Row> m_storage;

private:
	QStringList m_horizontalHeaders;
	QStringList m_verticalHeaders;
};

#include "TableModel.tpp"

#endif // TABLEMODEL_HPP
