//
// Created by chris on 12/17/25.
//

#include <QtRendering/GLWidget.hpp>
#include <chrono>
#include <iostream>
#include <qopenglcontext.h>
#include <QMouseEvent>

using std::chrono_literals::operator ""ms;

GLWidget::GLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
	, m_scene(nullptr)  // Don't create yet - no GL context!
	, m_frame_timer(16ms)
{
	// Request OpenGL 4.6 Core Profile
	QSurfaceFormat format;
	format.setVersion(4, 6);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	format.setSwapInterval(1);  // VSync
	setFormat(format);

	// Connect timer to update
	connect(&m_frame_timer, &QChronoTimer::timeout, this, QOverload<>::of(&GLWidget::update));
}


void GLWidget::initializeGL() {
#ifndef __EMSCRIPTEN__
	auto loader = [](const char* name) {
		return reinterpret_cast<void*>(
			QOpenGLContext::currentContext()->getProcAddress(name)
		);
	};

	if (!gladLoadGLLoader(loader)) {
		qFatal("Failed to initialize GLAD");
	}
#endif
	// NOW we can create the renderer - GL context exists!
	m_scene = std::make_unique<Scene>();

	// Start animation
	m_elapsed_timer.start();
	m_frame_timer.start();  // ~60 FPS (1000ms / 60 ≈ 16ms)
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);

	// Update aspect ratio in renderer if needed
	if (m_scene) {
		m_scene->update_aspect_ratio(w, h);
	}
}

void GLWidget::paintGL() {
	if (m_scene) {
		float time = m_elapsed_timer.elapsed() / 1000.0f;
		m_scene->draw(time);
	}
}
void GLWidget::mousePressEvent(QMouseEvent* event)
{
	QOpenGLWidget::mousePressEvent(event);
	auto pos = event->pos();
	m_scene->update_last_mouse_pos({pos.x(), pos.y()});
}
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	QOpenGLWidget::mouseMoveEvent(event);
	auto pos = event->pos();
	m_scene->drag_camera({pos.x(), pos.y()});
}
void GLWidget::wheelEvent(QWheelEvent* event)
{
	QOpenGLWidget::wheelEvent(event);
	m_scene->change_camera_distance(-event->angleDelta().y() / 120.0f);
}

GLWidget::~GLWidget() {
	// Must make context current before destroying GL resources
	makeCurrent();
	m_scene.reset();  // Destroy renderer while context is current
	doneCurrent();
}