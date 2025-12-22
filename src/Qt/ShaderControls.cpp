#include <../include/RobotArm/Qt/ShaderControls.hpp>
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
    m_slider->setValue(static_cast<int>((initial - min) / (max - min) * 1000));

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

    scrollLayout->addWidget(createShadowGroup());
    scrollLayout->addWidget(createSpecularGroup());
    scrollLayout->addWidget(createRimLightGroup());
    scrollLayout->addWidget(createOutlineGroup());
    scrollLayout->addWidget(createFogGroup());
    scrollLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
}

QGroupBox* ShaderControls::createShadowGroup()
{
    auto* group = new QGroupBox("Cel Shading");
    auto* layout = new QVBoxLayout(group);

    m_shadowThreshold = new FloatSlider("Threshold", -0.5f, 0.5f, 0.1f);
    m_shadowSoftness = new FloatSlider("Softness", 0.0f, 0.3f, 0.05f);
    m_shadowTint = new ColorPicker("Tint", glm::vec3(0.6f, 0.6f, 0.8f));
    m_shadowStrength = new FloatSlider("Darkness", 0.0f, 1.0f, 0.4f);

    layout->addWidget(m_shadowThreshold);
    layout->addWidget(m_shadowSoftness);
    layout->addWidget(m_shadowTint);
    layout->addWidget(m_shadowStrength);

    connect(m_shadowThreshold, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_shadowSoftness, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_shadowTint, &ColorPicker::colorChanged, this, &ShaderControls::settingsChanged);
    connect(m_shadowStrength, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

    return group;
}

QGroupBox* ShaderControls::createSpecularGroup()
{
    auto* group = new QGroupBox("Specular");
    auto* layout = new QVBoxLayout(group);

    m_specularEnabled = new QCheckBox("Enable");
    m_specularEnabled->setChecked(true);
    m_specularThreshold = new FloatSlider("Threshold", 0.5f, 0.99f, 0.9f);
    m_specularSize = new FloatSlider("Tightness", 8.0f, 128.0f, 32.0f);

    layout->addWidget(m_specularEnabled);
    layout->addWidget(m_specularThreshold);
    layout->addWidget(m_specularSize);

    connect(m_specularEnabled, &QCheckBox::toggled, this, &ShaderControls::settingsChanged);
    connect(m_specularThreshold, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_specularSize, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

    return group;
}

QGroupBox* ShaderControls::createRimLightGroup()
{
    auto* group = new QGroupBox("Rim Light");
    auto* layout = new QVBoxLayout(group);

    m_rimEnabled = new QCheckBox("Enable");
    m_rimEnabled->setChecked(true);
    m_rimColor = new ColorPicker("Color", glm::vec3(1.0f, 0.95f, 0.9f));
    m_rimThreshold = new FloatSlider("Threshold", 0.3f, 0.9f, 0.65f);
    m_rimSoftness = new FloatSlider("Softness", 0.0f, 0.3f, 0.1f);

    layout->addWidget(m_rimEnabled);
    layout->addWidget(m_rimColor);
    layout->addWidget(m_rimThreshold);
    layout->addWidget(m_rimSoftness);

    connect(m_rimEnabled, &QCheckBox::toggled, this, &ShaderControls::settingsChanged);
    connect(m_rimColor, &ColorPicker::colorChanged, this, &ShaderControls::settingsChanged);
    connect(m_rimThreshold, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_rimSoftness, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

    return group;
}

QGroupBox* ShaderControls::createOutlineGroup()
{
    auto* group = new QGroupBox("Edge Darkening");
    auto* layout = new QVBoxLayout(group);

    m_outlineEnabled = new QCheckBox("Enable");
    m_outlineEnabled->setChecked(true);
    m_outlineThreshold = new FloatSlider("Threshold", 0.1f, 0.6f, 0.25f);
    m_outlineStrength = new FloatSlider("Strength", 0.0f, 1.0f, 0.6f);

    layout->addWidget(m_outlineEnabled);
    layout->addWidget(m_outlineThreshold);
    layout->addWidget(m_outlineStrength);

    connect(m_outlineEnabled, &QCheckBox::toggled, this, &ShaderControls::settingsChanged);
    connect(m_outlineThreshold, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);
    connect(m_outlineStrength, &FloatSlider::valueChanged, this, &ShaderControls::settingsChanged);

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



// Shadow getters
float ShaderControls::getShadowThreshold() const { return m_shadowThreshold->getValue(); }
float ShaderControls::getShadowSoftness() const { return m_shadowSoftness->getValue(); }
glm::vec3 ShaderControls::getShadowTint() const { return m_shadowTint->getColor(); }
float ShaderControls::getShadowStrength() const { return m_shadowStrength->getValue(); }

// Specular getters
bool ShaderControls::isSpecularEnabled() const { return m_specularEnabled->isChecked(); }
float ShaderControls::getSpecularThreshold() const { return m_specularThreshold->getValue(); }
float ShaderControls::getSpecularSize() const { return m_specularSize->getValue(); }

// Rim getters
bool ShaderControls::isRimLightEnabled() const { return m_rimEnabled->isChecked(); }
glm::vec3 ShaderControls::getRimColor() const { return m_rimColor->getColor(); }
float ShaderControls::getRimThreshold() const { return m_rimThreshold->getValue(); }
float ShaderControls::getRimSoftness() const { return m_rimSoftness->getValue(); }

// Outline getters
bool ShaderControls::isOutlineEnabled() const { return m_outlineEnabled->isChecked(); }
float ShaderControls::getOutlineThreshold() const { return m_outlineThreshold->getValue(); }
float ShaderControls::getOutlineStrength() const { return m_outlineStrength->getValue(); }

// Fog getters
bool ShaderControls::isFogEnabled() const { return m_fogEnabled->isChecked(); }
glm::vec3 ShaderControls::getFogColor() const { return m_fogColor->getColor(); }
float ShaderControls::getFogDensity() const { return m_fogDensity->getValue(); }

