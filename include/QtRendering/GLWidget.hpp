//
// Created by chris on 12/17/25.
//

#ifndef OPENGL_TEST_GLWIDGET_HPP
#define OPENGL_TEST_GLWIDGET_HPP
#include "Renderer.hpp"
#include <QChronoTimer>
#include <QOpenGLWidget>


class GLWidget : public QOpenGLWidget {
	Q_OBJECT
public:
	explicit GLWidget(QWidget* parent = nullptr);
	~GLWidget() override;

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
private:
	std::unique_ptr<Renderer> m_renderer;
	QChronoTimer m_frame_timer;
	QElapsedTimer m_elapsed_timer;
};

#endif // OPENGL_TEST_GLWIDGET_HPP
