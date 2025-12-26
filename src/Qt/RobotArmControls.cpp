#include <../include/RobotArm/Qt/RobotArmControls.hpp>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>

// ============================================================================
// PistonWidget - Extends/retracts between MIN_LENGTH and max_length
// ============================================================================
PistonWidget::PistonWidget(std::size_t index, float maxLength, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
    , m_maxLength(maxLength)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // Header with title and remove button
    auto* header = new QHBoxLayout();
    m_titleLabel = new QLabel(QString("Piston %1").arg(index));
    m_titleLabel->setStyleSheet("font-weight: bold; color: #7ae664;");
    auto* removeBtn = new QPushButton("×");
    removeBtn->setFixedSize(20, 20);
    removeBtn->setStyleSheet("color: red; font-weight: bold;");
    header->addWidget(m_titleLabel);
    header->addStretch();
    header->addWidget(removeBtn);
    layout->addLayout(header);

    // Max length display
    auto* maxLengthLayout = new QHBoxLayout();
    maxLengthLayout->addWidget(new QLabel("Max Length:"));
    auto* maxLengthLabel = new QLabel(QString::number(maxLength, 'f', 1));
    maxLengthLabel->setStyleSheet("font-weight: bold;");
    maxLengthLayout->addWidget(maxLengthLabel);
    maxLengthLayout->addStretch();
    layout->addLayout(maxLengthLayout);

    // Target length slider (MIN_LENGTH=1.0 to max_length)
    auto* lengthLayout = new QHBoxLayout();
    lengthLayout->addWidget(new QLabel("Target:"));
    m_lengthSlider = new QSlider(Qt::Horizontal);
    m_lengthSlider->setRange(100, static_cast<int>(maxLength * 100)); // 1.0 to maxLength in 0.01 steps
    m_lengthSlider->setValue(100); // Start at MIN_LENGTH
    m_lengthLabel = new QLabel("1.00");
    m_lengthLabel->setFixedWidth(40);
    lengthLayout->addWidget(m_lengthSlider);
    lengthLayout->addWidget(m_lengthLabel);
    layout->addLayout(lengthLayout);

    // Connections
    connect(m_lengthSlider, &QSlider::valueChanged, this, [this](int value) {
        float length = value / 100.0f;
        m_lengthLabel->setText(QString::number(length, 'f', 2));
        emit targetLengthChanged(m_index, length);
    });

    connect(removeBtn, &QPushButton::clicked, this, [this]() {
        emit removeRequested(m_index);
    });

    setStyleSheet("PistonWidget { border: 1px solid #7ae664; border-radius: 4px; }");
}

void PistonWidget::setIndex(std::size_t index)
{
    m_index = index;
    m_titleLabel->setText(QString("Piston %1").arg(index));
}

// ============================================================================
// HingeWidget - Rotates on Z-axis
// ============================================================================
HingeWidget::HingeWidget(std::size_t index, float initialAngle, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // Header with title and remove button
    auto* header = new QHBoxLayout();
    m_titleLabel = new QLabel(QString("Hinge %1").arg(index));
    m_titleLabel->setStyleSheet("font-weight: bold; color: #cc241d;");
    auto* removeBtn = new QPushButton("×");
    removeBtn->setFixedSize(20, 20);
    removeBtn->setStyleSheet("color: red; font-weight: bold;");
    header->addWidget(m_titleLabel);
    header->addStretch();
    header->addWidget(removeBtn);
    layout->addLayout(header);

    // Target angle slider
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

    // Connections
    connect(m_angleSlider, &QSlider::valueChanged, this, [this](int value) {
        float angle = value / 100.0f;
        m_angleLabel->setText(QString::number(angle, 'f', 2));
        emit targetAngleChanged(m_index, angle);
    });

    connect(removeBtn, &QPushButton::clicked, this, [this]() {
        emit removeRequested(m_index);
    });

    setStyleSheet("HingeWidget { border: 1px solid #cc241d; border-radius: 4px; }");
}

void HingeWidget::setIndex(std::size_t index)
{
    m_index = index;
    m_titleLabel->setText(QString("Hinge %1").arg(index));
}

// ============================================================================
// SwivelWidget - Rotates on Y-axis
// ============================================================================
SwivelWidget::SwivelWidget(std::size_t index, float initialSpeed, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // Header with title and remove button
    auto* header = new QHBoxLayout();
    m_titleLabel = new QLabel(QString("Swivel %1").arg(index));
    m_titleLabel->setStyleSheet("font-weight: bold; color: #d79921;");
    auto* removeBtn = new QPushButton("×");
    removeBtn->setFixedSize(20, 20);
    removeBtn->setStyleSheet("color: red; font-weight: bold;");
    header->addWidget(m_titleLabel);
    header->addStretch();
    header->addWidget(removeBtn);
    layout->addLayout(header);

    // Rotational speed spinbox
    auto* speedLayout = new QHBoxLayout();
    speedLayout->addWidget(new QLabel("Speed (rad/s):"));
    m_speedSpinBox = new QDoubleSpinBox();
    m_speedSpinBox->setRange(-10.0, 10.0);
    m_speedSpinBox->setSingleStep(0.1);
    m_speedSpinBox->setValue(initialSpeed);
    speedLayout->addWidget(m_speedSpinBox);
    layout->addLayout(speedLayout);

    // Connections
    connect(m_speedSpinBox, &QDoubleSpinBox::valueChanged, this, [this](double value) {
        emit rotationalSpeedChanged(m_index, static_cast<float>(value));
    });

    connect(removeBtn, &QPushButton::clicked, this, [this]() {
        emit removeRequested(m_index);
    });

    setStyleSheet("SwivelWidget { border: 1px solid #d79921; border-radius: 4px; }");
}

void SwivelWidget::setIndex(std::size_t index)
{
    m_index = index;
    m_titleLabel->setText(QString("Swivel %1").arg(index));
}

// ============================================================================
// LinkWidget - Fixed length, no controls
// ============================================================================
LinkWidget::LinkWidget(std::size_t index, float length, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // Header with title and remove button
    auto* header = new QHBoxLayout();
    m_titleLabel = new QLabel(QString("Link %1").arg(index));
    m_titleLabel->setStyleSheet("font-weight: bold; color: #7a7a7a;");
    auto* removeBtn = new QPushButton("×");
    removeBtn->setFixedSize(20, 20);
    removeBtn->setStyleSheet("color: red; font-weight: bold;");
    header->addWidget(m_titleLabel);
    header->addStretch();
    header->addWidget(removeBtn);
    layout->addLayout(header);

    // Length display (read-only)
    auto* lengthLayout = new QHBoxLayout();
    lengthLayout->addWidget(new QLabel("Length:"));
    auto* lengthLabel = new QLabel(QString::number(length, 'f', 2));
    lengthLabel->setStyleSheet("font-weight: bold;");
    lengthLayout->addWidget(lengthLabel);
    lengthLayout->addStretch();
    layout->addLayout(lengthLayout);

    // Connections
    connect(removeBtn, &QPushButton::clicked, this, [this]() {
        emit removeRequested(m_index);
    });

    setStyleSheet("LinkWidget { border: 1px solid #7a7a7a; border-radius: 4px; }");
}

void LinkWidget::setIndex(std::size_t index)
{
    m_index = index;
    m_titleLabel->setText(QString("Link %1").arg(index));
}

// ============================================================================
// RobotArmControls - Main control panel
// ============================================================================
RobotArmControls::RobotArmControls(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    // Component type selection (radio buttons)
    auto* typeLabel = new QLabel("Component Type:");
    typeLabel->setStyleSheet("font-weight: bold; margin-top: 5px;");
    mainLayout->addWidget(typeLabel);

    auto* typeButtonLayout = new QHBoxLayout();
    m_componentTypeGroup = new QButtonGroup(this);

    m_pistonRadio = new QRadioButton("Piston");
    m_hingeRadio = new QRadioButton("Hinge");
    m_swivelRadio = new QRadioButton("Swivel");
    m_linkRadio = new QRadioButton("Link");

    m_pistonRadio->setStyleSheet("color: #7ae664; font-weight: bold;");
    m_hingeRadio->setStyleSheet("color: #cc241d; font-weight: bold;");
    m_swivelRadio->setStyleSheet("color: #d79921; font-weight: bold;");
    m_linkRadio->setStyleSheet("color: #7a7a7a; font-weight: bold;");

    m_componentTypeGroup->addButton(m_pistonRadio, 0);
    m_componentTypeGroup->addButton(m_hingeRadio, 1);
    m_componentTypeGroup->addButton(m_swivelRadio, 2);
    m_componentTypeGroup->addButton(m_linkRadio, 3);

    typeButtonLayout->addWidget(m_pistonRadio);
    typeButtonLayout->addWidget(m_hingeRadio);
    typeButtonLayout->addWidget(m_swivelRadio);
    typeButtonLayout->addWidget(m_linkRadio);
    mainLayout->addLayout(typeButtonLayout);

    m_pistonRadio->setChecked(true); // Default selection

    // Parameter inputs (shown/hidden based on selection)
    // Piston parameters
    m_pistonParamsWidget = new QWidget();
    auto* pistonParamsLayout = new QHBoxLayout(m_pistonParamsWidget);
    pistonParamsLayout->setContentsMargins(0, 0, 0, 0);
    pistonParamsLayout->addWidget(new QLabel("Max Length:"));
    m_pistonMaxLengthSpinBox = new QDoubleSpinBox();
    m_pistonMaxLengthSpinBox->setRange(1.1, 20.0);
    m_pistonMaxLengthSpinBox->setValue(5.0);
    m_pistonMaxLengthSpinBox->setSingleStep(0.5);
    pistonParamsLayout->addWidget(m_pistonMaxLengthSpinBox);
    pistonParamsLayout->addStretch();
    mainLayout->addWidget(m_pistonParamsWidget);

    // Link parameters
    m_linkParamsWidget = new QWidget();
    auto* linkParamsLayout = new QHBoxLayout(m_linkParamsWidget);
    linkParamsLayout->setContentsMargins(0, 0, 0, 0);
    linkParamsLayout->addWidget(new QLabel("Length:"));
    m_linkLengthSpinBox = new QDoubleSpinBox();
    m_linkLengthSpinBox->setRange(0.1, 20.0);
    m_linkLengthSpinBox->setValue(1.0);
    m_linkLengthSpinBox->setSingleStep(0.5);
    linkParamsLayout->addWidget(m_linkLengthSpinBox);
    linkParamsLayout->addStretch();
    mainLayout->addWidget(m_linkParamsWidget);

    // Add Component button
    auto* addButton = new QPushButton("Add Component");
    addButton->setStyleSheet("padding: 10px; font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(addButton);

    // Initially show correct parameters
    updateParameterVisibility();

    // Separator line
    auto* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

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

    // Connections
    connect(m_componentTypeGroup, &QButtonGroup::buttonClicked, this, &RobotArmControls::onComponentTypeChanged);
    connect(addButton, &QPushButton::clicked, this, &RobotArmControls::onAddComponentClicked);
}

void RobotArmControls::updateParameterVisibility()
{
    int selectedType = m_componentTypeGroup->checkedId();

    m_pistonParamsWidget->setVisible(selectedType == 0); // Piston
    m_linkParamsWidget->setVisible(selectedType == 3);   // Link
}

void RobotArmControls::onComponentTypeChanged()
{
    updateParameterVisibility();
}

void RobotArmControls::onAddComponentClicked()
{
    int selectedType = m_componentTypeGroup->checkedId();

    switch (selectedType) {
        case 0: { // Piston
            float maxLength = m_pistonMaxLengthSpinBox->value();
            addPistonWidget(maxLength);
            emit pistonAdded(maxLength);
            break;
        }
        case 1: // Hinge
            addHingeWidget(0.0f);
            emit hingeAdded();
            break;
        case 2: // Swivel
            addSwivelWidget(0.0f);
            emit swivelAdded();
            break;
        case 3: { // Link
            float length = m_linkLengthSpinBox->value();
            addLinkWidget(length);
            emit linkAdded(length);
            break;
        }
    }
}

void RobotArmControls::addPistonWidget(float maxLength)
{
    auto* widget = new PistonWidget(m_componentWidgets.size(), maxLength);

    connect(widget, &PistonWidget::targetLengthChanged, this, &RobotArmControls::pistonTargetLengthChanged);
    connect(widget, &PistonWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);

    m_componentWidgets.push_back(widget);
    m_componentsLayout->insertWidget(m_componentsLayout->count() - 1, widget);
}

void RobotArmControls::addHingeWidget(float initialAngle)
{
    auto* widget = new HingeWidget(m_componentWidgets.size(), initialAngle);

    connect(widget, &HingeWidget::targetAngleChanged, this, &RobotArmControls::hingeTargetAngleChanged);
    connect(widget, &HingeWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);

    m_componentWidgets.push_back(widget);
    m_componentsLayout->insertWidget(m_componentsLayout->count() - 1, widget);
}

void RobotArmControls::addSwivelWidget(float speed)
{
    auto* widget = new SwivelWidget(m_componentWidgets.size(), speed);

    connect(widget, &SwivelWidget::rotationalSpeedChanged, this, &RobotArmControls::swivelRotationalSpeedChanged);
    connect(widget, &SwivelWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);

    m_componentWidgets.push_back(widget);
    m_componentsLayout->insertWidget(m_componentsLayout->count() - 1, widget);
}

void RobotArmControls::addLinkWidget(float length)
{
    auto* widget = new LinkWidget(m_componentWidgets.size(), length);

    connect(widget, &LinkWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);

    m_componentWidgets.push_back(widget);
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
    // Update indices in all widgets after removal
    for (std::size_t i = 0; i < m_componentWidgets.size(); ++i) {
        if (auto* widget = qobject_cast<PistonWidget*>(m_componentWidgets[i])) {
            widget->setIndex(i);
        }
        else if (auto* widget = qobject_cast<HingeWidget*>(m_componentWidgets[i])) {
            widget->setIndex(i);
        }
        else if (auto* widget = qobject_cast<SwivelWidget*>(m_componentWidgets[i])) {
            widget->setIndex(i);
        }
        else if (auto* widget = qobject_cast<LinkWidget*>(m_componentWidgets[i])) {
            widget->setIndex(i);
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
