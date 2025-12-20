//
// Created by chris on 12/17/25.
//
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <qcontainerinfo.h>
#include <QtRendering/RobotArm.hpp>
#include <QtRendering/Scene.hpp>

GLuint compile_shader(GLenum type, const char* source, GLint source_length)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, &source_length);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char log[512];
		glGetShaderInfoLog(shader, 512, nullptr, log);
		std::cerr << "Shader compilation failed:\n" << log << std::endl;
	}
	return shader;
}
GLuint shader_from_file(GLenum type, std::filesystem::path filename)
{
	std::ifstream				   shader_file{filename};
	std::istreambuf_iterator<char> it{shader_file};
	std::string					   shader_text{it, std::istreambuf_iterator<char>()};
	return compile_shader(type, shader_text.c_str(), shader_text.length());
}

GLint create_program(std::filesystem::path vert_filename, std::filesystem::path frag_filename)
{

	auto  vert_shader = shader_from_file(GL_VERTEX_SHADER, vert_filename);
	auto  frag_shader = shader_from_file(GL_FRAGMENT_SHADER, frag_filename);
	GLint program	  = glCreateProgram();
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
	return program;
}

void Scene::upload_shader_params()
{
	glUseProgram(m_program);

	// Lighting
	glUniform3fv(glGetUniformLocation(m_program, "lightColor"), 1, glm::value_ptr(m_params.lightColor));
	glUniform1f(glGetUniformLocation(m_program, "lightIntensity"), m_params.lightIntensity);
	glUniform1f(glGetUniformLocation(m_program, "ambientStrength"), m_params.ambientStrength);
	glUniform1f(glGetUniformLocation(m_program, "diffuseStrength"), m_params.diffuseStrength);
	glUniform1f(glGetUniformLocation(m_program, "specularStrength"), m_params.specularStrength);
	glUniform1f(glGetUniformLocation(m_program, "shininess"), m_params.shininess);

	// Fresnel
	glUniform1i(glGetUniformLocation(m_program, "enableFresnel"), m_params.enableFresnel);
	glUniform1f(glGetUniformLocation(m_program, "fresnelBias"), m_params.fresnelBias);
	glUniform1f(glGetUniformLocation(m_program, "fresnelScale"), m_params.fresnelScale);
	glUniform1f(glGetUniformLocation(m_program, "fresnelPower"), m_params.fresnelPower);

	// Rim
	glUniform1i(glGetUniformLocation(m_program, "enableRimLight"), m_params.enableRimLight);
	glUniform3fv(glGetUniformLocation(m_program, "rimColor"), 1, glm::value_ptr(m_params.rimColor));
	glUniform1f(glGetUniformLocation(m_program, "rimPower"), m_params.rimPower);
	glUniform1f(glGetUniformLocation(m_program, "rimStrength"), m_params.rimStrength);

	// Fog
	glUniform1i(glGetUniformLocation(m_program, "enableHeightFog"), m_params.enableFog);
	glUniform3fv(glGetUniformLocation(m_program, "fogColor"), 1, glm::value_ptr(m_params.fogColor));
	glUniform1f(glGetUniformLocation(m_program, "fogDensity"), m_params.fogDensity);
	glUniform1f(glGetUniformLocation(m_program, "fogHeightFalloff"), m_params.fogHeightFalloff);

	// Gamma
	glUniform1i(glGetUniformLocation(m_program, "enableGammaCorrection"), m_params.enableGamma);
	glUniform1f(glGetUniformLocation(m_program, "gamma"), m_params.gamma);
}

Scene::Scene() :
	m_program(create_program(SHADER_DIR / "vert.glsl", SHADER_DIR / "frag.glsl")),
	m_background_program(create_program(SHADER_DIR / "background_vert.glsl", SHADER_DIR / "background_frag.glsl")),
	// m_floor_program(create_program(SHADER_DIR/"grid_floor_vert.glsl", SHADER_DIR / "grid_floor_frag.glsl")),
	m_light_pos_loc(glGetUniformLocation(m_program, "lightPos")),
	m_view_pos_loc(glGetUniformLocation(m_program, "viewPos")),
	m_camera(glGetUniformLocation(m_program, "view"), glGetUniformLocation(m_program, "projection"))
{

	glGenVertexArrays(1, &m_empty_vao);


}
void Scene::draw(float time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. Background gradient (no depth write)
	glDisable(GL_DEPTH_TEST);
	glUseProgram(m_background_program);
	glUniform3fv(glGetUniformLocation(m_background_program, "topColor"), 1, glm::value_ptr(m_params.topColor));
	glUniform3fv(glGetUniformLocation(m_background_program, "bottomColor"), 1, glm::value_ptr(m_params.bottomColor));
	glBindVertexArray(m_empty_vao);  // Empty VAO for fullscreen triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);


	// 2. Scene with depth testing
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_program);
	upload_shader_params();

	auto camera_pos = m_camera.get_position();
	m_camera.draw();

	glUniform3fv(m_light_pos_loc, 1, glm::value_ptr(glm::vec3(10, 10, 10)));
	glUniform3fv(m_view_pos_loc, 1, glm::value_ptr(camera_pos));
	m_arm.draw(time);

	// glUseProgram(m_floor_program);
}
void Scene::update_aspect_ratio(int width, int height)
{
	m_camera.update_aspect_ratio(width, height);
}
void Scene::update_last_mouse_pos(glm::vec2 last_mouse_pos)
{
	m_last_mouse_pos = last_mouse_pos;
}
void Scene::drag_camera(glm::vec2 new_mouse_pos)
{
	auto delta		  = new_mouse_pos - m_last_mouse_pos;
	update_last_mouse_pos(new_mouse_pos);
	m_camera.drag_camera(delta);
}
void Scene::change_camera_distance(float delta)
{
	m_camera.change_camera_distance(delta);
}
ShaderParams& Scene::get_shader_params()
{
	return m_params;
}

RobotArm& Scene::get_arm()
{
	return m_arm;
}
Scene::~Scene()
{
	if (m_program)
	{
		glDeleteProgram(m_program);
	}
}
