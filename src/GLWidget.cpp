//
// Created by chris on 12/17/25.
//
#include <chrono>
#include <glad/glad.h>
#include <iostream>
#include <qopenglcontext.h>
#include <QtRendering/GLWidget.hpp>
using std::chrono_literals::operator ""ms;

GLWidget::GLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
	, m_renderer(nullptr)  // Don't create yet - no GL context!
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
	auto loader = [](const char* name) {
		return reinterpret_cast<void*>(
			QOpenGLContext::currentContext()->getProcAddress(name)
		);
	};

	if (!gladLoadGLLoader(loader)) {
		qFatal("Failed to initialize GLAD");
	}
	// NOW we can create the renderer - GL context exists!
	m_renderer = std::make_unique<Renderer>();

	// Start animation
	m_elapsed_timer.start();
	m_frame_timer.start();  // ~60 FPS (1000ms / 60 ≈ 16ms)
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);

	// Update aspect ratio in renderer if needed
	if (m_renderer) {
		m_renderer->update_aspect_ratio(w, h);
	}
}

void GLWidget::paintGL() {
	if (m_renderer) {
		float time = m_elapsed_timer.elapsed() / 1000.0f;
		m_renderer->draw(time);
	}
}

GLWidget::~GLWidget() {
	// Must make context current before destroying GL resources
	makeCurrent();
	m_renderer.reset();  // Destroy renderer while context is current
	doneCurrent();
}