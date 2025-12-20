//
// Created by chris on 12/17/25.
//

#ifndef OPENGL_TEST_RENDERER_HPP
#define OPENGL_TEST_RENDERER_HPP
#include <filesystem>
#include "GLCommon.hpp"

#include "Camera.hpp"
#include "RobotArm.hpp"

struct ShaderParams {
	// Lighting
	float ambientStrength = 0.15f;
	float diffuseStrength = 1.0f;
	float specularStrength = 0.5f;
	float shininess = 32.0f;
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float lightIntensity = 1.0f;

	// Fresnel
	bool enableFresnel = true;
	float fresnelBias = 0.1f;
	float fresnelScale = 1.0f;
	float fresnelPower = 2.0f;

	// Rim
	bool enableRimLight = true;
	glm::vec3 rimColor = glm::vec3(0.3f, 0.4f, 0.5f);
	float rimPower = 3.0f;
	float rimStrength = 0.5f;

	// Fog
	bool enableFog = false;
	glm::vec3 fogColor = glm::vec3(0.5f, 0.6f, 0.7f);
	float fogDensity = 0.02f;
	float fogHeightFalloff = 0.5f;

	// Background
	glm::vec3 topColor = glm::vec3(0.4f, 0.5f, 0.7f);
	glm::vec3 bottomColor = glm::vec3(0.1f, 0.1f, 0.15f);

	// Gamma
	bool enableGamma = true;
	float gamma = 2.2f;
};

class Scene
{

public:
	Scene();
	void upload_shader_params();
	void draw(float time);
	void update_aspect_ratio(int width, int height);
	void update_last_mouse_pos(glm::vec2 last_mouse_pos);
	void drag_camera(glm::vec2 new_mouse_pos);
	void change_camera_distance(float delta);
	ShaderParams& get_shader_params();
	RobotArm& get_arm();
	~Scene();
private:
	static inline std::filesystem::path SHADER_DIR{SHADER_PATH};
	GLint m_program;
	GLint m_background_program;
	GLint m_floor_program;
	GLint m_light_pos_loc;
	GLint m_view_pos_loc;
	GLuint m_empty_vao;
	glm::vec2 m_last_mouse_pos{0,0};
	Camera m_camera;
	RobotArm m_arm;
	ShaderParams m_params;
};


#endif // OPENGL_TEST_RENDERER_HPP
