//
// Created by chris on 12/19/25.
//
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QtRendering/Camera.hpp>
#include <algorithm>

glm::mat4 Camera::get_view() const
{
	auto camera_pos = get_position();
	return  glm::lookAt(camera_pos + m_target, glm::vec3{0, 0, 0}, glm::vec3(0, 1, 0));
}
glm::mat4 Camera::get_projection() const
{
	return glm::perspective(glm::radians(m_fov), m_aspect_ratio, 0.1f, 100.0f);
}
glm::vec3 Camera::get_position() const
{
	glm::vec3 camera_pos;
	camera_pos.x = m_distance * std::cos(m_elevation) * std::sin(m_azimuth);
	camera_pos.y = m_distance * std::sin(m_elevation);
	camera_pos.z = m_distance * std::cos(m_elevation) * std::cos(m_azimuth);
	return camera_pos;
}
void Camera::update_aspect_ratio(int width, int height)
{
	m_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
}
void Camera::drag_camera(glm::vec2 delta)
{
	static constexpr float SENSITIVITY = 0.005f; // Radians per pixel
	static constexpr float PI = 3.14159265358979323846f;
	m_azimuth += delta.x * SENSITIVITY;											// Horizontal angle
	m_elevation += delta.y * SENSITIVITY;										// Vertical angle
	m_elevation = glm::clamp(m_elevation, 0.0f, PI / 2 - 0.1f); // Prevent flip
}
void Camera::change_camera_distance(float delta)
{
	m_distance = std::max(m_distance + delta, 1.0f);
}
void Camera::draw()
{
	auto view = get_view();
	auto projection = get_projection();
	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
}