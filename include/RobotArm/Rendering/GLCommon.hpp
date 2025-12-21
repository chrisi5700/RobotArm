//
// Created by chris on 12/18/25.
//

#ifndef OPENGL_TEST_GLCOMMON_HPP
#define OPENGL_TEST_GLCOMMON_HPP
#ifdef __EMSCRIPTEN__
	#include <GLES3/gl3.h>
#else
	#include <glad/glad.h>
#endif
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <vector>




struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};



struct InstanceData {
	glm::mat4 model;
	glm::vec3 color;
	float padding = 0.0f;
};
struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};
MeshData generate_cube();
MeshData generate_sphere(float radius, uint32_t latSegments, uint32_t lonSegments);
MeshData generate_cylinder(float radius, float height, uint32_t segments, bool caps = true);
enum class MeshId
{
	Sphere,
	Cube,
	Cylinder,
};

#endif // OPENGL_TEST_GLCOMMON_HPP
