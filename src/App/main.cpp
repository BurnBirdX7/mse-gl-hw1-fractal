#include <QApplication>
#include <QSurfaceFormat>

#include <QMainWindow>
#include <QDockWidget>


#include "MainWindow.h"
#include "MandelbrotWidget.h"

constexpr auto G_SAMPLES = 16;
constexpr auto G_MAJOR = 3;
constexpr auto G_MINOR = 3;

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setSamples(G_SAMPLES);
	format.setVersion(G_MAJOR, G_MINOR);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	auto* window = new MainWindow();
	window->resize(640, 480);
	window->show();

	return app.exec();
}