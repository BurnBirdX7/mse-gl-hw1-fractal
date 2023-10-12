#include "SettingsWindow.h"

SettingsWindow::SettingsWindow()
{
	iterationSlider_ = new QSlider(Qt::Horizontal);
	if (!iterationSlider_) {
		return;
	}

	iterationSlider_->setMinimum(MandelbrotWindow::MIN_ITERATIONS);
	iterationSlider_->setMaximum(MandelbrotWindow::MAX_ITERATIONS);
	iterationSlider_->setValue(MandelbrotWindow::DEFAULT_ITERATIONS);


	borderSlider_ = new QSlider(Qt::Horizontal);
	if (!borderSlider_) {
		delete iterationSlider_;
		return;
	}

	borderSlider_->setMinimum(MandelbrotWindow::MIN_BORDER * 100);
	borderSlider_->setMaximum(MandelbrotWindow::MAX_BORDER * 100);
	borderSlider_->setValue(MandelbrotWindow::DEFAULT_BORDER * 100);


	layout_ = new QFormLayout(this);
	if (!layout_) {
		delete borderSlider_;
		delete iterationSlider_;
		return;
	}

	layout_->addRow("Max iterations: ", iterationSlider_);
	layout_->addRow("Border value:", borderSlider_);

	connect(iterationSlider_, &QSlider::valueChanged,
			this, &SettingsWindow::maxIterationsChanged);

	connect(borderSlider_, &QSlider::valueChanged,
			this, &SettingsWindow::borderValueChanged);
}


