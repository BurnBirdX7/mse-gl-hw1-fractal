#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QSlider>
#include <QFormLayout>
#include <QLabel>
#include <QTimer>
#include <deque>

#include "MandelbrotWidget.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();

public slots:
	void setFpsLabel(float);
	void setFrameFrequencyLabel(float);

private:
	QLabel* fpsLabel_{};
	QLabel* freqLabel_{};
};


