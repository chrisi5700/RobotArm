#ifndef OPENGL_TEST_GLWINDOW_HPP
#define OPENGL_TEST_GLWINDOW_HPP

#include "GLCommon.hpp"
#include "Scene.hpp"
#include <QChronoTimer>
#include <QOpenGLWindow>
#include <QElapsedTimer>
#include <memory>

class GLWindow : public QOpenGLWindow {
	Q_OBJECT
public:
	explicit GLWindow(QWindow* parent = nullptr);
	~GLWindow() override;
	Scene& get_scene() { return *m_scene; }

	signals:
		void initialized();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	std::unique_ptr<Scene> m_scene;
	QChronoTimer m_frame_timer;
	QElapsedTimer m_elapsed_timer;
	bool m_is_dragging = false;
};

#endif