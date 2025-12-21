//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_SHADER_HPP
#define ROBOTARM_SHADER_HPP
#include <filesystem>
#include <flat_map>
#include <glm/gtc/type_ptr.hpp>
#include <variant>

#include "GLCommon.hpp"

class ShaderProgram
{
	std::flat_map<std::string, GLint> m_locations;
	ShaderProgram(std::flat_map<std::string, GLint> m_params, GLint m_program);
	GLint m_program;
public:
	static ShaderProgram create_graphics_shader(std::filesystem::path vert_path, std::filesystem::path frag_path,
		std::span<std::string> params); // <- those could be loaded directly from source but Im not writing a parser right now
	~ShaderProgram();
	void set_uniform(const std::string& name, float v) {
		glUniform1f(m_locations[name], v);
	}
	void set_uniform(const std::string& name, int v) {
		glUniform1i(m_locations[name], v);
	}

	// Vectors
	void set_uniform(const std::string& name, const glm::vec2& v) {
		glUniform2fv(m_locations[name], 1, glm::value_ptr(v));
	}
	void set_uniform(const std::string& name, const glm::vec3& v) {
		glUniform3fv(m_locations[name], 1, glm::value_ptr(v));
	}
	void set_uniform(const std::string& name, const glm::vec4& v) {
		glUniform4fv(m_locations[name], 1, glm::value_ptr(v));
	}

	// Matrices
	void set_uniform(const std::string& name, const glm::mat3& m) {
		glUniformMatrix3fv(m_locations[name], 1, GL_FALSE, glm::value_ptr(m));
	}
	void set_uniform(const std::string& name, const glm::mat4& m) {
		glUniformMatrix4fv(m_locations[name], 1, GL_FALSE, glm::value_ptr(m));
	}
	void bind() const;
};

#endif // ROBOTARM_SHADER_HPP
