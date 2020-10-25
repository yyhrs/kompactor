//------------------------------------------------------------------------------
//
// main.cpp created by Yyhrs 2020-10-23
//
//------------------------------------------------------------------------------

#include <QApplication>
#include <QProcess>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	MainWindow   window;

	window.show();
	return QApplication::exec();
}
