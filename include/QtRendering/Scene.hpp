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
	// Shadow/lit split
	float shadowThreshold = 0.1f;     // Where shadow begins
	float shadowSoftness = 0.05f;     // Transition width
	glm::vec3 shadowTint = glm::vec3(0.6f, 0.6f, 0.8f);  // Cool tint in shadows
	float shadowStrength = 0.4f;      // How dark (0 = no shadow, 1 = black)

	// Specular
	bool enableSpecular = true;
	float specularThreshold = 0.9f;   // High = small highlight
	float specularSize = 32.0f;       // Power exponent

	// Rim
	bool enableRimLight = true;
	glm::vec3 rimColor = glm::vec3(1.0f, 0.95f, 0.9f);
	float rimThreshold = 0.65f;
	float rimSoftness = 0.1f;

	// Outline
	bool enableOutline = true;
	float outlineThreshold = 0.25f;
	float outlineStrength = 0.6f;

	// Fog
	bool enableFog = false;
	glm::vec3 fogColor = glm::vec3(0.5f, 0.6f, 0.7f);
	float fogDensity = 0.02f;
	float fogHeightFalloff = 0.5f;

	// Light
	glm::vec3 lightColor = glm::vec3(1.0f);
	float lightIntensity = 1.0f;

	// Background
	glm::vec3 topColor = glm::vec3(0.24f, 0.22f, 0.21f);
	glm::vec3 bottomColor = glm::vec3(0.0f);

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
	// GLint m_floor_program;
	GLint m_light_pos_loc;
	GLint m_view_pos_loc;
	GLuint m_empty_vao;
	glm::vec2 m_last_mouse_pos{0,0};
	Camera m_camera;
	RobotArm m_arm;
	ShaderParams m_params;
};


#endif // OPENGL_TEST_RENDERER_HPP
