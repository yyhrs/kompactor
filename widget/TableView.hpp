//------------------------------------------------------------------------------
//
// TableView.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef TABLEVIEW_HPP
#define TABLEVIEW_HPP

#include <QDialog>
#include <QLabel>
#include <QTableView>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>

#include "TableProxyModel.hpp"

class TableView: public QTableView
{
	Q_OBJECT
	Q_PROPERTY(int searchTableHeight READ searchTableHeight WRITE setSearchTableHeight)

public:
	enum Action
	{
		ToggleFilter,
		AdjustColumns,
		AdjustRows,
		HideColumns,
		ShowAllColumns
	};

	explicit TableView(QWidget *parent = nullptr);
	~TableView() override = default;

	void                setModel(QAbstractItemModel *model) override;
	QAbstractProxyModel &proxyModel();

	const QMap<Action, QAction *> &actions() const;
	const QList<QAction *>        &columnActions() const;

	void            setColumnsHidden(const QList<int> &columns, bool hide);
	void            setColumnHidden(int column, bool hide);
	void            hideColumn(int column);
	void            showColumn(int column);
	void            hideSelectedColumns();
	void            resizeSelectedColumnsToContents();
	QList<int>      logicalRows();
	QList<int>      logicalSelectedRows();
	QList<int>      visualSelectedRows();
	QModelIndexList logicalSelectedIndexes(int column = 0);
	QModelIndexList visualSelectedIndexes(int column = 0);

	virtual void setZoomPercentage(int zoomPercentage);
	void         refreshZoomView();

	void         refreshSearchTable();
	virtual void setFiler(bool on);
	virtual void addFiler(const QSet<int> &rows);
	virtual void addFiler(int column, const QString &text);

signals:
	void zoomChanged(int zoomPourcentage);
	void filterRowsAdded(const QSet<int> &rows);
	void filterPatternAdded(int column, const QString &pattern);
	void columnHidden(int column, bool hide);

protected:
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

	virtual QAbstractProxyModel *createProxyModel() const;

	virtual void invalideFilers();

	int  searchTableHeight();
	void setSearchTableHeight(int height);
	void setSearchTableHidden(bool hide);

	virtual void copy();

	QTableWidget m_searchTable;

	void connectHeader();

private:
	Q_DISABLE_COPY(TableView)

	void connectActions();
	void connectWidgets();

	void showSizeDialog();

	QAbstractProxyModel     *m_tableProxyModel{nullptr};
	QDialog                 *m_sizeDialog{nullptr};
	QLabel                  *m_sizeLabel{nullptr};
	QVBoxLayout             m_searchTableLayout;
	QMap<Action, QAction *> m_actions;
	QList<QAction *>        m_columnActions;
	int                     m_height{0};
	int                     m_currentSection{0};
	QTimer                  m_zoomTimer;
	int                     m_zoomPourcentage{100};
	int                     m_zoomBaseSize{0};

	inline static const QString s_sizeFormat{QStringLiteral("<font size='36' color='#4d4d4d' face='consolas'>%1%</font>")};
};

#endif // TABLEVIEW_HPP
