#pragma once


#include <QWidget>
#include <QSlider>
#include <QFormLayout>
#include "MandelbrotWindow.h"


class SettingsWindow : public QWidget
{
	Q_OBJECT
public:
	SettingsWindow();

signals:
	void maxIterationsChanged(int newValue);
	void borderValueChanged(int newValue);

private:
	QSlider * iterationSlider_{};
	QSlider * borderSlider_{};
	QFormLayout* layout_{};

};


