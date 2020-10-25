//------------------------------------------------------------------------------
//
// ProgressBarButton.hpp created by Yyhrs 2017
//
//------------------------------------------------------------------------------

#ifndef PROGRESSBARBUTTON_HPP
#define PROGRESSBARBUTTON_HPP

#include <QMouseEvent>
#include <QProgressBar>

class ProgressBarButton: public QProgressBar
{
	Q_OBJECT

public:
	explicit ProgressBarButton(QWidget *parent = nullptr);

	void mousePressEvent(QMouseEvent *);
	void resetProgressBar();

signals:
	void clicked();

private:
	QString m_format;
};

#endif // PROGRESSBARBUTTON_HPP
