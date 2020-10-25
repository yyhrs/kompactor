//------------------------------------------------------------------------------
//
// CommandWidget.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include "CommandWidget.hpp"

CommandWidget::CommandWidget(QWidget *parent):
	QPlainTextEdit{parent}
{
}

void CommandWidget::startProcess(const QString &program, const QStringList &arguments, QIODevice::OpenMode mode)
{
	m_process.start(program, arguments, mode);
	appendHtml("<font color = 'yellow'>" + m_process.program() + " " + m_process.arguments().join(' ') + "</font>");
	connect(&m_process, &QProcess::readyReadStandardOutput, [this]
	{
		appendHtml("<font color = 'green'>" + QString{m_process.readAllStandardOutput()}.replace("\n", "<br/>") + "</font>");
	});
	connect(&m_process, &QProcess::readyReadStandardError, [this]
	{
		appendHtml("<font color = 'red'>" + QString{m_process.readAllStandardOutput()}.replace("\n", "<br/>") + "</font>");
	});
	connect(&m_process, &QProcess::started, this, &CommandWidget::started);
	connect(&m_process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &CommandWidget::finished);
}

void CommandWidget::waitProcessForFinished(int msecs)
{
	m_process.waitForFinished(msecs);
}
