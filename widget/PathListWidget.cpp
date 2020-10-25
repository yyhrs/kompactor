//------------------------------------------------------------------------------
//
// PathListWidget.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QDrag>
#include <QDropEvent>
#include <QFileSystemModel>
#include <QMimeData>
#include <QPainter>

#include "PathListWidget.hpp"

PathListWidget::PathListWidget(QWidget *parent): QListWidget{parent}
{
	setIconSize({16, 16});
	m_actions[Add] = new QAction{QIcon::fromTheme("add"), "Add", this};
	m_actions[Remove] = new QAction{QIcon::fromTheme("remove"), "Remove", this};
	addActions(m_actions.values());
	connect(m_actions[Remove], &QAction::triggered, [this]
	{
		auto		indexes{selectionModel()->selectedRows()};
		QList<int>	rows;

		for (const auto &index: indexes)
			rows << index.row();
		std::sort(rows.begin(), rows.end(), std::greater<int>());
		for (const auto &row: rows)
			model()->removeRow(row);
		emit listModified();
	});
}

const QMap<PathListWidget::Action, QAction *> &PathListWidget::actions() const
{
	return m_actions;
}

void PathListWidget::setBehaviour(Behaviour behaviour)
{
	m_behaviour = behaviour;
}
void PathListWidget::addItem(const QString &path)
{
	QFileSystemModel	model;
	auto				addPath{[this, &model](const QString &path)
		{
			int	row{count()};

			QListWidget::addItem(QDir::toNativeSeparators(path));
			item(row)->setIcon(model.fileIcon(model.index(path)));
			item(row)->setToolTip(QDir::toNativeSeparators(path));
		}};

	if (m_behaviour != AllEntries && QFileInfo{path}.isDir())
	{
		for (auto &file: QDir{path}.entryInfoList(QDir::Files))
			addPath(file.absoluteFilePath());
		if (m_behaviour == RecursiveFiles)
		{
			for (auto &file: QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
				addPath(file.absoluteFilePath());
			return ;
		}
	}
	else
		addPath(path);
	emit listModified();
}

QMimeData *PathListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
	auto		*mimeData{new QMimeData};
	QList<QUrl> urls;

	for (const auto &item: items)
		urls << QUrl::fromLocalFile(item->text());
	mimeData->setUrls(urls);
	return mimeData;
}

void PathListWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}
void PathListWidget::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}
void PathListWidget::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasUrls())
	{
		for (auto const &file: event->mimeData()->urls())
			addItem(file.toLocalFile());
		emit fileDropped();
	}
}
