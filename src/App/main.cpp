#include <QApplication>
#include <QSurfaceFormat>

#include <QMainWindow>
#include <QDockWidget>


#include "MainWindow.h"
#include "MandelbrotWidget.h"


int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	auto* window = new MainWindow;
	window->resize(640, 480);
	window->show();

	return app.exec();
}