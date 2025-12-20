#ifndef OPENGL_TEST_ROBOTARMCONTROLS_HPP
#define OPENGL_TEST_ROBOTARMCONTROLS_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QDoubleSpinBox>
#include <vector>
#include <functional>

class ComponentWidget : public QWidget {
	Q_OBJECT
public:
	explicit ComponentWidget(std::size_t index, float initialLength, float initialAngle, QWidget* parent = nullptr);

	signals:
		void angleChanged(std::size_t index, float angle);
	void lengthChanged(std::size_t index, float length);
	void removeRequested(std::size_t index);

private:
	std::size_t m_index;
	QSlider* m_angleSlider;
	QDoubleSpinBox* m_lengthSpinBox;
	QLabel* m_angleLabel;
};

class RobotArmControls : public QWidget {
	Q_OBJECT
public:
	explicit RobotArmControls(QWidget* parent = nullptr);

	signals:
		void componentAdded(float length, float angle);
	void angleChanged(std::size_t index, float angle);
	void lengthChanged(std::size_t index, float length);
	void componentRemoved(std::size_t index);

public slots:
	void addComponentWidget(float length = 1.0f, float angle = 0.0f);
	void removeComponentWidget(std::size_t index);
	void clear();

private:
	void rebuildIndices();

	QVBoxLayout* m_componentsLayout;
	std::vector<ComponentWidget*> m_componentWidgets;
};

#endif // OPENGL_TEST_ROBOTARMCONTROLS_HPP