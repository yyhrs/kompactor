//------------------------------------------------------------------------------
//
// DockWidget.cpp created by Thill 2018
//
//------------------------------------------------------------------------------

#include <QBoxLayout>

#include "DockWidget.hpp"

DockWidget::DockWidget(QWidget *parent): QDockWidget{parent}
{
	windowPromote(isWindow());
	connectWidgets();
}

void DockWidget::connectWidgets()
{
	connect(this, &QDockWidget::topLevelChanged, this, &DockWidget::windowPromote);
	connect(this, &QDockWidget::dockLocationChanged, [this](Qt::DockWidgetArea area)
	{
		QList<QSpacerItem *> spacers;

		for (int index = 0; index < widget()->layout()->count(); ++index)
		{
			auto *item{dynamic_cast<QSpacerItem *>(widget()->layout()->itemAt(index))};

			if (item)
				spacers << item;
		}
		setFeatures(features().setFlag(QDockWidget::DockWidgetVerticalTitleBar, area != Qt::TopDockWidgetArea && area != Qt::BottomDockWidgetArea));
		if (area == Qt::TopDockWidgetArea || area == Qt::BottomDockWidgetArea)
		{
			static_cast<QBoxLayout *>(widget()->layout())->setDirection(QBoxLayout::LeftToRight);
			for (auto *spacer: spacers)
				spacer->changeSize(spacer->sizeHint().width(), spacer->sizeHint().height(), QSizePolicy::Expanding, QSizePolicy::Minimum);
		}
		else
		{
			static_cast<QBoxLayout *>(widget()->layout())->setDirection(QBoxLayout::TopToBottom);
			for (auto *spacer: spacers)
				spacer->changeSize(spacer->sizeHint().width(), spacer->sizeHint().height(), QSizePolicy::Minimum, QSizePolicy::Expanding);
		}
	});
}

void DockWidget::windowPromote(bool promote)
{
	if (promote)
	{
		setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
		setWindowFlags(Qt::Window);
		setVisible(true);
	}
}
