//------------------------------------------------------------------------------
//
// TabWidget.hpp created by Yyhrs 2019-04-04
//
//------------------------------------------------------------------------------

#ifndef TABWIDGET_HPP
#define TABWIDGET_HPP

#include <QTabWidget>

class TabWidget: public QTabWidget
{
	Q_OBJECT

public:
	explicit TabWidget(QWidget *parent = nullptr);
	~TabWidget() override = default;

	virtual void detachTab(int index);

private:
	Q_DISABLE_COPY(TabWidget)

	void connectWidgets();
};

#endif // TABWIDGET_HPP
