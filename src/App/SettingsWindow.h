#pragma once


#include <QWidget>
#include <QSlider>
#include <QFormLayout>
#include <QLabel>
#include "MandelbrotWindow.h"


class SettingsWindow : public QWidget
{
	Q_OBJECT
public:
	SettingsWindow();

public slots:
	void setFpsLabel(float);

signals:
	void maxIterationsChanged(int newValue);
	void borderValueChanged(int newValue);

private:
	QSlider * iterationSlider_{};
	QSlider * borderSlider_{};
	QLabel * fpsLabel_{};
	QFormLayout* layout_{};

};


