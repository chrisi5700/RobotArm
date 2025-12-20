//
// Created by chris on 12/18/25.
//
// ShaderControls.cpp
#include <QtRendering/ShaderControls.hpp>
#include <QScrollArea>

ColorPicker::ColorPicker(const QString& label, glm::vec3 initial, QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(new QLabel(label));

    auto createSpinBox = [](float initial) {
        auto* spin = new QDoubleSpinBox();
        spin->setRange(0.0, 1.0);
        spin->setSingleStep(0.05);
        spin->setValue(initial);
        spin->setFixedWidth(60);
        return spin;
    };

    m_r = createSpinBox(initial.r);
    m_g = createSpinBox(initial.g);
    m_b = createSpinBox(initial.b);

    layout->addWidget(new QLabel("R"));
    layout->addWidget(m_r);
    layout->addWidget(new QLabel("G"));
    layout->addWidget(m_g);
    layout->addWidget(new QLabel("B"));
    layout->addWidget(m_b);

    auto emitChange = [this]() {
        emit colorChanged(getColor());
    };

    connect(m_r, &QDoubleSpinBox::valueChanged, this, emitChange);
    connect(m_g, &QDoubleSpinBox::valueChanged, this, emitChange);
    connect(m_b, &QDoubleSpinBox::valueChanged, this, emitChange);
}

glm::vec3 ColorPicker::getColor() const
{
    return glm::vec3(m_r->value(), m_g->value(), m_b->value());
}

FloatSlider::FloatSlider(const QString& label, float min, float max, float initial, QWidget* parent)
    : QWidget(parent)
    , m_min(min)
    , m_max(max)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(new QLabel(label));

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setRange(0, 1000);
    m_slider->setValue((initial - min) / (max - min) * 1000);

    m_valueLabel = new QLabel(QString::number(initial, 'f', 2));
    m_valueLabel->setFixedWidth(40);

    layout->addWidget(m_slider, 1);
    layout->addWidget(m_valueLabel);

    connect(m_slider, &QSlider::valueChanged, this, [this](int value) {
        float fval = m_min + (m_max - m_min) * value / 1000.0f;
        m_valueLabel->setText(QString::number(fval, 'f', 2));
        emit valueChanged(fval);
    });
}

float FloatSlider::getValue() const
{
    return m_min + (m_max - m_min) * m_slider->value() / 1000.0f;
}

ShaderControls::ShaderControls(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    auto* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto* scrollWidget = new QWidget();
    auto* scrollLayout = new QVBoxLayout(scrollWidget);

    scrollLayout->addWidget(createLightingGroup());
    scrollLayout->addWidget(createRimLightGroup());
    scrollLayout->addWidget(createFogGroup());
    scrollLayout->addWidget(createBackgroundGroup());
    scrollLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

}

QGroupBox* ShaderControls::createLightingGroup()
{
    auto* group = new QGroupBox("Lighting");
    auto* layout = new QVBoxLayout(group);

    m_ambientSlider = new FloatSlider("Ambient", 0.0f, 1.0f, 0.15f);
    m_diffuseSlider = new FloatSlider("Diffuse", 0.0f, 2.0f, 1.0f);
    m_specularSlider = new FloatSlider("Specular", 0.0f, 2.0f, 0.5f);
    m_shininessSlider = new FloatSlider("Shininess", 1.0f, 128.0f, 32.0f);

    layout->addWidget(m_ambientSlider);
    layout->addWidget(m_diffuseSlider);
    layout->addWidget(m_specularSlider);
    layout->addWidget(m_shininessSlider);

    connect(m_ambientSlider, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_diffuseSlider, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_specularSlider, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_shininessSlider, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

    return group;
}

QGroupBox* ShaderControls::createRimLightGroup()
{
    auto* group = new QGroupBox("Rim Light");
    auto* layout = new QVBoxLayout(group);

    m_rimEnabled = new QCheckBox("Enable");
    m_rimEnabled->setChecked(true);
    m_rimColor = new ColorPicker("Color", glm::vec3(0.3f, 0.4f, 0.5f));
    m_rimPower = new FloatSlider("Power", 1.0f, 8.0f, 3.0f);
    m_rimStrength = new FloatSlider("Strength", 0.0f, 2.0f, 0.5f);

    layout->addWidget(m_rimEnabled);
    layout->addWidget(m_rimColor);
    layout->addWidget(m_rimPower);
    layout->addWidget(m_rimStrength);

    connect(m_rimEnabled, &QCheckBox::toggled, this, &ShaderControls::settingsChanged);
    connect(m_rimColor, &ColorPicker::colorChanged, this, &ShaderControls::settingsChanged);
    connect(m_rimPower, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_rimStrength, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

    return group;
}

QGroupBox* ShaderControls::createFogGroup()
{
    auto* group = new QGroupBox("Height Fog");
    auto* layout = new QVBoxLayout(group);

    m_fogEnabled = new QCheckBox("Enable");
    m_fogEnabled->setChecked(false);
    m_fogColor = new ColorPicker("Color", glm::vec3(0.5f, 0.6f, 0.7f));
    m_fogDensity = new FloatSlider("Density", 0.0f, 0.1f, 0.02f);

    layout->addWidget(m_fogEnabled);
    layout->addWidget(m_fogColor);
    layout->addWidget(m_fogDensity);

    connect(m_fogEnabled, &QCheckBox::toggled, this, &ShaderControls::settingsChanged);
    connect(m_fogColor, &ColorPicker::colorChanged, this, &ShaderControls::settingsChanged);
    connect(m_fogDensity, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

    return group;
}

QGroupBox* ShaderControls::createBackgroundGroup()
{
    auto* group = new QGroupBox("Background");
    auto* layout = new QVBoxLayout(group);

    m_topColor = new ColorPicker("Top", glm::vec3(0.4f, 0.5f, 0.7f));
    m_bottomColor = new ColorPicker("Bottom", glm::vec3(0.1f, 0.1f, 0.15f));

    layout->addWidget(m_topColor);
    layout->addWidget(m_bottomColor);

    connect(m_topColor, &ColorPicker::colorChanged, this, &ShaderControls::settingsChanged);
    connect(m_bottomColor, &ColorPicker::colorChanged, this, &ShaderControls::settingsChanged);

    return group;
}

// Getters
float ShaderControls::getAmbientStrength() const { return m_ambientSlider->getValue(); }
float ShaderControls::getDiffuseStrength() const { return m_diffuseSlider->getValue(); }
float ShaderControls::getSpecularStrength() const { return m_specularSlider->getValue(); }
float ShaderControls::getShininess() const { return m_shininessSlider->getValue(); }

bool ShaderControls::isRimLightEnabled() const { return m_rimEnabled->isChecked(); }
glm::vec3 ShaderControls::getRimColor() const { return m_rimColor->getColor(); }
float ShaderControls::getRimPower() const { return m_rimPower->getValue(); }
float ShaderControls::getRimStrength() const { return m_rimStrength->getValue(); }

bool ShaderControls::isFogEnabled() const { return m_fogEnabled->isChecked(); }
glm::vec3 ShaderControls::getFogColor() const { return m_fogColor->getColor(); }
float ShaderControls::getFogDensity() const { return m_fogDensity->getValue(); }

glm::vec3 ShaderControls::getTopColor() const { return m_topColor->getColor(); }
glm::vec3 ShaderControls::getBottomColor() const { return m_bottomColor->getColor(); }