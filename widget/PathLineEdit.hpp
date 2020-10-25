//------------------------------------------------------------------------------
//
// PathLineEdit.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef PATHLINEEDIT_HPP
#define PATHLINEEDIT_HPP

#include <QLineEdit>

class PathLineEdit: public QLineEdit
{
	Q_OBJECT

public:
	enum PathType
	{
		File = 0,
		Folder
	};

	explicit PathLineEdit(QWidget *parent = nullptr);

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void setText(const QString &path);
	void openBrowser(PathType type, const QString &filter = QString());
};

#endif // PATHLINEEDIT_HPP
