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

    // Shadow
    float getShadowThreshold() const;
    float getShadowSoftness() const;
    glm::vec3 getShadowTint() const;
    float getShadowStrength() const;

    // Specular
    bool isSpecularEnabled() const;
    float getSpecularThreshold() const;
    float getSpecularSize() const;

    // Rim light
    bool isRimLightEnabled() const;
    glm::vec3 getRimColor() const;
    float getRimThreshold() const;
    float getRimSoftness() const;

    // Outline
    bool isOutlineEnabled() const;
    float getOutlineThreshold() const;
    float getOutlineStrength() const;

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
    QGroupBox* createShadowGroup();
    QGroupBox* createSpecularGroup();
    QGroupBox* createRimLightGroup();
    QGroupBox* createOutlineGroup();
    QGroupBox* createFogGroup();

    // Shadow
    FloatSlider* m_shadowThreshold;
    FloatSlider* m_shadowSoftness;
    ColorPicker* m_shadowTint;
    FloatSlider* m_shadowStrength;

    // Specular
    QCheckBox* m_specularEnabled;
    FloatSlider* m_specularThreshold;
    FloatSlider* m_specularSize;

    // Rim
    QCheckBox* m_rimEnabled;
    ColorPicker* m_rimColor;
    FloatSlider* m_rimThreshold;
    FloatSlider* m_rimSoftness;

    // Outline
    QCheckBox* m_outlineEnabled;
    FloatSlider* m_outlineThreshold;
    FloatSlider* m_outlineStrength;

    // Fog
    QCheckBox* m_fogEnabled;
    ColorPicker* m_fogColor;
    FloatSlider* m_fogDensity;

};