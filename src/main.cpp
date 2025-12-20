#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QtRendering/GLWidget.hpp>
#include <QtRendering/RobotArmControls.hpp>
#include <QtRendering/ShaderControls.hpp>

int main(int argc, char* argv[]) {
#ifdef __EMSCRIPTEN__
	QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, false);
#endif
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setSwapInterval(1);
    QSurfaceFormat::setDefaultFormat(format);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Robot Arm");
    mainWindow.resize(1280, 720);

    auto* central = new QWidget();
    auto* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    auto* glWidget = new GLWidget();

    // Tab widget for both control panels
    auto* tabs = new QTabWidget();
    tabs->setFixedWidth(350);

    auto* armControls = new RobotArmControls();
    auto* shaderControls = new ShaderControls();

    tabs->addTab(armControls, "Robot Arm");
    tabs->addTab(shaderControls, "Shaders");

    layout->addWidget(glWidget, 1);
    layout->addWidget(tabs, 0);

    mainWindow.setCentralWidget(central);

    // Defer until GL is initialized
    QObject::connect(glWidget, &QOpenGLWidget::frameSwapped, glWidget, [=]() {
        static bool initialized = false;
        if (initialized) return;
        initialized = true;

        auto& arm = glWidget->get_scene().get_arm();
        arm.add_component(2.0f, 0.0f);
        arm.add_component(1.5f, 0.5f);
        arm.add_component(1.0f, -0.3f);

        armControls->addComponentWidget(2.0f, 0.0f);
        armControls->addComponentWidget(1.5f, 0.5f);
        armControls->addComponentWidget(1.0f, -0.3f);
    }, Qt::SingleShotConnection);

    // Robot arm signals
    QObject::connect(armControls, &RobotArmControls::componentAdded,
        [glWidget](float length, float angle) {
            glWidget->get_scene().get_arm().add_component(length, angle);
        });

    QObject::connect(armControls, &RobotArmControls::angleChanged,
        [glWidget](std::size_t index, float angle) {
            glWidget->get_scene().get_arm().set_target_angle(index, angle);
        });

    QObject::connect(armControls, &RobotArmControls::lengthChanged,
        [glWidget](std::size_t index, float length) {
            glWidget->get_scene().get_arm().set_length(index, length);
        });

    QObject::connect(armControls, &RobotArmControls::componentRemoved,
        [glWidget](std::size_t index) {
            glWidget->get_scene().get_arm().remove_component(index);
        });

    // Shader controls signals
    QObject::connect(shaderControls, &ShaderControls::settingsChanged,
        [glWidget, shaderControls]() {
            auto& params = glWidget->get_scene().get_shader_params();

            params.ambientStrength = shaderControls->getAmbientStrength();
            params.diffuseStrength = shaderControls->getDiffuseStrength();
            params.specularStrength = shaderControls->getSpecularStrength();
            params.shininess = shaderControls->getShininess();

            params.enableRimLight = shaderControls->isRimLightEnabled();
            params.rimColor = shaderControls->getRimColor();
            params.rimPower = shaderControls->getRimPower();
            params.rimStrength = shaderControls->getRimStrength();

            params.enableFog = shaderControls->isFogEnabled();
            params.fogColor = shaderControls->getFogColor();
            params.fogDensity = shaderControls->getFogDensity();

            params.topColor = shaderControls->getTopColor();
            params.bottomColor = shaderControls->getBottomColor();
        });

    mainWindow.show();
    return app.exec();
}