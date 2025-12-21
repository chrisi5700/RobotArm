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


public:
	Camera() =  default;
	// TODO Probably cache these
	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;
	glm::vec3 get_position() const;
	void update_aspect_ratio(int width, int height);
	void drag_camera(glm::vec2 delta);
	void change_camera_distance(float delta);
};

#endif // OPENGL_TEST_CAMERA_HPP
