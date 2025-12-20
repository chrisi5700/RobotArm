// ShaderControls.hpp
#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <glm/glm.hpp>
#include <functional>

class ColorPicker : public QWidget {
    Q_OBJECT
public:
    ColorPicker(const QString& label, glm::vec3 initial, QWidget* parent = nullptr);
    glm::vec3 getColor() const;

signals:
    void colorChanged(glm::vec3 color);

private:
    QDoubleSpinBox* m_r;
    QDoubleSpinBox* m_g;
    QDoubleSpinBox* m_b;
};

class FloatSlider : public QWidget {
    Q_OBJECT
public:
    FloatSlider(const QString& label, float min, float max, float initial, QWidget* parent = nullptr);
    float getValue() const;

signals:
    void valueChanged(float value);

private:
    QSlider* m_slider;
    QLabel* m_valueLabel;
    float m_min, m_max;
};

class ShaderControls : public QWidget {
    Q_OBJECT
public:
    explicit ShaderControls(QWidget* parent = nullptr);

    // Lighting
    float getAmbientStrength() const;
    float getDiffuseStrength() const;
    float getSpecularStrength() const;
    float getShininess() const;

    // Rim light
    bool isRimLightEnabled() const;
    glm::vec3 getRimColor() const;
    float getRimPower() const;
    float getRimStrength() const;

    // Fog
    bool isFogEnabled() const;
    glm::vec3 getFogColor() const;
    float getFogDensity() const;

    // Background
    glm::vec3 getTopColor() const;
    glm::vec3 getBottomColor() const;

signals:
    void settingsChanged();

private:
    QGroupBox* createLightingGroup();
    QGroupBox* createRimLightGroup();
    QGroupBox* createFogGroup();
    QGroupBox* createBackgroundGroup();

    // Lighting
    FloatSlider* m_ambientSlider;
    FloatSlider* m_diffuseSlider;
    FloatSlider* m_specularSlider;
    FloatSlider* m_shininessSlider;

    // Rim
    QCheckBox* m_rimEnabled;
    ColorPicker* m_rimColor;
    FloatSlider* m_rimPower;
    FloatSlider* m_rimStrength;

    // Fog
    QCheckBox* m_fogEnabled;
    ColorPicker* m_fogColor;
    FloatSlider* m_fogDensity;

    // Background
    ColorPicker* m_topColor;
    ColorPicker* m_bottomColor;
};