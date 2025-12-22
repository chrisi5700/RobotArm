#ifndef OPENGL_TEST_GLWINDOW_HPP
#define OPENGL_TEST_GLWINDOW_HPP


#include "RobotArm/Rendering/Renderer.hpp"
#include "Scene.hpp"

#include <memory>
#include <QChronoTimer>
#include <QElapsedTimer>
#include <QOpenGLWindow>


class GLWindow : public QOpenGLWindow {
	Q_OBJECT
public:
	explicit GLWindow(QWindow* parent = nullptr);
	~GLWindow() override;
	Scene& get_scene() { return m_scene; }
	void set_shader_params(const ShaderParams& params);

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
	std::unique_ptr<Renderer> m_renderer;
	RenderQueue m_render_queue;
	Scene m_scene;
	QPoint m_last_pos{};
	qint64 m_last_time{};
	// QChronoTimer m_frame_timer;
	QElapsedTimer m_elapsed_timer;
	bool m_is_dragging = false;
};

#endif