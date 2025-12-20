//
// Created by chris on 12/19/25.
//

#ifndef OPENGL_TEST_CAMERA_HPP
#define OPENGL_TEST_CAMERA_HPP
#include "GLCommon.hpp"

class Camera
{
	float m_azimuth = 0;
	float m_elevation = 0;
	float m_distance = 10;
	float m_fov = 90;
	float m_aspect_ratio = 16.0f/9.0f;
	glm::vec3 m_target = {0,0,0};
	GLint m_view_loc = 0;
	GLint m_proj_loc = 0;

public:
	Camera(GLint view_loc, GLint proj_loc) : m_view_loc(view_loc), m_proj_loc(proj_loc) {}
	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;
	glm::vec3 get_position() const;
	void update_aspect_ratio(int width, int height);
	void drag_camera(glm::vec2 delta);
	void change_camera_distance(float delta);
	void draw();
};

#endif // OPENGL_TEST_CAMERA_HPP
