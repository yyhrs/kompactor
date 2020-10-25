//------------------------------------------------------------------------------
//
// LogWidget.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef LOGWIDGET_HPP
#define LOGWIDGET_HPP

#include <QDir>
#include <QMetaEnum>

#include "TableModel.hpp"
#include "TableView.hpp"

class LogWidget: public TableView
{
	Q_OBJECT

public:
	enum LogType
	{
		Information = 0,
		Warning,
		Critical
	};
	Q_ENUM(LogType)
	enum Column
	{
		Time = 0,
		Type,
		Log,
		ColumnCount
	};
	Q_ENUM(Column)

	template<typename Value, typename T>
	using Return = typename QtPrivate::QEnableIf<QtPrivate::IsQEnumHelper<Value>::Value, T>::Type;

	explicit LogWidget(QWidget *parent = nullptr);
	~LogWidget() = default;

	void	setColor(const QColor color, LogType type);
	void	setAutoFlush(int interval, const QDir &destination = {});

	void	addLog(LogType type, const QVariant &log, const QVariantList &description = {});
	void	addLogs(LogType type, const QVariant &log, const QList<QVariantList> &descriptions = {});
	QString flush(const QDir &destination = {});
	void	save(const QFileInfo fileInfo);

	template<typename Header> Return<Header, void>	addColumns(const QList<Header> &headers);

private:
	TableModel				m_logModel;
	QMetaEnum				m_metaEnum;
	QMap<LogType, QColor>	m_colors;
	QTimer					m_timer;
};

#include "LogWidget.tpp"

#endif // LOGWIDGET_HPP
