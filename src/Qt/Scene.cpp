//
// Created by chris on 12/17/25.
//
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <RobotArm/Qt/Scene.hpp>
#include <utility>

MeshId mesh_for(ComponentType type)
{
	switch (type)
	{
		case ComponentType::Segment: return MeshId::Cube;
		case ComponentType::Joint: return MeshId::Sphere;
		case ComponentType::Spinner: return MeshId::Cylinder;
	}
	std::unreachable();
}

glm::vec3 color_for(ComponentType type)
{
	switch (type)
	{
		case ComponentType::Segment: return glm::vec3(142 / 255.f,192 / 255.f,124 / 255.f);
		case ComponentType::Joint: return glm::vec3(215 / 255.f,153 / 255.f,33 / 255.f);
		case ComponentType::Spinner: return glm::vec3(204 / 255.f,36 / 255.f,29 / 255.f);
	}
	std::unreachable();
}


void Scene::submit_to(RenderQueue& queue) const
{
	for (const auto& data : m_simulation.get_render_data())
	{
		queue.submit({
			mesh_for(data.type),
			InstanceData {
				data.model_matrix,
				color_for(data.type)
			},
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