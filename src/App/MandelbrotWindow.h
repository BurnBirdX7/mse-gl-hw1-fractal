#pragma once

#include <Base/GLWindow.hpp>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

#include <memory>

class MandelbrotWindow final : public fgl::GLWindow
{

public:
	void init() override;
	void render() override;

protected:
	void mousePressEvent(QMouseEvent * e) override;
	void mouseReleaseEvent(QMouseEvent * e) override;
	void mouseMoveEvent(QMouseEvent * e) override;
	void resizeEvent(QResizeEvent * e) override;
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

	int border_value_ = 2;
	int max_iteration_ = 100;
	float scale_ = 1.5f;
	QVector2D center_ = {0.0, 0.0};
	float aspectRatio_ = 1.0f;

	// Tracking params:
	bool trackMouse_ = false;
	QPointF mousePressPosition_{0.f, 0.f};
	QPointF windowCenter_ = {0.f, 0.f};
};
