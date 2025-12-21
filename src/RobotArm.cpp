//
// Created by chris on 12/18/25.
//
#include <../include/RobotArm/Rendering/GLCommon.hpp>
#include <RobotArm/RobotArm.hpp>

RobotArm::RobotArm() : m_segments(generate_cube()), m_joints(generate_sphere(0.3f, 20, 20))
{}
void RobotArm::add_component(float length, float angle)
{
	m_components.emplace_back(length, angle, angle);
}
void RobotArm::set_target_angle(std::size_t component_idx, float angle)
{
	assert(component_idx < m_components.size());
	m_components[component_idx].target_angle = angle;
}
void RobotArm::set_length(std::size_t component_idx, float length)
{
	assert(component_idx < m_components.size());
	m_components[component_idx].length = length;
}

void RobotArm::remove_component(std::size_t index)
{
	if (index < m_components.size()) {
		m_components.erase(m_components.begin() + index);
	}
}
void RobotArm::build_instance_data()
{
	auto& cube_instances = m_segments.get_instance_data();
	cube_instances.clear();
	auto& sphere_instances = m_joints.get_instance_data();
	sphere_instances.clear();


	// This tracks "where is the current joint and what direction are we facing"
	glm::mat4 joint = glm::mat4(1.0f);

	for (const auto& component : m_components)
	{
		// 1. Rotate at this joint (this affects everything downstream)
		joint = glm::rotate(joint, component.current_angle, glm::vec3(0, 0, 1));

		// 2. Build the segment's model matrix:
		//    - Start from current joint position/orientation
		//    - Move half-length forward (so segment starts at joint, not centered on it)
		//    - Scale to make it the right size
		glm::mat4 segment = glm::translate(joint, glm::vec3(0, component.length / 2.0f, 0));
		segment = glm::scale(segment, glm::vec3(0.3f, component.length, 0.3f));

		cube_instances.emplace_back(segment, glm::vec3(142 / 255.f, 192 / 255.f, 124 / 255.f));
																	// ^^ Gruvbox Green

		// 3. Move joint to the END of this segment (for next iteration)
		sphere_instances.emplace_back(joint, glm::vec3(204 / 255.f,36 / 255.f,29 / 255.f));
																	// ^^ Gruvbox Red
		joint = glm::translate(joint, glm::vec3(0, component.length, 0));
	}
}

void RobotArm::update(float time)
{
	if (last_update == 0.0f)
	{
		last_update = time;
		return;
	}
	auto delta = time - last_update;
	last_update = time;
	for (auto& component : m_components)
	{
		if (component.current_angle == component.target_angle) continue;
		if (component.target_angle < component.current_angle)
		{
			component.current_angle -= std::min(component.current_angle-component.target_angle, delta * 3.1415f/8.0f);
		} else
		{
			component.current_angle += std::min(component.target_angle-component.current_angle, delta * 3.1415f/8.0f);
		}
	}
}
void RobotArm::draw(float time)
{
	update(time);
	build_instance_data();
	m_joints.draw(time);
	m_segments.draw(time);
}