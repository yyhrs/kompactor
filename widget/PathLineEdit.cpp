//------------------------------------------------------------------------------
//
// PathLineEdit.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include <QDir>
#include <QDropEvent>
#include <QFileDialog>
#include <QMimeData>

#include "PathLineEdit.hpp"

PathLineEdit::PathLineEdit(QWidget *parent): QLineEdit(parent)
{
}

void PathLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void PathLineEdit::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasUrls())
		setText(event->mimeData()->urls()[0].toLocalFile());
}

void PathLineEdit::setText(const QString &path)
{
	QLineEdit::setText(QDir::toNativeSeparators(path));
}

void PathLineEdit::openBrowser(PathLineEdit::PathType type, const QString &filter)
{
	QString path;

	if (type == Folder)
		path = QFileDialog::getExistingDirectory(this, {}, text());
	else if (type == File)
		path = QFileDialog::getOpenFileName(this, {}, text(), filter);
	if (!path.isEmpty())
		setText(path);
}
