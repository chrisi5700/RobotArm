//
// Created by chris on 12/21/25.
//
#include <RobotArm/Rendering/Mesh.hpp>
#include <utility>

Mesh::Mesh(std::span<const Vertex> verts, std::span<const uint32_t> indices)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_instance_vbo);

	glBindVertexArray(m_vao);

	m_vert_count = verts.size();
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	if (!indices.empty())
	{
		m_index_count = indices.size();
		glGenBuffers(1, &m_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);

	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(2 + i, 1);
	}

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_instance_vbo);
	if (m_ebo) {
		glDeleteBuffers(1, &m_ebo);
	}
}

Mesh::Mesh(Mesh&& other) noexcept
	: m_vao(std::exchange(other.m_vao, 0))
	, m_vbo(std::exchange(other.m_vbo, 0))
	, m_vert_count(std::exchange(other.m_vert_count, 0))
	, m_ebo(std::exchange(other.m_ebo, 0))
	, m_index_count(std::exchange(other.m_index_count, 0))
	, m_instance_vbo(std::exchange(other.m_instance_vbo, 0))
{
}
Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	using std::swap;
	swap(*this, other);
	return *this;
}
void Mesh::upload_instances(std::span<const InstanceData> instance_data)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, instance_data.size() * sizeof(InstanceData),
				 instance_data.data(), GL_DYNAMIC_DRAW);
}
void Mesh::draw(std::size_t instance_count) const
{

	if (instance_count == 0) return;
	glBindVertexArray(m_vao);

	if (m_index_count > 0) {
		glDrawElementsInstanced(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0, instance_count);
	} else {
		glDrawArraysInstanced(GL_TRIANGLES, 0, m_vert_count, instance_count);
	}
}