#include "SettingsWindow.h"

QSlider* createIterSlider() {
	auto* slider = new QSlider(Qt::Horizontal);

	slider->setMinimum(MandelbrotWindow::MIN_ITERATIONS);
	slider->setMaximum(MandelbrotWindow::MAX_ITERATIONS);
	slider->setValue(MandelbrotWindow::DEFAULT_ITERATIONS);
	return slider;
}

QSlider* createBorderSlider() {
	auto slider = new QSlider(Qt::Horizontal);

	slider->setMinimum(MandelbrotWindow::MIN_BORDER * 100);
	slider->setMaximum(MandelbrotWindow::MAX_BORDER * 100);
	slider->setValue(MandelbrotWindow::DEFAULT_BORDER * 100);
	return slider;
}

QLabel* createFpsLabel() {
	auto label = new QLabel();
	label->setText("<...>");
	return label;
}

SettingsWindow::SettingsWindow()
{
	iterationSlider_ = createIterSlider();
	borderSlider_ = createBorderSlider();
	fpsLabel_ = createFpsLabel();

	layout_ = new QFormLayout(this);

	layout_->addRow("Max iterations: ", iterationSlider_);
	layout_->addRow("Border value:", borderSlider_);
	layout_->addRow("FPS:", fpsLabel_);

	connect(iterationSlider_, &QSlider::valueChanged,
			this, &SettingsWindow::maxIterationsChanged);

	connect(borderSlider_, &QSlider::valueChanged,
			this, &SettingsWindow::borderValueChanged);
}
void SettingsWindow::setFpsLabel(float num)
{
	fpsLabel_->setText(QString::number(num, 'g', 2));
}
