//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_MESH_HPP
#define ROBOTARM_MESH_HPP
#include <span>

#include "GLCommon.hpp"



class Mesh {
	GLuint m_vao{};
	GLuint m_vbo{};          // vertex data (pos, normal, uv)
	GLsizei m_vert_count{};
	GLuint m_ebo{};          // indices
	GLsizei m_index_count{};
	GLuint m_instance_vbo{}; // per-instance data (model matrices)

	 public:
	Mesh(std::span<const Vertex> vertices, std::span<const uint32_t> indices);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;
	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void upload_instances(std::span<const InstanceData>);
	void draw(std::size_t instance_count) const;
};

#endif // ROBOTARM_MESH_HPP
