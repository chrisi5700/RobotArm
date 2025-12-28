//
// Created by chris on 12/21/25.
//
#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <ranges>
#include <RobotArm/Simulation/Simulation.hpp>
#include <utility>

// TODO Rework this, Piston (change length), Swivel (change Y-Axis rot), Hinge (change X-Axis rot), Link (fixed length)

void Piston::tick(float dt)
{
	float delta = target_length - current_length;
	current_length += std::clamp(delta, -PISTON_SPEED * dt, PISTON_SPEED * dt);
	current_length = std::clamp(current_length, MIN_LENGTH, max_length);
}
ModelMatrixChainOutput Piston::get_model_matrix(glm::mat4 joint_matrix) const
{
	// Translate since the scale stretches the segment in both directions
	glm::mat4 segment = glm::translate(joint_matrix, glm::vec3(0, current_length / 2.0f, 0));
	// Actually scale it to length (this kind of assumes a normalized length but thats fine)
	segment = glm::scale(segment, glm::vec3(0.3f, current_length, 0.3f));
	// Move joint to top of scaled segment
	joint_matrix = glm::translate(joint_matrix, glm::vec3(0, current_length, 0));
	return {segment, joint_matrix};
}
void Hinge::tick(float dt)
{
	float delta = target_angle - current_angle;
	current_angle += std::clamp(delta, -ROTATION_SPEED * dt, ROTATION_SPEED * dt);
}
ModelMatrixChainOutput Hinge::get_model_matrix(glm::mat4 joint_matrix) const
{
	joint_matrix = glm::rotate(joint_matrix, current_angle, glm::vec3(0, 0, 1));
	return {joint_matrix, joint_matrix};
}

void Swivel::tick(float dt)
{
	angle = std::fmod(angle + rotational_speed * dt, 3.141592f * 2.0f); // No overflow by staying in [0, 2pi)
}
ModelMatrixChainOutput Swivel::get_model_matrix(glm::mat4 joint_matrix) const
{
	// Just attached and scaled
	auto model_matrix = glm::scale(joint_matrix, {0.35f, 0.3f, 0.35f});

	// Similar to Segment but this rotates vertically
	joint_matrix = glm::translate(joint_matrix, {0.0f, 0.15f, 0.0f});
	joint_matrix = glm::rotate(joint_matrix, angle, glm::vec3(0, 1, 0));

	return {model_matrix, joint_matrix};
}
ModelMatrixChainOutput Link::get_model_matrix(glm::mat4 joint_matrix) const
{
	// Translate since the scale stretches the segment in both directions
	glm::mat4 segment = glm::translate(joint_matrix, glm::vec3(0, length / 2.0f, 0));
	// Actually scale it to length (this kind of assumes a normalized length but thats fine)
	segment = glm::scale(segment, glm::vec3(0.3f, length, 0.3f));
	// Move joint to top of scaled segment
	joint_matrix = glm::translate(joint_matrix, glm::vec3(0, length, 0));
	return {segment, joint_matrix};
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
	if (std::holds_alternative<Piston>(component))
		return ComponentType::Piston;
	if (std::holds_alternative<Hinge>(component))
		return ComponentType::Hinge;
	if (std::holds_alternative<Swivel>(component))
		return ComponentType::Swivel;
	if (std::holds_alternative<Link>(component))
		return ComponentType::Link;
	std::unreachable();
}

glm::vec3 get_translation(const glm::mat4& m)
{
	return glm::vec3(m[3]);
}
glm::mat3 get_rotation(const glm::mat4& m)
{
	return glm::mat3(m);
}

RenderData Simulation::get_render_data() const
{
	RenderData out{{}, glm::vec3{0.0f}, glm::vec3{0.0f}};

	out.components.reserve(m_components.size()); // Id prefer not to allocate at all

	struct AngularComponent
	{
		glm::vec3 pos;
		glm::vec3 rot_axis;
		float	  angular_velocity;
	};
	std::vector<AngularComponent> angular_components;

	glm::mat4 joint_matrix{1.0f};
	for (const auto& component : m_components)
	{
		auto [model, joint] = component.get_model_matrix(joint_matrix);
		joint_matrix		= joint;
		auto component_type = to_enum(component);
		out.components.emplace_back(component_type, model);
		// TODO reconsider choices
		out.tip_pos = get_translation(joint_matrix);
		if (component_type == ComponentType::Piston)
		{
			auto piston = std::get<Piston>(component);
			if (piston.current_length == piston.target_length)
				continue; // Not moving if we reached target
			auto speed =
				(static_cast<float>(piston.current_length < piston.target_length) - 0.5f) * 2.0f * Piston::PISTON_SPEED;
			out.tip_vel += get_rotation(joint_matrix) * glm::vec3{0, speed, 0};
		}
		else if (component_type == ComponentType::Swivel)
		{
			auto swivel = std::get<Swivel>(component);
			if (swivel.rotational_speed == 0)
				continue; // Not moving if we reached target
			angular_components.emplace_back(get_translation(joint_matrix),
											get_rotation(joint_matrix) * glm::vec3{0, 1, 0}, swivel.rotational_speed);
		}
		else if (component_type == ComponentType::Hinge)
		{
			auto hinge = std::get<Hinge>(component);
			if (hinge.current_angle == hinge.target_angle)
				continue; // Not moving if we reached target
			float sign = (hinge.target_angle > hinge.current_angle) ? 1.0f : -1.0f;
			angular_components.emplace_back(get_translation(joint_matrix),
											get_rotation(joint_matrix) * glm::vec3{0, 0, 1},
											sign * Hinge::ROTATION_SPEED);
		}
	}
	for (auto angular_component : angular_components)
	{
		auto omega = angular_component.rot_axis * angular_component.angular_velocity;
		auto r	   = out.tip_pos - angular_component.pos;
		out.tip_vel += glm::cross(omega, r); // We spin most really far out and at a 90° angle for example
	}
	return out;
}
std::vector<ComponentType> Simulation::get_component_types() const
{
	namespace v = std::views;
	namespace r = std::ranges;
	return m_components | v::transform(to_enum) | r::to<std::vector>();
}
void Simulation::set_piston_target_length(std::size_t idx, float f)
{
	auto& piston = std::get<Piston>(m_components.at(idx));
	assert(piston.max_length >= f);
	assert(Piston::MIN_LENGTH <= f);
	piston.target_length = f;
}
void Simulation::set_hinge_target_angle(std::size_t idx, float f)
{
	auto& hinge		   = std::get<Hinge>(m_components.at(idx));
	hinge.target_angle = f;
}
void Simulation::set_swivel_rotation_speed(std::size_t idx, float f)
{
	auto& swivel			= std::get<Swivel>(m_components.at(idx));
	swivel.rotational_speed = f;
}
void Simulation::remove_component(std::size_t idx)
{
	m_components.erase(m_components.begin() + idx);
}
void Simulation::add_piston(float max_length)
{
	m_components.emplace_back(Piston{Piston::MIN_LENGTH, Piston::MIN_LENGTH, max_length});
}
void Simulation::add_hinge()
{
	m_components.emplace_back(Hinge{0, 0});
}
void Simulation::add_swivel()
{
	m_components.emplace_back(Swivel{0, 0});
}
void Simulation::add_link(float length)
{
	m_components.emplace_back(Link{length});
}
