//
// Created by chris on 12/17/25.
//
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QtRendering/Renderer.hpp>

#include "QtRendering/InstancedCubes.hpp"

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
GLuint shader_from_file(GLenum type, const char* filename)
{
	std::ifstream shader_file{filename};
	std::istreambuf_iterator<char> it{shader_file};
	std::string shader_text{it, std::istreambuf_iterator<char>()};
	return compile_shader(type, shader_text.c_str(), shader_text.length());

}
Renderer::Renderer()
{
	glEnable(GL_DEPTH_TEST);
	auto vert_shader = shader_from_file(GL_VERTEX_SHADER, VERT_SHADER_PATH);
	auto frag_shader = shader_from_file(GL_FRAGMENT_SHADER, FRAG_SHADER_PATH);
	m_program = glCreateProgram();
	glAttachShader(m_program, vert_shader);
	glAttachShader(m_program, frag_shader);
	glLinkProgram(m_program);
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	m_view_loc = glGetUniformLocation(m_program, "view");
	m_proj_loc = glGetUniformLocation(m_program, "projection");
	m_light_pos_loc = glGetUniformLocation(m_program, "lightPos");
	m_view_pos_loc = glGetUniformLocation(m_program, "viewPos");

	m_drawables.push_back(std::make_unique<InstancedCubes>());

}
void Renderer::draw(float time)
{
	glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);

	// Camera
	glm::vec3 cameraPos = glm::vec3(sin(time * 0.2f) * 20.0f, 15.0f, cos(time * 0.2f) * 20.0f);
	glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);

	glUniformMatrix4fv(m_view_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(m_proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(m_light_pos_loc, 1, glm::value_ptr(glm::vec3(10, 10, 10)));
	glUniform3fv(m_view_pos_loc, 1, glm::value_ptr(cameraPos));
	for (auto& obj : m_drawables)
	{
		obj->draw(time);
	}
}
void Renderer::update_aspect_ratio(int width, int height)
{
	m_aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
}
Renderer::~Renderer()
{
	if (m_program) {
		glDeleteProgram(m_program);
	}
}

