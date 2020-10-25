//------------------------------------------------------------------------------
//
// ProgressBarButton.cpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#include "ProgressBarButton.hpp"

#include <QDebug>

ProgressBarButton::ProgressBarButton(QWidget *parent):
	QProgressBar{parent}
{
	resetProgressBar();
}

void ProgressBarButton::mousePressEvent(QMouseEvent *)
{
	if (m_format.isEmpty())
	{
		m_format = format();
		emit clicked();
	}
}

void ProgressBarButton::resetProgressBar()
{
	setFormat(m_format);
	m_format.clear();
	setValue(0);
}
