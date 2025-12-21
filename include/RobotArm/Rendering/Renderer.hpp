//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_RENDERER_HPP
#define ROBOTARM_RENDERER_HPP
#include "GLCommon.hpp"
#include "Camera.hpp"
#include "MeshRegistry.hpp"
#include "RenderQueue.hpp"
#include "ShaderProgram.hpp"


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

class Renderer {
	ShaderProgram m_shader;
	MeshRegistry m_meshes;

public:
	Renderer();
	void render(RenderQueue& queue, const Camera& camera);
	MeshRegistry& mesh_registry();
	void push_shader_params(const ShaderParams& params);
};


#endif // ROBOTARM_RENDERER_HPP
