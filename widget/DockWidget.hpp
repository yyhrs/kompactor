//------------------------------------------------------------------------------
//
// DockWidget.hpp created by Thill 2018
//
//------------------------------------------------------------------------------

#ifndef DOCKWIDGET_HPP
#define DOCKWIDGET_HPP

#include <QDockWidget>

class DockWidget: public QDockWidget
{
	Q_OBJECT

public:
	explicit DockWidget(QWidget *parent = nullptr);

private:
	void connectWidgets();

	void windowPromote(bool promote = true);
};

#endif // DOCKWIDGET_HPP
