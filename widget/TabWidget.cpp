//------------------------------------------------------------------------------
//
// TabWidget.cpp created by Yyhrs 2019-04-04
//
//------------------------------------------------------------------------------

#include <QApplication>
#include <QDialog>
#include <QLayout>
#include <QTabBar>

#include "TabWidget.hpp"

TabWidget::TabWidget(QWidget *parent):
	QTabWidget(parent)
{
	connectWidgets();
}

void TabWidget::detachTab(int index)
{
	auto	*page{widget(index)};
	auto	*dialog{new QDialog{this}};
	auto	*layout{new QHBoxLayout};

	dialog->setWindowFlag(Qt::WindowMaximizeButtonHint);
	dialog->setWindowTitle(tabBar()->tabText(index));
	dialog->setWindowIcon(tabBar()->tabIcon(index));
	layout->addWidget(page);
	dialog->setLayout(layout);
	dialog->resize(page->width(), page->height());
	dialog->show();
	page->setVisible(true);
	if (count() == 0)
		hide();
	connect(dialog, &QDialog::finished, [this, index, page, dialog]
	{
		setCurrentIndex(insertTab(index, page, dialog->windowIcon(), dialog->windowTitle()));
		show();
	});
}

void TabWidget::connectWidgets()
{
	connect(this, &TabWidget::tabCloseRequested, this, &TabWidget::detachTab);
}
