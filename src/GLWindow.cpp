#include <chrono>
#include <QMouseEvent>
#include <QtRendering/GLWindow.hpp>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#include <qopenglcontext.h>
#endif

using std::chrono_literals::operator""ms;

GLWindow::GLWindow(QWindow* parent)
	: QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
	, m_scene(nullptr)
	, m_frame_timer(16ms)
{
	connect(&m_frame_timer, &QChronoTimer::timeout, this, QOverload<>::of(&GLWindow::update));
}

void GLWindow::initializeGL()
{
#ifndef __EMSCRIPTEN__
	auto loader = [](const char* name)
	{ return reinterpret_cast<void*>(QOpenGLContext::currentContext()->getProcAddress(name)); };

	if (!gladLoadGLLoader(loader))
	{
		qFatal("Failed to initialize GLAD");
	}
#endif
	m_scene = std::make_unique<Scene>();
	m_elapsed_timer.start();
	m_frame_timer.start();

	emit initialized();
}

void GLWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	m_scene->update_aspect_ratio(w, h);
}

void GLWindow::paintGL()
{
	float time = m_elapsed_timer.elapsed() / 1000.0f;
	m_scene->draw(time);
}

void GLWindow::mousePressEvent(QMouseEvent* event)
{
	QOpenGLWindow::mousePressEvent(event);
	auto pos = event->pos();
	m_scene->update_last_mouse_pos({pos.x(), pos.y()});
}

void GLWindow::mouseMoveEvent(QMouseEvent* event)
{
	QOpenGLWindow::mouseMoveEvent(event);
	auto pos = event->pos();
	m_scene->drag_camera({pos.x(), pos.y()});
}

void GLWindow::wheelEvent(QWheelEvent* event)
{
	QOpenGLWindow::wheelEvent(event);
	m_scene->change_camera_distance(-event->angleDelta().y() / 120.0f);
}

GLWindow::~GLWindow()
{
	makeCurrent();
	m_scene.reset();
	doneCurrent();
}