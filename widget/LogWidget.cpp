//------------------------------------------------------------------------------
//
// LogWidget.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include <QAction>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QHeaderView>

#include "LogWidget.hpp"

LogWidget::LogWidget(QWidget *parent):
	TableView{parent},
	m_metaEnum{QMetaEnum::fromType<LogType>()},
	m_colors{{Information, {61, 174, 233}}, {Warning, {237, 193, 30}}, {Critical, {218, 68, 83}}}
{
	auto	*clearLogs{new QAction{QIcon::fromTheme("x"), "Clear Logs", this}};
	auto	*exportLogs{new QAction{QIcon::fromTheme("file"), "Export Logs", this}};

	m_logModel.addColumns(QList<Column>{Time, Type, Log});
	horizontalHeader()->setStretchLastSection(true);
	setModel(&m_logModel);
	addActions({exportLogs, clearLogs});
	connect(clearLogs, &QAction::triggered, &m_logModel, &TableModel::clear);
	connect(exportLogs, &QAction::triggered, [this]
	{
		save(QFileDialog::getSaveFileName(this, "Export Logs", {}, "Tab-separated values (*.tsv)"));
	});
}

void LogWidget::setColor(const QColor color, LogWidget::LogType type)
{
	m_colors[type] = color;
}

void LogWidget::setAutoFlush(int interval, const QDir &destination)
{
	m_timer.start(interval * 60 * 1000);
	connect(&m_timer, &QTimer::timeout, [this, destination]
	{
		flush(destination);
	});
}

void LogWidget::addLog(LogWidget::LogType type, const QVariant &log, const QVariantList &description)
{
	addLogs(type, log, {description});
}

void LogWidget::addLogs(LogWidget::LogType type, const QVariant &log, const QList<QVariantList> &descriptions)
{
	QList<TableModel::Row>	rows;

	for (const auto &description: descriptions)
	{
		TableModel::Row row{m_logModel.columnCount()};
		int				column{0};

		row[Time][Qt::DisplayRole][TableModel::s_universal] = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
		row[Type][Qt::DisplayRole][TableModel::s_universal] = m_metaEnum.valueToKey(type);
		row[Type][Qt::BackgroundRole][TableModel::s_universal] = m_colors.value(type);
		row[Log][Qt::DisplayRole][TableModel::s_universal] = log.toString();
		while (column + ColumnCount < m_logModel.columnCount())
		{
			row[column + ColumnCount][Qt::DisplayRole][TableModel::s_universal] = description.value(column).toString();
			++column;
		}
		rows << row;
	}
	m_logModel.addRows(rows);
	resizeColumnToContents(1);
	scrollToBottom();
}

QString LogWidget::flush(const QDir &destination)
{
	QString path{destination.filePath(QDate::currentDate().toString(Qt::ISODate) + ".log")};

	save(path);
	m_logModel.clear();
	return path;
}

void LogWidget::save(const QFileInfo fileInfo)
{
	QFile	file{fileInfo.absoluteFilePath()};

	if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text))
	{
		for (int row = 0; row < proxyModel().rowCount(); ++row)
		{
			QStringList items;

			for (int column = 0; column < proxyModel().columnCount(); ++column)
				items << proxyModel().index(row, column).data().toString();
			file.write(items.join(QChar::Tabulation).append(QChar::CarriageReturn).toUtf8());
		}
		file.close();
	}
}
