//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_SIMULATION_HPP
#define ROBOTARM_SIMULATION_HPP
#include <glm/glm.hpp>
#include <variant>
#include <vector>

struct ModelMatrixChainOutput
{
	glm::mat4 model_matrix; // translation, rotation and scale of current component
	glm::mat4 joint_matrix; // translation and rotation of where the next component is attached to
};


// Extends or retracts
struct Piston
{
	static constexpr float PISTON_SPEED = 10;
	static constexpr float MIN_LENGTH = 1.0f;
	float current_length = 1.0f;
	float target_length = 1.0f;
	float max_length = 1.0f;

	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};


struct Hinge
{
	static constexpr float ROTATION_SPEED = 3.1415f / 2.0f; // 90° / sec
	float current_angle = 0.0f;
	float target_angle = 0.0f;
	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};


// Rotates radially
struct Swivel
{
	float rotational_speed = 0.0f;
	float angle = 0.0f;

	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};


// Fixed size link
struct Link
{
	float length;
	void tick(float) {}
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;

};

using ComponentBase = std::variant<Piston, Hinge, Swivel, Link>;

class Component : public ComponentBase
{
public:
	using ComponentBase::ComponentBase;
	void tick(float dt);
	[[nodiscard]] ModelMatrixChainOutput get_model_matrix(glm::mat4 joint_matrix) const;
};

enum class ComponentType {Piston, Hinge, Swivel, Link};

struct RenderData
{
	std::vector<std::pair<ComponentType, glm::mat4>> components;
	glm::vec3 tip_pos;
	glm::vec3 tip_vel;
};

class Simulation
{
	std::vector<Component> m_components;
public:
	void tick(float dt);
	[[nodiscard]] RenderData get_render_data() const;
	[[nodiscard]] std::vector<ComponentType> get_component_types() const;

	// Must be bounded between max_length and MIN_LENGTH
	void set_piston_target_length(std::size_t idx, float f);
	void set_hinge_target_angle(std::size_t idx, float f);
	void set_swivel_rotation_speed(std::size_t idx, float f);
	void remove_component(std::size_t idx);

	void add_piston(float max_length);
	void add_hinge();
	void add_swivel();
	void add_link(float length);
};

#endif // ROBOTARM_SIMULATION_HPP
