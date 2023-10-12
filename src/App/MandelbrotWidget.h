#pragma once

#include <chrono>

#include <Base/GLWindow.hpp>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector2D>
#include <QTimer>

#include <memory>

class MandelbrotWidget final
	: public QOpenGLWidget,
	  protected QOpenGLFunctions
{
	Q_OBJECT

public:
	constexpr static int MIN_ITERATIONS = 50;
	constexpr static int MAX_ITERATIONS = 500;
	constexpr static int DEFAULT_ITERATIONS = 100;

	constexpr static float MIN_BORDER = 0.1f;
	constexpr static float MAX_BORDER = 4.0f;
	constexpr static float DEFAULT_BORDER = 2.0f;

public:
	MandelbrotWidget(bool animate = false);

	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

signals:
	void fpsUpdated(float);
	void timedFpsUpdated(float);

public slots:
	void setMaxIterations(int max_iterations);
	void setBorderValue(int border_value);

protected slots:
	void fpsTimer();

protected:
	void mousePressEvent(QMouseEvent * e) override;
	void mouseReleaseEvent(QMouseEvent * e) override;
	void mouseMoveEvent(QMouseEvent * e) override;
	void wheelEvent(QWheelEvent * e) override;

private:
	QVector2D scaleDiff(QPointF const& point);

private:
	// OpenGL:
	GLint borderUniform_ = -1;
	GLint maxIterationUniform_ = -1;
	GLint scaleUniform_ = -1;
	GLint centerUniform_ = -1;
	GLint aspectUniform_ = -1;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

	size_t frame_ = 0;

	// Fractal params:

	float border_value_ = DEFAULT_BORDER;
	int max_iterations_ = DEFAULT_ITERATIONS;
	float scale_ = 1.0f;
	QVector2D center_ = {0.0, 0.0};
	float aspectRatio_ = 1.0f;

	// Tracking params:
	bool trackMouse_ = false;
	QPointF mousePressPosition_{0.0f, 0.0f};
	QPointF windowCenter_ = {0.0f, 0.0f};

	QTimer* updateTimer_ {};
	QTimer* frameTimer_ {};
	std::chrono::steady_clock::time_point lastFrame_;
	size_t framePoint_ = 0;
	bool animate_ = false;
};
