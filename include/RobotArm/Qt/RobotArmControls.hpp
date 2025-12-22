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

class SegmentWidget : public QWidget {
	Q_OBJECT
public:
	explicit SegmentWidget(std::size_t index, float initialLength, float initialAngle, QWidget* parent = nullptr);

	void setIndex(std::size_t index);
	std::size_t index() const { return m_index; }

signals:
	void angleChanged(std::size_t index, float angle);
	void lengthChanged(std::size_t index, float length);
	void removeRequested(std::size_t index);

private:
	std::size_t m_index;
	QSlider* m_angleSlider;
	QDoubleSpinBox* m_lengthSpinBox;
	QLabel* m_angleLabel;
	QLabel* m_titleLabel;
};

class SpinnerWidget : public QWidget {
	Q_OBJECT
public:
	explicit SpinnerWidget(std::size_t index, float initialSpeed = 0.0f, QWidget* parent = nullptr);

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

class RobotArmControls : public QWidget {
	Q_OBJECT
public:
	explicit RobotArmControls(QWidget* parent = nullptr);

signals:
	void segmentAdded(float length, float angle);
	void spinnerAdded();
	void angleChanged(std::size_t index, float angle);
	void lengthChanged(std::size_t index, float length);
	void rotationalSpeedChanged(std::size_t index, float speed);
	void componentRemoved(std::size_t index);

public slots:
	void addSegmentWidget(float length = 1.0f, float angle = 0.0f);
	void addSpinnerWidget(float speed = 0.0f);
	void removeComponentWidget(std::size_t index);
	void clear();

private:
	void rebuildIndices();

	QVBoxLayout* m_componentsLayout;
	std::vector<QWidget*> m_componentWidgets;
};

#endif // OPENGL_TEST_ROBOTARMCONTROLS_HPP