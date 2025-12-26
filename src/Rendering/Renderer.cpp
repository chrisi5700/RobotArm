//
// Created by chris on 12/21/25.
//
#include <RobotArm/Rendering/Renderer.hpp>

ShaderProgram load_shader()
{
	std::filesystem::path shader_dir = SHADER_PATH;
	std::vector<std::string> vars{
		"view",
		"projection",

		"viewPos",
		"lightPos",
		"lightColor",
		"lightIntensity",
		"shadowThreshold",    // Where shadow starts (0.0-0.5)
		"shadowSoftness",     // Transition width
		"shadowTint",          // Multiplied with base color in shadow
		"shadowStrength",     // How dark shadows are (0-1)
		"enableSpecular",
		"specularThreshold",
		"specularSize",       // Smaller = tighter highlight
		"enableRimLight",
		"rimColor",
		"rimThreshold",
		"rimSoftness",
		"enableOutline",
		"outlineThreshold",
		"outlineStrength",
		"enableHeightFog",
		"fogColor",
		"fogDensity",
		"fogHeightFalloff",
		"enableGammaCorrection",
		"gamma",
	};
	return ShaderProgram::create_graphics_shader(shader_dir/"vert.glsl", shader_dir / "frag.glsl", vars);
}

Renderer::Renderer() : m_shader(load_shader()), m_meshes()
{
	m_shader.bind();
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable face culling
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);*/

	// Set clear color
	glClearColor(60 / 255.f,56 / 255.f,54 / 255.f, 1.0f);
	push_shader_params({});
	auto sphere = generate_sphere(0.33f, 20, 20);
	m_meshes.load(MeshId::Sphere, sphere.vertices, sphere.indices);
	auto cube = generate_cube();
	m_meshes.load(MeshId::Cube, cube.vertices, cube.indices);
	auto cylinder = generate_cylinder(1.0f, 1.0f, 20);
	m_meshes.load(MeshId::Cylinder, cylinder.vertices, cylinder.indices);
	auto arrow = generate_arrow(1, 3, 0.2, 16);
	m_meshes.load(MeshId::Arrow, arrow.vertices, arrow.indices);
}
void Renderer::render(RenderQueue& queue, const Camera& camera)
{
	m_shader.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader.set_uniform("view", camera.get_view());
	m_shader.set_uniform("projection", camera.get_projection());
	m_shader.set_uniform("viewPos", camera.get_position());
	m_shader.set_uniform("lightPos", glm::vec3(10, 10, 10));

	// Group by mesh for instanced drawing
	auto batches = queue.get_meshes_batched();

	for (const auto& [mesh_id, matrices] : batches)
	{
		auto& mesh = m_meshes.get(mesh_id);
		mesh.upload_instances(matrices);
		mesh.draw(matrices.size());
	}
	queue.clear();
}
MeshRegistry& Renderer::mesh_registry()
{
	return m_meshes;
}
void Renderer::push_shader_params(const ShaderParams& params)
{
	// Light
	m_shader.set_uniform("lightColor", params.lightColor);
	m_shader.set_uniform("lightIntensity", params.lightIntensity);

	// Shadows
	m_shader.set_uniform("shadowThreshold", params.shadowThreshold);
	m_shader.set_uniform("shadowSoftness", params.shadowSoftness);
	m_shader.set_uniform("shadowTint", params.shadowTint);
	m_shader.set_uniform("shadowStrength", params.shadowStrength);



    // Specular
    m_shader.set_uniform("enableSpecular", params.enableSpecular);
    m_shader.set_uniform("specularThreshold", params.specularThreshold);
    m_shader.set_uniform("specularSize", params.specularSize);

    // Rim
    m_shader.set_uniform("enableRimLight", params.enableRimLight);
    m_shader.set_uniform("rimColor", params.rimColor);
    m_shader.set_uniform("rimThreshold", params.rimThreshold);
    m_shader.set_uniform("rimSoftness", params.rimSoftness);


    // Outline
    m_shader.set_uniform("enableOutline", params.enableOutline);
    m_shader.set_uniform("outlineThreshold", params.outlineThreshold);
    m_shader.set_uniform("outlineStrength", params.outlineStrength);

    // Fog
    m_shader.set_uniform("enableHeightFog", params.enableFog);
    m_shader.set_uniform("fogColor", params.fogColor);
    m_shader.set_uniform("fogDensity", params.fogDensity);
    m_shader.set_uniform("fogHeightFalloff", params.fogHeightFalloff);

    // Gamma
    m_shader.set_uniform("enableGammaCorrection", params.enableGamma);
    m_shader.set_uniform("gamma", params.gamma);
}