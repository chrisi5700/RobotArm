//
// Created by chris on 12/21/25.
//
#include <fstream>
#include <iostream>
#include <RobotArm/Rendering/ShaderProgram.hpp>

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


ShaderProgram::ShaderProgram(std::flat_map<std::string, GLint> m_locations, GLint m_program)
	: m_locations(m_locations)
	, m_program(m_program)
{
}
ShaderProgram ShaderProgram::create_graphics_shader(std::filesystem::path vert_path, std::filesystem::path frag_path,
													std::span<std::string> params)
{
	auto program = create_program(vert_path, frag_path);
	std::flat_map<std::string, GLint> m_params{};
	for (const auto& name  : params)
	{
		auto loc = glGetUniformLocation(program, name.c_str());
		m_params.insert({name, loc});
	}
	return ShaderProgram{std::move(m_params), program};

}
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_program);
}
void ShaderProgram::bind() const
{
	glUseProgram(m_program);
}