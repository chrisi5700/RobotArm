//
// Created by chris on 12/21/25.
//
#include <RobotArm/Rendering/MeshRegistry.hpp>

Mesh& MeshRegistry::get(MeshId id)
{
	return m_meshes.at(id);
}
void MeshRegistry::load(MeshId id, std::span<const Vertex> vertices, std::span<const uint32_t> indices)
{
	Mesh mesh{vertices, indices};
	m_meshes.insert({id, std::move(mesh)});
}