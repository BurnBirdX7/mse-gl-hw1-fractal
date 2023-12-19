#include "MandelbrotWidget.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QScreen>

#include <array>
#include <cmath>

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

void MandelbrotWidget::initializeGL()
{
	initializeOpenGLFunctions();

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


	// Clear all FBO buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start update timer
	if (animate_) {
		updateTimer_ = new QTimer;
		connect(updateTimer_, &QTimer::timeout,
				this, qOverload<>(&MandelbrotWidget::update));
		updateTimer_->start(16);
	}

	// Start frame timer
	frameTimer_ = new QTimer;
	connect(frameTimer_, &QTimer::timeout,
			this, &MandelbrotWidget::fpsTimer);
	frameTimer_->setTimerType(Qt::TimerType::PreciseTimer);
	frameTimer_->start(1000);
}

void MandelbrotWidget::paintGL()
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

	// Update instant FPS counter
	auto time = std::chrono::steady_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(time - lastFrame_);
	auto fps = 1000.f / dur.count();
	emit fpsUpdated(fps);
	lastFrame_ = time;
}

void MandelbrotWidget::mousePressEvent(QMouseEvent * e)
{
	mousePressPosition_ = e->localPos();
	trackMouse_ = true;
}

void MandelbrotWidget::mouseReleaseEvent(QMouseEvent *)
{
	trackMouse_ = false;
}

QVector2D MandelbrotWidget::scaleDiff(QPointF const& point) {
	auto vec = QVector2D(point);
	vec.setX(-vec.x() / static_cast<float>(width()) * 2 * aspectRatio_);
	vec.setY( vec.y() / static_cast<float>(height()) * 2);
	vec *= scale_;
	return vec;
}

void MandelbrotWidget::mouseMoveEvent(QMouseEvent * e)
{
	if (trackMouse_) {
		center_ -= scaleDiff(e->localPos() - mousePressPosition_);
		mousePressPosition_ = e->localPos();
		update();
	}
}

void MandelbrotWidget::wheelEvent(QWheelEvent * e)
{
	constexpr int MIN_SCROLL_ANGLE = 120;
	constexpr float SCALING_FACTOR = 1.1f;

	if (e->phase() == Qt::ScrollBegin || e->phase() == Qt::ScrollEnd) {
		// Ignore OS specific flags
		return;
	}

	static int angle = 0;
	angle += e->angleDelta().y();

	int scroll_amount = angle / MIN_SCROLL_ANGLE;
	if (scroll_amount == 0) { // Not enough angle
		return;
	}

	angle = angle % MIN_SCROLL_ANGLE;
	auto diff = e->position() - windowCenter_;
	center_ += scaleDiff(diff);

	if (scroll_amount > 0) {
		scale_ /= std::pow(SCALING_FACTOR, static_cast<float>(scroll_amount));
	} else {
		scale_ *= std::pow(SCALING_FACTOR, static_cast<float>(-scroll_amount));
	}

	center_ -= scaleDiff(diff);
	update();
}

void MandelbrotWidget::setMaxIterations(int max_iterations)
{
	max_iterations_ = std::clamp(max_iterations, MIN_ITERATIONS, MAX_ITERATIONS);
	update();
}

void MandelbrotWidget::setBorderValue(int border_value)
{
	border_value_ = std::clamp(static_cast<float>(border_value) / 100.f, MIN_BORDER, MAX_BORDER);
	update();
}

void MandelbrotWidget::resizeGL(int w, int h)
{
	aspectRatio_ = static_cast<float>(w) / static_cast<float>(h);
	windowCenter_ = {w / 2.f, h / 2.f};
}

void MandelbrotWidget::fpsTimer() {
	emit timedFpsUpdated(frame_ - framePoint_);
	framePoint_ = frame_;
}

MandelbrotWidget::MandelbrotWidget(bool animate)
	: animate_(animate)
{}
