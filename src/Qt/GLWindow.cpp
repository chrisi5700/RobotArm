#include <chrono>
#include <QMouseEvent>

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#include <qopenglcontext.h>
#endif
#include <RobotArm/Qt/GLWindow.hpp>

using std::chrono_literals::operator""ms;

GLWindow::GLWindow(QWindow* parent)
	: QOpenGLWindow(QOpenGLWindow::NoPartialUpdate, parent)
	, m_renderer(nullptr)
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
	m_renderer = std::make_unique<Renderer>();
	m_elapsed_timer.start();
	m_frame_timer.start();

	emit initialized();
}

void GLWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	m_scene.get_camera().update_aspect_ratio(w, h);
}

void GLWindow::paintGL()
{
	auto time = m_elapsed_timer.elapsed();
	auto duration = time - m_last_time;
	m_last_time = time;
	m_scene.get_simulation().tick(duration / 1000.f);
	m_scene.submit_to(m_render_queue);
	m_renderer->render(m_render_queue, m_scene.get_camera());
	m_render_queue.clear();
}

void GLWindow::mousePressEvent(QMouseEvent* event)
{
	QOpenGLWindow::mousePressEvent(event);
	m_is_dragging = not m_is_dragging;
}


void GLWindow::mouseMoveEvent(QMouseEvent* event)
{
	QOpenGLWindow::mouseMoveEvent(event);
	auto pos = event->pos();
	auto delta = pos-m_last_pos;
	if (m_is_dragging)
	{
		m_scene.get_camera().drag_camera({delta.x(), delta.y()});
	}
	m_last_pos = pos;
}

void GLWindow::wheelEvent(QWheelEvent* event)
{
	QOpenGLWindow::wheelEvent(event);
	m_scene.get_camera().change_camera_distance(-event->angleDelta().y() / 120.0f);
}

GLWindow::~GLWindow()
{
	makeCurrent();
	m_renderer.reset();
	doneCurrent();
}
void GLWindow::set_shader_params(const ShaderParams& params)
{
	m_renderer->push_shader_params(params);
}