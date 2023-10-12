#include "MandelbrotWindow.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QScreen>

#include <array>

namespace
{

constexpr std::array<GLfloat, 16u> vertices = {
	 1.0f,  1.0f, // 0. top-right
	 1.0f, -1.0f, // 1. bottom-right
	-1.0f, -1.0f, // 2. bottom-left
	-1.0f,  1.0f, // 3. top-left
};

constexpr std::array<GLuint, 4u> indices = {
	0, 1, 3, 2
};

}// namespace

void MandelbrotWindow::init()
{
	// Configure shaders
	program_ = std::make_unique<QOpenGLShaderProgram>(this);
	program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/mandelbrot.vs");
	program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
									  ":/Shaders/mandelbrot.fs");
	program_->link();

	// Create VAO object
	vao_.create();
	vao_.bind();

	// Create VBO
	vbo_.create();
	vbo_.bind();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(GLfloat)));

	// Create IBO
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));

	// Bind attributes
	program_->bind();

	int stride = static_cast<int>(2 * sizeof(GLfloat));

	program_->enableAttributeArray(0);
	program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, stride);

	borderUniform_ = program_->uniformLocation("border_value");
	maxIterationUniform_ = program_->uniformLocation("max_iterations");
	scaleUniform_ = program_->uniformLocation("scale");
	centerUniform_ = program_->uniformLocation("center");
	aspectUniform_ = program_->uniformLocation("aspect");

	// Release all
	program_->release();

	vao_.release();

	ibo_.release();
	vbo_.release();

	// Uncomment to enable depth test and face culling
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);

	// Clear all FBO buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start frame timer
	frameTime_ = std::chrono::steady_clock::now();
}

void MandelbrotWindow::render()
{
	// Configure viewport
	const auto retinaScale = devicePixelRatio();
	glViewport(0, 0, static_cast<GLint>(width() * retinaScale),
			   static_cast<GLint>(height() * retinaScale));

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind VAO and shader program
	program_->bind();
	vao_.bind();

	// Update uniform values
	program_->setUniformValue(borderUniform_, border_value_);
	program_->setUniformValue(maxIterationUniform_, max_iterations_);
	program_->setUniformValue(scaleUniform_,  scale_);
	program_->setUniformValue(centerUniform_, center_);
	program_->setUniformValue(aspectUniform_, aspectRatio_);

	// Draw
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

	// Release VAO and shader program
	vao_.release();
	program_->release();

	// Increment frame counter
	++frame_;

	// Update FPS counter
	auto time = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(time - frameTime_);
	auto fps = 1000.f / dur.count();
	emit fpsUpdated(fps);
	frameTime_ = time;
}

void MandelbrotWindow::mousePressEvent(QMouseEvent * e)
{
	mousePressPosition_ = e->localPos();
	trackMouse_ = true;
}

void MandelbrotWindow::mouseReleaseEvent(QMouseEvent *)
{
	trackMouse_ = false;
}

QVector2D MandelbrotWindow::scaleDiff(QPointF const& point) {
	auto vec = QVector2D(point);
	vec.setX(-vec.x() / static_cast<float>(width()) * 2 * aspectRatio_);
	vec.setY( vec.y() / static_cast<float>(height()) * 2);
	vec *= scale_;
	return vec;
}

void MandelbrotWindow::mouseMoveEvent(QMouseEvent * e)
{
	if (trackMouse_) {
		center_ -= scaleDiff(e->localPos() - mousePressPosition_);
		mousePressPosition_ = e->localPos();
	}
}

void MandelbrotWindow::resizeEvent(QResizeEvent * e)
{
	auto const& s = e->size();
	aspectRatio_ = static_cast<float>(s.width()) / static_cast<float>(s.height());
	windowCenter_ = {s.width() / 2.f, s.height() / 2.f};
}
void MandelbrotWindow::wheelEvent(QWheelEvent * e)
{
	if (e->phase() == Qt::ScrollBegin || e->phase() == Qt::ScrollEnd) {
		return;
	}

	float angle = static_cast<float>(e->angleDelta().y());
	if (angle == 0.0f) { // To avoid division by zero
		return;
	}

	auto diff = e->position() - windowCenter_;
	center_ += scaleDiff(diff);

	constexpr float SCALING_FACTOR = 1.2f;

	auto mult = angle / 120;


	if (angle > 0) {
		scale_ /= SCALING_FACTOR * mult;
	} else {
		scale_ *= -SCALING_FACTOR * mult;
	}

	center_ -= scaleDiff(diff);
}
void MandelbrotWindow::setMaxIterations(int max_iterations)
{
	max_iterations_ = std::clamp(max_iterations, MIN_ITERATIONS, MAX_ITERATIONS);
}

void MandelbrotWindow::setBorderValue(int border_value)
{
	border_value_ = std::clamp(static_cast<float>(border_value) / 100.f, MIN_BORDER, MAX_BORDER);
}
