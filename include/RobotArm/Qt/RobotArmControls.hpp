#ifndef OPENGL_TEST_ROBOTARMCONTROLS_HPP
#define OPENGL_TEST_ROBOTARMCONTROLS_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <vector>
#include <functional>

class PistonWidget : public QWidget {
	Q_OBJECT
public:
	explicit PistonWidget(std::size_t index, float maxLength = 5.0f, QWidget* parent = nullptr);

	void setIndex(std::size_t index);
	std::size_t index() const { return m_index; }

signals:
	void targetLengthChanged(std::size_t index, float length);
	void removeRequested(std::size_t index);

private:
	std::size_t m_index;
	QSlider* m_lengthSlider;
	QLabel* m_lengthLabel;
	QLabel* m_titleLabel;
	float m_maxLength;
};

class HingeWidget : public QWidget {
	Q_OBJECT
public:
	explicit HingeWidget(std::size_t index, float initialAngle = 0.0f, QWidget* parent = nullptr);

	void setIndex(std::size_t index);
	std::size_t index() const { return m_index; }

signals:
	void targetAngleChanged(std::size_t index, float angle);
	void removeRequested(std::size_t index);

private:
	std::size_t m_index;
	QSlider* m_angleSlider;
	QLabel* m_angleLabel;
	QLabel* m_titleLabel;
};

class SwivelWidget : public QWidget {
	Q_OBJECT
public:
	explicit SwivelWidget(std::size_t index, float initialSpeed = 0.0f, QWidget* parent = nullptr);

	void setIndex(std::size_t index);
	std::size_t index() const { return m_index; }

signals:
	void rotationalSpeedChanged(std::size_t index, float speed);
	void removeRequested(std::size_t index);

private:
	std::size_t m_index;
	QDoubleSpinBox* m_speedSpinBox;
	QLabel* m_titleLabel;
};

class LinkWidget : public QWidget {
	Q_OBJECT
public:
	explicit LinkWidget(std::size_t index, float length = 1.0f, QWidget* parent = nullptr);

	void setIndex(std::size_t index);
	std::size_t index() const { return m_index; }

signals:
	void removeRequested(std::size_t index);

private:
	std::size_t m_index;
	QLabel* m_titleLabel;
};

class RobotArmControls : public QWidget {
	Q_OBJECT
public:
	explicit RobotArmControls(QWidget* parent = nullptr);

signals:
	void pistonAdded(float maxLength);
	void hingeAdded();
	void swivelAdded();
	void linkAdded(float length);
	void pistonTargetLengthChanged(std::size_t index, float length);
	void hingeTargetAngleChanged(std::size_t index, float angle);
	void swivelRotationalSpeedChanged(std::size_t index, float speed);
	void componentRemoved(std::size_t index);

public slots:
	void addPistonWidget(float maxLength = 5.0f);
	void addHingeWidget(float initialAngle = 0.0f);
	void addSwivelWidget(float speed = 0.0f);
	void addLinkWidget(float length = 1.0f);
	void removeComponentWidget(std::size_t index);
	void clear();

private slots:
	void onComponentTypeChanged();
	void onAddComponentClicked();

private:
	void rebuildIndices();
	void updateParameterVisibility();

	QVBoxLayout* m_componentsLayout;
	std::vector<QWidget*> m_componentWidgets;

	// Component type selection
	QButtonGroup* m_componentTypeGroup;
	QRadioButton* m_pistonRadio;
	QRadioButton* m_hingeRadio;
	QRadioButton* m_swivelRadio;
	QRadioButton* m_linkRadio;

	// Parameter inputs
	QWidget* m_pistonParamsWidget;
	QWidget* m_linkParamsWidget;
	QDoubleSpinBox* m_pistonMaxLengthSpinBox;
	QDoubleSpinBox* m_linkLengthSpinBox;
};

#endif // OPENGL_TEST_ROBOTARMCONTROLS_HPP