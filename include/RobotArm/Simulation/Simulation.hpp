//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_SIMULATION_HPP
#define ROBOTARM_SIMULATION_HPP
#include <glm/glm.hpp>
#include <variant>

struct ModelMatrixChainOutput
{
	glm::mat4 model_matrix; // translation, rotation and scale of current component
	glm::mat4 joint_matrix; // translation and rotation of where the next component is attached to
};

struct Segment
{
	static constexpr float ROTATION_SPEED = 3.1415f / 2.0f; // 90° / sec
	float length = 1.0f;
	float angle = 0.0f;
	float target_angle = 0.0f;

	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};



struct Spinner
{
	float rotational_speed = 0.0f;
	float angle = 0.0f;

	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};

using ComponentBase = std::variant<Segment, Spinner>;

class Component : public ComponentBase
{
public:
	using ComponentBase::ComponentBase;
	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};

enum class ComponentType { Segment, Joint, Spinner };

struct RenderData
{
	ComponentType type;
	glm::mat4 model_matrix;
};

class Simulation
{
	std::vector<Component> m_components;
public:
	void tick(float dt);
	[[nodiscard]] std::vector<RenderData> get_render_data() const;
	[[nodiscard]] std::vector<ComponentType> get_component_types() const;
	void add_segment(float length, float angle);
	void add_spinner();
	void set_segment_target_angle(std::size_t id, float angle);
	void set_segment_length(std::size_t id, float length);
	void set_spinner_rotational_speed(std::size_t id, float speed);
	void remove_component(std::size_t id);
};

#endif // ROBOTARM_SIMULATION_HPP
