#include <QApplication>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QTabWidget>
#include <RobotArm/Qt/GLWindow.hpp>
#include <RobotArm/Qt/ShaderControls.hpp>

#include "RobotArm/Qt/RobotArmControls.hpp"

int main(int argc, char* argv[]) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

#ifdef __EMSCRIPTEN__
    format.setVersion(3, 0);
    format.setRenderableType(QSurfaceFormat::OpenGLES);
#else
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif

    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Robot Arm");
    mainWindow.resize(1280, 720);

    auto* central = new QWidget();
    auto* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* glWindow = new GLWindow();
    auto* glContainer = QWidget::createWindowContainer(glWindow, central);
    glContainer->setMinimumSize(400, 400);
    glContainer->setFocusPolicy(Qt::StrongFocus);

    auto* tabs = new QTabWidget();
    tabs->setFixedWidth(350);

    auto* armControls = new RobotArmControls();
    auto* shaderControls = new ShaderControls();

    tabs->addTab(armControls, "Robot Arm");
    tabs->addTab(shaderControls, "Shaders");

    layout->addWidget(glContainer, 1);
    layout->addWidget(tabs, 0);

    mainWindow.setCentralWidget(central);

    QObject::connect(glWindow, &GLWindow::initialized, glWindow, [=]() {
        auto& arm = glWindow->get_scene().get_simulation();
        arm.add_segment(2.0f, 0.0f);
        arm.add_segment(1.5f, 0.5f);
        arm.add_segment(1.0f, -0.3f);

        armControls->addSegmentWidget(2.0f, 0.0f);
        armControls->addSegmentWidget(1.5f, 0.5f);
        armControls->addSegmentWidget(1.0f, -0.3f);
    });

    QObject::connect(armControls, &RobotArmControls::segmentAdded,
        [glWindow](float length, float angle) {
            glWindow->get_scene().get_simulation().add_segment(length, angle);
        });

    QObject::connect(armControls, &RobotArmControls::spinnerAdded,
        [glWindow]() {
            glWindow->get_scene().get_simulation().add_spinner();
        });

    QObject::connect(armControls, &RobotArmControls::angleChanged,
        [glWindow](std::size_t index, float angle) {
            glWindow->get_scene().get_simulation().set_segment_target_angle(index, angle);
        });

    QObject::connect(armControls, &RobotArmControls::lengthChanged,
        [glWindow](std::size_t index, float length) {
            glWindow->get_scene().get_simulation().set_segment_length(index, length);
        });

    QObject::connect(armControls, &RobotArmControls::rotationalSpeedChanged,
        [glWindow](std::size_t index, float speed) {
            glWindow->get_scene().get_simulation().set_spinner_rotational_speed(index, speed);
        });

    QObject::connect(armControls, &RobotArmControls::componentRemoved,
        [glWindow](std::size_t index) {
            glWindow->get_scene().get_simulation().remove_component(index);
        });

	QObject::connect(shaderControls, &ShaderControls::settingsChanged,
	[glWindow, shaderControls]() {
		auto params = ShaderParams{};

		// Shadow
		params.shadowThreshold = shaderControls->getShadowThreshold();
		params.shadowSoftness = shaderControls->getShadowSoftness();
		params.shadowTint = shaderControls->getShadowTint();
		params.shadowStrength = shaderControls->getShadowStrength();

		// Specular
		params.enableSpecular = shaderControls->isSpecularEnabled();
		params.specularThreshold = shaderControls->getSpecularThreshold();
		params.specularSize = shaderControls->getSpecularSize();

		// Rim
		params.enableRimLight = shaderControls->isRimLightEnabled();
		params.rimColor = shaderControls->getRimColor();
		params.rimThreshold = shaderControls->getRimThreshold();
		params.rimSoftness = shaderControls->getRimSoftness();

		// Outline
		params.enableOutline = shaderControls->isOutlineEnabled();
		params.outlineThreshold = shaderControls->getOutlineThreshold();
		params.outlineStrength = shaderControls->getOutlineStrength();

		// Fog
		params.enableFog = shaderControls->isFogEnabled();
		params.fogColor = shaderControls->getFogColor();
		params.fogDensity = shaderControls->getFogDensity();

		glWindow->set_shader_params(params);
	});

    mainWindow.show();
    return app.exec();
}