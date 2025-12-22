#include <../include/RobotArm/Qt/RobotArmControls.hpp>
#include <QGroupBox>
#include <QHBoxLayout>

SegmentWidget::SegmentWidget(std::size_t index, float initialLength, float initialAngle, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // Header with title and remove button
    auto* header = new QHBoxLayout();
    m_titleLabel = new QLabel(QString("Segment %1").arg(index));
    m_titleLabel->setStyleSheet("font-weight: bold;");
    auto* removeBtn = new QPushButton("×");
    removeBtn->setFixedSize(20, 20);
    removeBtn->setStyleSheet("color: red; font-weight: bold;");
    header->addWidget(m_titleLabel);
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

    setStyleSheet("SegmentWidget { border: 1px solid #ccc; border-radius: 4px; }");
}

void SegmentWidget::setIndex(std::size_t index)
{
    m_index = index;
    m_titleLabel->setText(QString("Segment %1").arg(index));
}

SpinnerWidget::SpinnerWidget(std::size_t index, float initialSpeed, QWidget* parent)
    : QWidget(parent)
    , m_index(index)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);

    // Header with title and remove button
    auto* header = new QHBoxLayout();
    m_titleLabel = new QLabel(QString("Spinner %1").arg(index));
    m_titleLabel->setStyleSheet("font-weight: bold; color: #d0421d;");
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

    setStyleSheet("SpinnerWidget { border: 1px solid #d0421d; border-radius: 4px; }");
}

void SpinnerWidget::setIndex(std::size_t index)
{
    m_index = index;
    m_titleLabel->setText(QString("Spinner %1").arg(index));
}

RobotArmControls::RobotArmControls(QWidget* parent)
    : QWidget(parent)
{
    auto* mainLayout = new QVBoxLayout(this);

    // Button layout for adding components
    auto* buttonLayout = new QHBoxLayout();
    auto* addSegmentBtn = new QPushButton("+ Add Segment");
    auto* addSpinnerBtn = new QPushButton("+ Add Spinner");
    addSegmentBtn->setStyleSheet("padding: 8px; font-weight: bold;");
    addSpinnerBtn->setStyleSheet("padding: 8px; font-weight: bold; color: #d0421d;");
    buttonLayout->addWidget(addSegmentBtn);
    buttonLayout->addWidget(addSpinnerBtn);
    mainLayout->addLayout(buttonLayout);

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

    connect(addSegmentBtn, &QPushButton::clicked, this, [this]() {
        addSegmentWidget(1.0f, 0.0f);
        emit segmentAdded(1.0f, 0.0f);
    });

    connect(addSpinnerBtn, &QPushButton::clicked, this, [this]() {
        addSpinnerWidget(0.0f);
        emit spinnerAdded();
    });
}

void RobotArmControls::addSegmentWidget(float length, float angle)
{
    auto* widget = new SegmentWidget(m_componentWidgets.size(), length, angle);

    connect(widget, &SegmentWidget::angleChanged, this, &RobotArmControls::angleChanged);
    connect(widget, &SegmentWidget::lengthChanged, this, &RobotArmControls::lengthChanged);
    connect(widget, &SegmentWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);

    m_componentWidgets.push_back(widget);

    // Insert before the stretch
    m_componentsLayout->insertWidget(m_componentsLayout->count() - 1, widget);
}

void RobotArmControls::addSpinnerWidget(float speed)
{
    auto* widget = new SpinnerWidget(m_componentWidgets.size(), speed);

    connect(widget, &SpinnerWidget::rotationalSpeedChanged, this, &RobotArmControls::rotationalSpeedChanged);
    connect(widget, &SpinnerWidget::removeRequested, this, &RobotArmControls::removeComponentWidget);

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
    // Update indices in all widgets after removal
    for (std::size_t i = 0; i < m_componentWidgets.size(); ++i) {
        // Try casting to SegmentWidget first
        if (auto* segmentWidget = qobject_cast<SegmentWidget*>(m_componentWidgets[i])) {
            segmentWidget->setIndex(i);
        }
        // Try casting to SpinnerWidget
        else if (auto* spinnerWidget = qobject_cast<SpinnerWidget*>(m_componentWidgets[i])) {
            spinnerWidget->setIndex(i);
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