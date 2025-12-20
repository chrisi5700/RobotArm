//
// Created by chris on 12/17/25.
//

#ifndef OPENGL_TEST_GLWIDGET_HPP
#define OPENGL_TEST_GLWIDGET_HPP
#include "GLCommon.hpp"
#include "Scene.hpp"
#include <QChronoTimer>
#include <QOpenGLWidget>
#include <QElapsedTimer>


class GLWidget : public QOpenGLWidget {
	Q_OBJECT
public:
	explicit GLWidget(QWidget* parent = nullptr);
	~GLWidget() override;
	Scene& get_scene() { return *m_scene; }

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	// void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
private:
	std::unique_ptr<Scene> m_scene;
	QChronoTimer m_frame_timer;
	QElapsedTimer m_elapsed_timer;
};

#endif // OPENGL_TEST_GLWIDGET_HPP
