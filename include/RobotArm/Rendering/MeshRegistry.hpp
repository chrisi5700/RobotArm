//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_SHADERREGISTRY_HPP
#define ROBOTARM_SHADERREGISTRY_HPP
#include "Mesh.hpp"

class MeshRegistry {
	std::unordered_map<MeshId, Mesh> m_meshes;

public:
	void load(MeshId id, std::span<const Vertex> vertices, std::span<const uint32_t> indices);
	Mesh& get(MeshId id);
};
#endif // ROBOTARM_SHADERREGISTRY_HPP
