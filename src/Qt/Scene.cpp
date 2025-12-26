//
// Created by chris on 12/17/25.
//
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <print>
#include <RobotArm/Qt/Scene.hpp>
#include <utility>

MeshId mesh_for(ComponentType type)
{
	switch (type)
	{
		case ComponentType::Link: return MeshId::Cube;
		case ComponentType::Piston: return MeshId::Cylinder;
		case ComponentType::Swivel: return MeshId::Cylinder;
		case ComponentType::Hinge: return MeshId::Sphere;
	}
	std::unreachable();
}

glm::vec3 color_for(ComponentType type)
{
	switch (type)
	{
		case ComponentType::Link: return glm::vec3(122 / 255.f,122 / 255.f,122 / 255.f);
		case ComponentType::Piston: return glm::vec3(122 / 255.f,230 / 255.f,100 / 255.f);
		case ComponentType::Swivel: return glm::vec3(215 / 255.f,153 / 255.f,33 / 255.f);
		case ComponentType::Hinge: return glm::vec3(204 / 255.f,36 / 255.f,29 / 255.f);
	}
	std::unreachable();
}


void Scene::submit_to(RenderQueue& queue) const
{
	queue.submit(
		{
			MeshId::Sphere,
			{
				glm::scale(glm::mat4{1}, glm::vec3(0.85f, 0.85f, 0.85f)),
				glm::vec3(122 / 255.f,122 / 255.f,122 / 255.f)
			}
		});
	auto render_data = m_simulation.get_render_data();
	for (const auto& [type, model]  : render_data.components)
	{
		queue.submit({
			mesh_for(type),
			InstanceData {
				model,
				color_for(type)
			},
		});
	}
	glm::vec3 vel = render_data.tip_vel;
	float magnitude = glm::length(vel);

	if (magnitude > 0.001f)
	{
		glm::vec3 dir = vel / magnitude;
		glm::vec3 up{0, 1, 0};

		glm::mat4 rotation{1.0f};
		glm::vec3 axis = glm::cross(up, dir);
		float axis_len = glm::length(axis);

		if (axis_len > 0.001f)
		{
			// Normal case: rotate around the perpendicular axis
			float angle = glm::acos(glm::clamp(glm::dot(up, dir), -1.0f, 1.0f));
			rotation = glm::rotate(glm::mat4{1.0f}, angle, axis / axis_len);
		}
		else if (dir.y < 0)
		{
			// Velocity pointing straight down, flip 180°
			rotation = glm::rotate(glm::mat4{1.0f}, glm::pi<float>(), glm::vec3{1, 0, 0});
		}
		// else: pointing straight up, no rotation needed

		glm::mat4 model = glm::translate(glm::mat4{1.0f}, render_data.tip_pos);
		model = model * rotation;
		model = glm::scale(model, glm::vec3{0.05f, magnitude, 0.05f});

		queue.submit({
			MeshId::Arrow,
			InstanceData{model, glm::vec3{0.2f, 0.8f, 0.8f}}
		});
	}
}
Camera& Scene::get_camera()
{
	return m_camera;
}
Simulation& Scene::get_simulation()
{
	return m_simulation;
}