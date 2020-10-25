//------------------------------------------------------------------------------
//
// MainWindow.hpp created by Yyhrs 2020-10-23
//
//------------------------------------------------------------------------------

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QFileInfo>
#include <QFutureWatcher>
#include <QMovie>

#include <Settings.hpp>

#include "Kompactor.hpp"

#include "ui_MainWindow.h"

class MainWindow: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	enum ReportColumn
	{
		File,
		OldFileSize,
		NewFileSize,
		MeshName,
		OldVertexNumber,
		NewVertexNumber,
		OldFaceNumber,
		NewFaceNumber,
	};
	Q_ENUM(ReportColumn)
	enum SettingsKey
	{
		InputPath,
		OutputPath,
		VertexPrecision,
		NormalPrecision
	};
	Q_ENUM(SettingsKey)

	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

signals:
	void reportsReceived(const QFileInfo &file, const QList<Kompactor::Report> &reports);

private:
	void connectWidgets();

	QFileInfoList getFiles(const QFileInfo &file) const;

	Settings             m_settings;
	QMovie               m_3263827;
	QFutureWatcher<void> m_watcher;
	QFileInfoList        m_files;
};

#endif // MAINWINDOW_HPP
