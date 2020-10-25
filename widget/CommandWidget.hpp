//------------------------------------------------------------------------------
//
// CommandWidget.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef COMMANDWIDGET_HPP
#define COMMANDWIDGET_HPP

#include <QPlainTextEdit>
#include <QProcess>

class CommandWidget: public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CommandWidget(QWidget *parent = nullptr);

	void	startProcess(const QString &program, const QStringList &arguments, QIODevice::OpenMode mode = QIODevice::ReadWrite);
	void	waitProcessForFinished(int msecs = 30000);

signals:
    void	started();
    void	finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess m_process;
};

#endif // COMMANDWIDGET_HPP
