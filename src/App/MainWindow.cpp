#include "MainWindow.h"
#include "MandelbrotWidget.h"

#include <QDockWidget>

using namespace std::chrono_literals;

QSlider* createIterSlider() {
	auto* slider = new QSlider(Qt::Horizontal);

	slider->setMinimum(MandelbrotWidget::MIN_ITERATIONS);
	slider->setMaximum(MandelbrotWidget::MAX_ITERATIONS);
	slider->setValue(MandelbrotWidget::DEFAULT_ITERATIONS);
	return slider;
}

QSlider* createBorderSlider() {
	auto slider = new QSlider(Qt::Horizontal);

	slider->setMinimum(MandelbrotWidget::MIN_BORDER * 100);
	slider->setMaximum(MandelbrotWidget::MAX_BORDER * 100);
	slider->setValue(MandelbrotWidget::DEFAULT_BORDER * 100);
	return slider;
}

QLabel* createFpsLabel() {
	auto label = new QLabel();
	label->setText("<...>");
	return label;
}

MandelbrotWidget * createMandelbrotWidget() {
	return new MandelbrotWidget;
}

MainWindow::MainWindow()
{
	auto iterationSlider = createIterSlider();
	auto borderSlider = createBorderSlider();
	fpsLabel_ = createFpsLabel();
	freqLabel_ = createFpsLabel();

	auto formLayout = new QFormLayout();

	formLayout->addRow("Max iterations: ", iterationSlider);
	formLayout->addRow("Border value:", borderSlider);
	formLayout->addRow("FPS last second:", fpsLabel_);
	formLayout->addRow("FPS last frame :", freqLabel_);

	auto mandelbrotWidget = createMandelbrotWidget();

	connect(iterationSlider, &QSlider::valueChanged,
			mandelbrotWidget, &MandelbrotWidget::setMaxIterations);

	connect(borderSlider, &QSlider::valueChanged,
			mandelbrotWidget, &MandelbrotWidget::setBorderValue);

	connect(mandelbrotWidget, &MandelbrotWidget::fpsUpdated,
			this, &MainWindow::setFrameFrequencyLabel);

	connect(mandelbrotWidget, &MandelbrotWidget::timedFpsUpdated,
			this, &MainWindow::setFpsLabel);

	setCentralWidget(mandelbrotWidget);

	auto dock = new QDockWidget;
	auto settingsWidget = new QWidget;
	settingsWidget->setLayout(formLayout);
	dock->setWidget(settingsWidget);

	this->addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::setFpsLabel(float num)
{
	fpsLabel_->setText(QString::number(num, 'g', 2));
}

void MainWindow::setFrameFrequencyLabel(float num)
{
	freqLabel_->setText(QString::number(num, 'g', 2));
}
