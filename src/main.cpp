// main.cpp
#include <QApplication>


#include <QtRendering/GLWidget.hpp>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	// Set default surface format before creating any windows
	QSurfaceFormat format;
	format.setVersion(4, 6);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setSwapInterval(1);  // VSync
	QSurfaceFormat::setDefaultFormat(format);

	GLWidget widget;
	widget.setWindowTitle("OpenGL Instanced Cubes");
	widget.resize(1280, 720);
	widget.show();

	return app.exec();
}