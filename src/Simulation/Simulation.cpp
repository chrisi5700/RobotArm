//
// Created by chris on 12/21/25.
//
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <RobotArm/Simulation/Simulation.hpp>
#include <utility>
#include <ranges>

void Segment::tick(float dt)
{
	float delta = target_angle - angle;
	angle += std::clamp(delta, -ROTATION_SPEED * dt, ROTATION_SPEED * dt);
}
ModelMatrixChainOutput Segment::get_model_matrix(glm::mat4 joint_matrix) const
{
	// Rotation effects everything down the chain
	joint_matrix = glm::rotate(joint_matrix, angle, glm::vec3(0, 0, 1));
	// Translate since the scale stretches the segment in both directions
	glm::mat4 segment = glm::translate(joint_matrix, glm::vec3(0, length / 2.0f, 0));
	// Actually scale it to length (this kind of assumes a normalized length but thats fine)
	segment = glm::scale(segment, glm::vec3(0.3f, length, 0.3f));
	// Move joint to top of scaled segment
	joint_matrix = glm::translate(joint_matrix, glm::vec3(0, length, 0));
	return {segment, joint_matrix};
}

void Spinner::tick(float dt)
{
	angle = std::fmod(angle + rotational_speed * dt, 3.141592f * 2.0f); // No overflow by staying in [0, 2pi]
}
ModelMatrixChainOutput Spinner::get_model_matrix(glm::mat4 joint_matrix) const
{
	// Just attached and scaled
	auto model_matrix = glm::scale(joint_matrix, {0.35f,0.3f,0.35f});

	// Similar to Segment but this rotates vertically
	joint_matrix = glm::translate(joint_matrix, {0.0f, 0.15f, 0.0f});
	joint_matrix = glm::rotate(joint_matrix, angle, glm::vec3(0, 1, 0));

	return {model_matrix, joint_matrix};
}
void Component::tick(float dt)
{
	std::visit([=](auto& held) { return held.tick(dt); }, *this);
}
ModelMatrixChainOutput Component::get_model_matrix(glm::mat4 joint_matrix) const
{
	return std::visit([=](const auto& held) { return held.get_model_matrix(joint_matrix); }, *this);
}
void Simulation::tick(float dt)
{
	for (auto& component : m_components)
	{
		component.tick(dt);
	}
}

ComponentType to_enum(const Component& component)
{
	if (std::holds_alternative<Segment>(component))
		return ComponentType::Segment;
	if (std::holds_alternative<Spinner>(component))
		return ComponentType::Spinner;
	std::unreachable();
}

std::vector<RenderData> Simulation::get_render_data() const
{
	std::vector<RenderData> out;
	out.reserve(m_components.size()); // Id prefer not to allocate at all
	glm::mat4 joint_matrix{1.0f};
	for (const auto& component : m_components)
	{
		auto [model, joint] = component.get_model_matrix(joint_matrix);
		joint_matrix		= joint;
		out.emplace_back(to_enum(component), model);
		out.emplace_back(ComponentType::Joint, joint);
	}
	return out;
}
std::vector<ComponentType> Simulation::get_component_types() const
{
	namespace v = std::views;
	namespace r = std::ranges;
	return m_components | v::transform(to_enum) | r::to<std::vector>();
}
void Simulation::add_segment(float length, float angle)
{
	m_components.push_back(Segment{length, angle, angle});
}
void Simulation::add_spinner()
{
	m_components.push_back(Spinner{});
}
void Simulation::set_segment_target_angle(std::size_t id, float angle)
{
	std::get<Segment>(m_components[id]).target_angle = angle;
}
void Simulation::set_segment_length(std::size_t id, float length)
{
	std::get<Segment>(m_components[id]).length = length;
}
void Simulation::set_spinner_rotational_speed(std::size_t id, float speed)
{
	std::get<Spinner>(m_components[id]).rotational_speed = speed;
}
void Simulation::remove_component(std::size_t id)
{
	m_components.erase(m_components.begin() + id); // Just component
}
