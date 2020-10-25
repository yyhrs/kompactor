//------------------------------------------------------------------------------
//
// MainWindow.cpp created by Yyhrs 2020-10-23
//
//------------------------------------------------------------------------------

#include <QDebug>
#include <QStyleFactory>
#include <QtConcurrent>

#include <operators.tpp>

#include "Kompactor.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent):
	QMainWindow{parent},
	m_3263827{":/3263827.gif"}
{
	QList<QPair<QString, int>> precisions{
		{"FULL", -1},
		{"9", 9},
		{"8", 8},
		{"7", 7},
		{"6", 6},
		{"5", 5},
		{"4", 4},
		{"3", 3},
		{"2", 2},
		{"1", 1},
		{"GGGWARRRHHWWWW", 0}};

	qApp->setStyle(QStyleFactory::create("fusion"));
	setupUi(this);
	for (const auto &precision: precisions)
	{
		vertexPrecisionComboBox->addItem(precision.first, precision.second);
		normalPrecisionComboBox->addItem(precision.first, precision.second);
	}
	movieLabel->setMovie(&m_3263827);
	reportWidget->addColumns(QList{File, OldFileSize, NewFileSize, MeshName, OldVertexNumber, NewVertexNumber, OldFaceNumber, NewFaceNumber});
	reportWidget->hideColumn(LogWidget::Time);
	reportWidget->hideColumn(LogWidget::Log);
	connectWidgets();
	m_settings.restoreState(this);
	inputPathLineEdit->setText(m_settings.value(InputPath).toString());
	outputPathLineEdit->setText(m_settings.value(OutputPath).toString());
	vertexPrecisionComboBox->setCurrentIndex(m_settings.value(VertexPrecision, vertexPrecisionComboBox->currentIndex()).toInt());
	normalPrecisionComboBox->setCurrentIndex(m_settings.value(NormalPrecision, normalPrecisionComboBox->currentIndex()).toInt());
	m_settings.restoreValues(Shaders, listWidget, {"MeshShadowVolume.fxo"});
}

MainWindow::~MainWindow()
{
	m_settings.saveState(this);
	m_settings.setValue(InputPath, inputPathLineEdit->text());
	m_settings.setValue(OutputPath, outputPathLineEdit->text());
	m_settings.setValue(VertexPrecision, vertexPrecisionComboBox->currentIndex());
	m_settings.setValue(NormalPrecision, normalPrecisionComboBox->currentIndex());
	m_settings.saveValues(Shaders, listWidget);
}

void MainWindow::connectWidgets()
{
	connect(inputPushButton, &QPushButton::clicked, [this]
	{
		inputPathLineEdit->openBrowser(PathLineEdit::Folder);
	});
	connect(inputPathLineEdit, &PathLineEdit::textChanged, [this](const QString &text)
	{
		progressBar->setDisabled(text.isEmpty() || QFileInfo{text} == QFileInfo{outputPathLineEdit->text()});
	});
	connect(outputPushButton, &QPushButton::clicked, [this]
	{
		outputPathLineEdit->openBrowser(PathLineEdit::Folder);
	});
	connect(outputPathLineEdit, &PathLineEdit::textChanged, [this](const QString &text)
	{
		progressBar->setDisabled(text.isEmpty() || QFileInfo{text} == QFileInfo{inputPathLineEdit->text()});
	});
	connect(progressBar, &ProgressBarButton::clicked, [this]
	{
		QStringList shaders;

		for (int index = 0; index < listWidget->count(); ++index)
		{
			QFileInfo shader{listWidget->item(index)->text()};

			if (shader.suffix() == "fxo")
				shaders << shader.baseName() + ".fx";
		}
		settingsGroupBox->setDisabled(true);
		stackedWidget->setCurrentIndex(0);
		m_3263827.start();
		reportWidget->flush();
		m_files = getFiles(inputPathLineEdit->text());
		QDir{}.mkpath(QFileInfo{outputPathLineEdit->text()}.absoluteFilePath());
		qDebug() << m_files;
		m_watcher.setFuture(QtConcurrent::map(m_files, [this, shaders](const QFileInfo &file)
		{
			Kompactor kompactor{file, outputPathLineEdit->text() / file.fileName(), vertexPrecisionComboBox->currentData().toInt(), normalPrecisionComboBox->currentData().toInt(), shaders};

			emit reportsReceived(file, kompactor.excute());
		}));

	});
	connect(&m_watcher, &QFutureWatcherBase::progressRangeChanged, progressBar, &ProgressBarButton::setRange);
	connect(&m_watcher, &QFutureWatcherBase::progressValueChanged, progressBar, &ProgressBarButton::setValue);
	connect(&m_watcher, &QFutureWatcherBase::finished, [this]
	{
		settingsGroupBox->setDisabled(false);
		stackedWidget->setCurrentIndex(1);
		m_3263827.stop();
		progressBar->resetProgressBar();
	});
	connect(this, &MainWindow::reportsReceived, [this](const QFileInfo &file, const QList<Kompactor::Report> &reports)
	{
		const QFileInfo     newFile{outputPathLineEdit->text() / file.fileName()};
		QList<QVariantList> descriptions;

		for (const auto &report: reports)
		{
			QVariantList description;

			description << file.baseName()
						<< file.size() / 1000000.
						<< newFile.size() / 1000000.
						<< report.name
						<< report.oldVerticesNumber
						<< report.newVerticesNumber
						<< report.oldFaceNumber
						<< report.newFaceNumber;
			if (report.name == file.fileName())
				reportWidget->addLog(LogWidget::Warning, file.baseName(), description);
			else
				descriptions << description;
		}
		reportWidget->addLogs(LogWidget::Information, file.baseName(), descriptions);
	});
}

QFileInfoList MainWindow::getFiles(const QFileInfo &file) const
{
	QFileInfoList files;

	if (file.isDir())
	{
		static const QStringList filters{"*.alo"};
		QDir                     dir{file.absoluteFilePath()};

		for (const auto &fileInfo: dir.entryInfoList(filters, QDir::Files))
			files << fileInfo;
//		for (const auto &fileInfo: dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
//			files << getFiles(fileInfo);
	}
	else
		files << file;
	return files;
}
