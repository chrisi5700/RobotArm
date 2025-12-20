#include <QtRendering/RobotArmControls.hpp>
#include <QGroupBox>
#include <QHBoxLayout>

ComponentWidget::ComponentWidget(std::size_t index, float initialLength, float initialAngle, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    
    // Header with title and remove button
    auto* header = new QHBoxLayout();
    auto* title = new QLabel(QString("Segment %1").arg(index));
    title->setStyleSheet("font-weight: bold;");
    auto* removeBtn = new QPushButton("×");
    removeBtn->setFixedSize(20, 20);
    removeBtn->setStyleSheet("color: red; font-weight: bold;");
    header->addWidget(title);
    header->addStretch();
    header->addWidget(removeBtn);
    layout->addLayout(header);
    
    // Angle slider
    auto* angleLayout = new QHBoxLayout();
    angleLayout->addWidget(new QLabel("Angle:"));
    m_angleSlider = new QSlider(Qt::Horizontal);
    m_angleSlider->setRange(-314, 314);  // -π to π in centidegrees
    m_angleSlider->setValue(static_cast<int>(initialAngle * 100));
    m_angleLabel = new QLabel(QString::number(initialAngle, 'f', 2));
    m_angleLabel->setFixedWidth(40);
    angleLayout->addWidget(m_angleSlider);
    angleLayout->addWidget(m_angleLabel);
    layout->addLayout(angleLayout);
    
    // Length spinbox
    auto* lengthLayout = new QHBoxLayout();
    lengthLayout->addWidget(new QLabel("Length:"));
    m_lengthSpinBox = new QDoubleSpinBox();
    m_lengthSpinBox->setRange(0.1, 10.0);
    m_lengthSpinBox->setSingleStep(0.1);
    m_lengthSpinBox->setValue(initialLength);
    lengthLayout->addWidget(m_lengthSpinBox);
    layout->addLayout(lengthLayout);
    
    // Connections
    connect(m_angleSlider, &QSlider::valueChanged, this, [this](int value) {
        float angle = value / 100.0f;
        m_angleLabel->setText(QString::number(angle, 'f', 2));
        emit angleChanged(m_index, angle);
    });
    
    connect(m_lengthSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        emit lengthChanged(m_index, static_cast<float>(value));
    });
    
    connect(removeBtn, &QPushButton::clicked, this, [this]() {
        emit removeRequested(m_index);
    });
    
    setStyleSheet("ComponentWidget { border: 1px solid #ccc; border-radius: 4px; }");
}

RobotArmControls::RobotArmControls(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);
    
    // Add button at top
    auto* addButton = new QPushButton("+ Add Segment");
    addButton->setStyleSheet("padding: 8px; font-weight: bold;");
    mainLayout->addWidget(addButton);
    
    // Scroll area for components
    auto* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    auto* scrollWidget = new QWidget();
    m_componentsLayout = new QVBoxLayout(scrollWidget);
    m_componentsLayout->setAlignment(Qt::AlignTop);
    m_componentsLayout->addStretch();
    
    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);
    
    connect(addButton, &QPushButton::clicked, this, [this]() {
        addComponentWidget(1.0f, 0.0f);
        emit componentAdded(1.0f, 0.0f);
    });
    
}

void RobotArmControls::addComponentWidget(float length, float angle)
{
    auto* widget = new ComponentWidget(m_componentWidgets.size(), length, angle);
    
    connect(widget, &ComponentWidget::angleChanged, this, &RobotArmControls::angleChanged);
    connect(widget, &ComponentWidget::lengthChanged, this, &RobotArmControls::lengthChanged);
    connect(widget, &ComponentWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);
    
    m_componentWidgets.push_back(widget);
    
    // Insert before the stretch
    m_componentsLayout->insertWidget(m_componentsLayout->count() - 1, widget);
}

void RobotArmControls::removeComponentWidget(std::size_t index)
{
    if (index >= m_componentWidgets.size()) return;
    
    auto* widget = m_componentWidgets[index];
    m_componentsLayout->removeWidget(widget);
    widget->deleteLater();
    
    m_componentWidgets.erase(m_componentWidgets.begin() + index);
    
    rebuildIndices();
    emit componentRemoved(index);
}

void RobotArmControls::rebuildIndices()
{
    // After removal, we need to rebuild with correct indices
    // Simplest approach: just update the title labels
    for (std::size_t i = 0; i < m_componentWidgets.size(); ++i) {
        auto* label = m_componentWidgets[i]->findChild<QLabel*>();
        if (label) {
            label->setText(QString("Segment %1").arg(i));
        }
    }
}

void RobotArmControls::clear()
{
    for (auto* widget : m_componentWidgets) {
        widget->deleteLater();
    }
    m_componentWidgets.clear();
}