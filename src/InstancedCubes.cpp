//
// Created by chris on 12/17/25.
//
#include <QtRendering/InstancedCubes.hpp>
InstancedCubes::InstancedCubes()
{
	glGenVertexArrays(1, &m_cube_vao);
	glGenBuffers(1, &m_cube_vbo);

	glBindVertexArray(m_cube_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);

	// Position attribute (location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute (location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_cube_instance_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_instance_vbo); // Model matrices

	for (int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE,
							  sizeof(InstanceData),
							  (void*)(sizeof(glm::vec4) * i)); // One iteration <=> One column of 4x4 mat
		glVertexAttribDivisor(2 + i, 1); // Each instance has its own model mat
	}

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE,
						  sizeof(InstanceData),
						  (void*)offsetof(InstanceData, color)); // Color
	glVertexAttribDivisor(6, 1); // Each instance gets its own color

	for (int x = -5; x <= 5; x++) {
		for (int z = -5; z <= 5; z++) {
			m_cubes.push_back({
				glm::vec3(x * 2.0f, 0.0f, z * 2.0f),
				glm::normalize(glm::vec3(x, 1, z)),
				0.5f + 0.1f * (x + z),
				glm::vec3(0.4f),
				glm::vec3(
					0.5f + 0.5f * sin(x * 0.5f),
					0.5f + 0.5f * sin(z * 0.5f),
					0.5f + 0.5f * cos((x + z) * 0.3f)
				)
			});
		}
	}
	m_cube_instances.reserve(m_cubes.size());
}
void InstancedCubes::draw(float time)
{
	m_cube_instances.clear();
	for (const auto& cube : m_cubes)
	{
		auto mat = cube.get_model_mat(time);
		auto color = cube.color;
		m_cube_instances.emplace_back(mat, color);
	}
	glBindVertexArray(m_cube_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_cube_instance_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				   m_cube_instances.size() * sizeof(InstanceData),
				   m_cube_instances.data(),
				   GL_DYNAMIC_DRAW);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, m_cubes.size());
}


InstancedCubes::~InstancedCubes() {
	// Clean up GL resources
	glDeleteVertexArrays(1, &m_cube_vao);
	glDeleteBuffers(1, &m_cube_vbo);
	glDeleteBuffers(1, &m_cube_instance_vbo);
}