#include <QApplication>
#include <QSurfaceFormat>

#include "MandelbrotWindow.h"
#include "SettingsWindow.h"

namespace
{
constexpr auto g_sampels = 16;
constexpr auto g_gl_major_version = 3;
constexpr auto g_gl_minor_version = 3;
}// namespace

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setSamples(g_sampels);
	format.setVersion(g_gl_major_version, g_gl_minor_version);
	format.setProfile(QSurfaceFormat::CoreProfile);

	MandelbrotWindow mandelbrotWindow;
	mandelbrotWindow.setFormat(format);
	mandelbrotWindow.resize(640, 480);

	mandelbrotWindow.setAnimated(true);

	SettingsWindow settingsWindow;

	QObject::connect(&settingsWindow, &SettingsWindow::maxIterationsChanged,
					 &mandelbrotWindow, &MandelbrotWindow::setMaxIterations);
	QObject::connect(&settingsWindow, &SettingsWindow::borderValueChanged,
					 &mandelbrotWindow, &MandelbrotWindow::setBorderValue);

	mandelbrotWindow.show();
	settingsWindow.show();

	return app.exec();
}