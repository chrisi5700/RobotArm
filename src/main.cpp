#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QtRendering/GLWindow.hpp>
#include <QtRendering/RobotArmControls.hpp>
#include <QtRendering/ShaderControls.hpp>

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

    format.setSwapInterval(1);
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
        auto& arm = glWindow->get_scene().get_arm();
        arm.add_component(2.0f, 0.0f);
        arm.add_component(1.5f, 0.5f);
        arm.add_component(1.0f, -0.3f);

        armControls->addComponentWidget(2.0f, 0.0f);
        armControls->addComponentWidget(1.5f, 0.5f);
        armControls->addComponentWidget(1.0f, -0.3f);
    });

    QObject::connect(armControls, &RobotArmControls::componentAdded,
        [glWindow](float length, float angle) {
            glWindow->get_scene().get_arm().add_component(length, angle);
        });

    QObject::connect(armControls, &RobotArmControls::angleChanged,
        [glWindow](std::size_t index, float angle) {
            glWindow->get_scene().get_arm().set_target_angle(index, angle);
        });

    QObject::connect(armControls, &RobotArmControls::lengthChanged,
        [glWindow](std::size_t index, float length) {
            glWindow->get_scene().get_arm().set_length(index, length);
        });

    QObject::connect(armControls, &RobotArmControls::componentRemoved,
        [glWindow](std::size_t index) {
            glWindow->get_scene().get_arm().remove_component(index);
        });

    QObject::connect(shaderControls, &ShaderControls::settingsChanged,
        [glWindow, shaderControls]() {
            auto& params = glWindow->get_scene().get_shader_params();

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