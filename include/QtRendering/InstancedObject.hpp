//
// Created by chris on 12/18/25.
//

#ifndef OPENGL_TEST_INSTANCEDOBJECT_HPP
#define OPENGL_TEST_INSTANCEDOBJECT_HPP
#include "GLCommon.hpp"
#include "Drawable.hpp"

struct InstanceData {
	glm::mat4 model;
	glm::vec3 color;
	float padding = 0.0f;
};

class InstancedObject : public Drawable
{
public:
	InstancedObject(const std::vector<Vertex>& verts);
	InstancedObject(const MeshData& mesh);
	~InstancedObject();

	std::vector<InstanceData>& get_instance_data();
	void draw(float time) override;

private:
	void setup_vao(const std::vector<Vertex>& verts, const std::vector<uint32_t>& indices);

	std::vector<InstanceData> m_instances;

	GLuint m_vao = 0;
	GLuint m_vert_vbo = 0;
	GLuint m_ebo = 0;
	GLuint m_instance_vbo = 0;

	GLsizei m_vertex_count = 0;
	GLsizei m_index_count = 0;  // 0 = non-indexed
};
#endif // OPENGL_TEST_INSTANCEDOBJECT_HPP
