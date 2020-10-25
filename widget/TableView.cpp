//------------------------------------------------------------------------------
//
// TableView.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QGuiApplication>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenu>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QScrollBar>

#include "TableView.hpp"

TableView::TableView(QWidget *parent):
	QTableView{parent},
	m_zoomTimer{this}
{
	auto *layout{new QVBoxLayout};

	m_actions[ToggleFilter] = new QAction{QIcon::fromTheme("filter"), "Toggle Filter", this};
	m_actions[ToggleFilter]->setCheckable(true);
	m_actions[AdjustColumns] = new QAction{QIcon::fromTheme("horizontal"), "Adjust Columns", this};
	m_actions[AdjustRows] = new QAction{QIcon::fromTheme("vertical"), "Adjust Rows", this};
	m_actions[HideColumns] = new QAction{QIcon::fromTheme("hide"), "Hide Column(s)", this};
	m_actions[ShowAllColumns] = new QAction{QIcon::fromTheme("show"), "Show All Column", this};
	m_sizeLabel = new QLabel;
	m_sizeLabel->setAttribute(Qt::WA_TranslucentBackground);
	m_zoomTimer.setSingleShot(true);
	connect(&m_zoomTimer, &QTimer::timeout, [this]
	{
		m_sizeDialog->hide();
	});
	layout->addWidget(m_sizeLabel);
	m_sizeDialog = new QDialog{this, Qt::FramelessWindowHint | Qt::ToolTip};
	m_sizeDialog->setLayout(layout);
	m_sizeDialog->setAttribute(Qt::WA_TranslucentBackground);
	m_height = horizontalHeader()->height();
	setLayout(&m_searchTableLayout);
	m_searchTableLayout.setSpacing(0);
	m_searchTableLayout.setContentsMargins(0, 0, 0, 0);
	m_searchTableLayout.addWidget(&m_searchTable);
	m_searchTableLayout.addStretch(0);
	m_searchTableLayout.addItem(new QSpacerItem{0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding});
	m_searchTableLayout.setAlignment(parent->layout(), Qt::AlignTop);
	m_searchTable.setParent(this);
	m_searchTable.setHidden(true);
	m_searchTable.setFrameShape(QFrame::NoFrame);
	m_searchTable.setEditTriggers(QAbstractItemView::AllEditTriggers);
	m_searchTable.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	m_searchTable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_searchTable.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	connectHeader();
	connectActions();
	connectWidgets();
}

void TableView::setModel(QAbstractItemModel *model)
{
	m_zoomBaseSize = 0;
	m_tableProxyModel = createProxyModel();
	m_tableProxyModel->setParent(this);
	QTableView::setModel(m_tableProxyModel);
	m_tableProxyModel->setSourceModel(model);
	for (int index = 0; index < horizontalHeader()->model()->columnCount(); ++index)
	{
		auto *action{new QAction{horizontalHeader()->model()->headerData(index, Qt::Horizontal).toString()}};

		action->setCheckable(true);
		action->setChecked(!horizontalHeader()->isSectionHidden(index));
		connect(action, &QAction::toggled, [this, index](bool checked)
		{
			setColumnHidden(index, !checked);
			emit columnHidden(index, !checked);
			if (checked)
				resizeColumnToContents(index);
		});
		m_columnActions << action;
	}
	refreshSearchTable();
}

QAbstractProxyModel &TableView::proxyModel()
{
	return *m_tableProxyModel;
}

const QMap<TableView::Action, QAction *> &TableView::actions() const
{
	return m_actions;
}

const QList<QAction *> &TableView::columnActions() const
{
	return m_columnActions;
}

void TableView::setColumnsHidden(const QList<int> &columns, bool hide)
{
	for (auto column: columns)
		setColumnHidden(column, hide);
}

void TableView::setColumnHidden(int column, bool hide)
{
	m_searchTable.setColumnHidden(column, hide);
	QTableView::setColumnHidden(column, hide);
}

void TableView::hideColumn(int column)
{
	setColumnHidden(column, true);
}

void TableView::showColumn(int column)
{
	setColumnHidden(column, false);
}

void TableView::hideSelectedColumns()
{
	for (const auto &index: horizontalHeader()->selectionModel()->selectedColumns())
		m_columnActions.at(index.column())->setChecked(false);
}

void TableView::resizeSelectedColumnsToContents()
{
	for (const auto &index: horizontalHeader()->selectionModel()->selectedColumns())
		resizeColumnToContents(index.column());
}

QList<int> TableView::logicalRows()
{
	QList<int> rows;

	for (int row = 0; row < model()->rowCount(); ++row)
		rows << proxyModel().mapToSource(proxyModel().index(row, 0)).row();
	return rows;
}

QList<int> TableView::logicalSelectedRows()
{
	QList<int> rows;

	for (int row = 0; row < model()->rowCount(); ++row)
		if (selectionModel()->rowIntersectsSelection(row, {}))
			rows << proxyModel().mapToSource(proxyModel().index(row, 0)).row();
	return rows;
}

QList<int> TableView::visualSelectedRows()
{
	QList<int> rows;

	for (int row = 0; row < model()->rowCount(); ++row)
		if (selectionModel()->rowIntersectsSelection(row, {}))
			rows << proxyModel().index(row, 0).row();
	return rows;
}

QModelIndexList TableView::logicalSelectedIndexes(int column)
{
	QModelIndexList indexes;

	for (int row = 0; row < model()->rowCount(); ++row)
		if (selectionModel()->rowIntersectsSelection(row, {}))
			indexes << proxyModel().mapToSource(proxyModel().index(row, column));
	return indexes;
}

QModelIndexList TableView::visualSelectedIndexes(int column)
{
	QModelIndexList indexes;

	for (int row = 0; row < model()->rowCount(); ++row)
		if (selectionModel()->rowIntersectsSelection(row, {}))
			indexes << proxyModel().index(row, column);
	return indexes;
}

void TableView::setZoomPercentage(int zoomPercentage)
{
	if (m_zoomBaseSize == 0)
		m_zoomBaseSize = font().pixelSize();
	m_zoomPourcentage = zoomPercentage;
	refreshZoomView();
}

void TableView::refreshZoomView()
{
	if (m_zoomBaseSize)
	{
		for (int row = 0; row < model()->rowCount(); ++row)
			setRowHeight(row, verticalHeader()->minimumSectionSize() * m_zoomPourcentage / 100);
		setStyleSheet("TableView { font-size: " + QString::number(m_zoomBaseSize * m_zoomPourcentage / 100) + "px; }");
	}
}

void TableView::refreshSearchTable()
{
	QList<int> filterColumns;

	if (!m_searchTable.isHidden())
	{
		setSearchTableHeight(0);
		m_searchTable.hide();
	}
	m_searchTable.clearContents();
	m_searchTable.setHorizontalScrollMode(horizontalScrollMode());
	m_searchTable.verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	m_searchTable.verticalHeader()->setHidden(verticalHeader()->isHidden());
	m_searchTable.setRowCount(1);
	m_searchTable.setColumnCount(model()->columnCount());
	for (int column = 0; column < m_searchTable.columnCount(); ++column)
	{
		auto *lineEdit{new QLineEdit};

		lineEdit->setPlaceholderText(QStringLiteral("..."));
		m_searchTable.setCellWidget(0, column, lineEdit);
		m_searchTable.horizontalHeader()->resizeSection(column, horizontalHeader()->sectionSize(0));
		connect(lineEdit, &QLineEdit::textEdited, [this, column](const QString &text)
		{
			addFiler(column, text);
		});
		filterColumns << column;
	}
	m_searchTable.setVerticalHeaderItem(0, new QTableWidgetItem);
	m_searchTable.horizontalHeader()->hide();
	invalideFilers();
	m_searchTable.setMinimumHeight(0);
	m_searchTable.setMaximumHeight(0);
}

void TableView::setFiler(bool on)
{
	if (on == m_searchTable.isHidden())
	{
		qobject_cast<TableProxyModel *>(m_tableProxyModel)->setFilter(on);
		setSearchTableHidden(!on);
		m_actions[ToggleFilter]->setChecked(on);
	}
}

void TableView::addFiler(const QSet<int> &rows)
{
	clearSelection();
	qobject_cast<TableProxyModel *>(m_tableProxyModel)->addFilterRows(rows);
	emit filterRowsAdded(rows);
}

void TableView::addFiler(int column, const QString &text)
{
	clearSelection();
	qobject_cast<TableProxyModel *>(m_tableProxyModel)->addFilterPattern(column, text);
	emit filterPatternAdded(column, text);
}

void TableView::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() & Qt::ControlModifier)
	{
		if (event->angleDelta().y() > 1 && m_zoomPourcentage < 200)
			setZoomPercentage(m_zoomPourcentage + 10);
		else if (event->angleDelta().y() < 1 && m_zoomPourcentage > 10)
			setZoomPercentage(m_zoomPourcentage - 10);
		showSizeDialog();
		emit zoomChanged(m_zoomPourcentage);
	}
	else
		QTableView::wheelEvent(event);
}

void TableView::keyPressEvent(QKeyEvent *event)
{
	if (event->matches(QKeySequence::Copy))
		copy();
	else if (event->matches(QKeySequence::Find))
		m_actions[ToggleFilter]->toggle();
	else if (event->matches(QKeySequence::Cancel))
		m_actions[ToggleFilter]->setChecked(false);
	else
		QTableView::keyPressEvent(event);
}

QAbstractProxyModel *TableView::createProxyModel() const
{
	return new TableProxyModel;
}

void TableView::invalideFilers()
{
	qobject_cast<TableProxyModel *>(m_tableProxyModel)->clearFilter();
}

int TableView::searchTableHeight()
{
	return m_searchTable.height();
}

void TableView::setSearchTableHeight(int height)
{
	horizontalHeader()->setMinimumHeight(m_height + height);
	horizontalHeader()->setMaximumHeight(m_height + height);
	m_searchTable.setMinimumHeight(height);
	m_searchTable.setMaximumHeight(height);
}

void TableView::setSearchTableHidden(bool hide)
{
	auto *filterAnimation{new QPropertyAnimation{this, "searchTableHeight", parent()}};

	m_searchTable.show();
	if (hide)
	{
		filterAnimation->setStartValue(m_height);
		filterAnimation->setEndValue(0);
		horizontalHeader()->setMinimumHeight(m_height);
		horizontalHeader()->setMaximumHeight(m_height);
	}
	else
	{
		m_searchTable.setVerticalScrollBarPolicy(verticalScrollBarPolicy());
		m_searchTableLayout.setContentsMargins(0, m_height, 0, 0);
		filterAnimation->setStartValue(0);
		filterAnimation->setEndValue(m_height);
	}
	filterAnimation->setDuration(100);
	filterAnimation->start(QAbstractAnimation::DeleteWhenStopped);
	connect(filterAnimation, &QPropertyAnimation::finished, [this, hide]
	{
		m_searchTable.setHidden(hide);
		emit horizontalHeader()->geometriesChanged();
	});
}

void TableView::copy()
{
	static const QString htmlHeader{"<td style='background-color:%1;font-weight: bold'>%2</td>"};
	static const QString htmlCell{"<td style='background-color:%1'>%2</td>"};
	QVector<QStringList> rows{2};
	QVector<QString>     clipboard{2};
	QString              color{QColor{Qt::lightGray}.name()};

	auto *mime{new QMimeData};

	clipboard[0].append("<html><style>br{mso-data-placement: same-cell;}td{border: 1px solid DarkGray;}</style><body><table><tr>");
	for (int column = 0; column < model()->columnCount(); ++column)
		if (!isColumnHidden(column) && selectionModel()->columnIntersectsSelection(column, QModelIndex()))
			clipboard[0].append(htmlHeader.arg(color, model()->headerData(column, Qt::Horizontal).toString().replace("\n", "<br>")));
	clipboard[0].append("</tr>");
	for (int row = 0; row < model()->rowCount(); ++row)
		if (selectionModel()->rowIntersectsSelection(row, QModelIndex()))
		{
			QVector<QStringList> cells{2};

			for (int column = 0; column < model()->columnCount(); ++column)
				if (!isColumnHidden(column) && selectionModel()->columnIntersectsSelection(column, QModelIndex()))
				{
					QModelIndex index(model()->index(row, column));
					QString     value;

					if (selectionModel()->isSelected(index))
					{
						cells[1] << model()->itemData(index).value(Qt::DisplayRole).toString().replace("\n", " ");
						value = model()->itemData(index).value(Qt::DisplayRole).toString().replace("\n", "<br>");
					}
					else
						cells[1] << "-";
					color = model()->itemData(index).value(Qt::BackgroundRole).value<QColor>().name();
					cells[0] << htmlCell.arg(color.contains("#000000") ? "white" : color, value);
					row = index.row();
				}
			rows[0] << cells[0].join("");
			rows[1] << cells[1].join('\t');
		}
	clipboard[0].append("<tr>").append(rows[0].join("</tr><tr>")).append("</tr>").append("</table></body></html>");
	clipboard[1].append(rows[1].join('\n'));
	mime->setHtml(clipboard[0]);
	mime->setText(clipboard[1]);
	QGuiApplication::clipboard()->setMimeData(mime);
}

void TableView::connectHeader()
{
	horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignTop | static_cast<Qt::Alignment>(Qt::TextWordWrap));
	horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(horizontalHeader(), &QHeaderView::customContextMenuRequested, [this](const QPoint &pos)
	{
		QMenu            contextMenu{QStringLiteral("Header Context Menu"), this};
		QList<QAction *> actions{m_actions[ShowAllColumns]};

		contextMenu.addAction(m_actions[ToggleFilter]);
		contextMenu.addSeparator();
		contextMenu.addActions({m_actions[AdjustColumns], m_actions[AdjustRows]});
		if (selectionModel()->isColumnSelected(indexAt(pos).column(), QModelIndex{}))
			contextMenu.addAction(m_actions[HideColumns]);
		actions << m_columnActions;
		contextMenu.addMenu(QStringLiteral("Columns"))->addActions(actions);
		contextMenu.exec(mapToGlobal(pos));
	});
	connect(horizontalHeader(), &QHeaderView::sectionMoved, [this](int logicalIndex, int, int newVisualIndex)
	{
		m_searchTable.horizontalHeader()->moveSection(m_searchTable.horizontalHeader()->visualIndex(logicalIndex), newVisualIndex);
	});
	connect(horizontalHeader(), &QHeaderView::sectionResized, [this](int logicalIndex, int, int newSize)
	{
		m_searchTable.verticalHeader()->setMinimumWidth(verticalHeader()->width());
		m_searchTable.horizontalHeader()->resizeSection(logicalIndex, newSize);
		for (const auto &index: horizontalHeader()->selectionModel()->selectedColumns())
			horizontalHeader()->resizeSection(index.column(), newSize);
	});
	connect(horizontalHeader(), &QHeaderView::sectionHandleDoubleClicked, this, &TableView::resizeSelectedColumnsToContents);
	connect(horizontalScrollBar(), &QScrollBar::valueChanged, m_searchTable.horizontalScrollBar(), &QScrollBar::setValue);
	connect(horizontalScrollBar(), &QScrollBar::sliderMoved, m_searchTable.horizontalScrollBar(), &QScrollBar::setSliderPosition);
	connect(horizontalHeader(), &QHeaderView::geometriesChanged, [this]
	{
		m_searchTable.horizontalHeader()->restoreState(horizontalHeader()->saveState());
	});
}

void TableView::connectActions()
{
	connect(m_actions[ToggleFilter], &QAction::toggled, this, &TableView::setFiler);
	connect(m_actions[AdjustColumns], &QAction::triggered, this, &TableView::resizeColumnsToContents);
	connect(m_actions[AdjustRows], &QAction::triggered, this, &TableView::resizeRowsToContents);
	connect(m_actions[HideColumns], &QAction::triggered, this, &TableView::hideSelectedColumns);
	connect(m_actions[ShowAllColumns], &QAction::triggered, [this]
	{
		for (auto action: m_columnActions)
			action->setChecked(true);
	});
}

void TableView::connectWidgets()
{
	connect(verticalHeader(), &QHeaderView::sectionCountChanged, [this](int, int newCount)
	{
		if (newCount == 0)
			m_searchTable.verticalHeader()->hide();
		else
			m_searchTable.verticalHeader()->setHidden(verticalHeader()->isHidden());
	});
	connect(verticalHeader(), &QHeaderView::geometriesChanged, [this]
	{
		m_searchTable.verticalHeader()->setMinimumWidth(verticalHeader()->width());
	});
	connect(m_searchTable.horizontalScrollBar(), &QScrollBar::valueChanged, horizontalScrollBar(), &QScrollBar::setValue);
}

void TableView::showSizeDialog()
{
	QRect geometry{0, 0, 0, 0};

	geometry.setWidth(qMin(width(), viewportSizeHint().width()));
	geometry.setHeight(qMin(height(), viewportSizeHint().height()));
	m_sizeDialog->move(mapToGlobal(geometry.center()) - m_sizeDialog->rect().center());
	m_sizeLabel->setText(s_sizeFormat.arg(QString::number(m_zoomPourcentage)));
	m_sizeDialog->show();
	m_zoomTimer.start(1000);
}
