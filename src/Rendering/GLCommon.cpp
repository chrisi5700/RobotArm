//
// Created by chris on 12/18/25.
//
#include <RobotArm/Rendering/GLCommon.hpp>

MeshData generate_cube()
{
	// 8 unique corners, 6 faces × 2 triangles × 3 indices = 36 indices
	// But we need 24 vertices (4 per face) because normals differ per face

	MeshData mesh;

	// Per-face vertices (each face has its own 4 vertices with correct normal)
	const glm::vec3 normals[6] = {
		{0, 0, 1},	// Front
		{0, 0, -1}, // Back
		{1, 0, 0},	// Right
		{-1, 0, 0}, // Left
		{0, 1, 0},	// Top
		{0, -1, 0}, // Bottom
	};

	// Face vertex positions (CCW winding when looking at face)
	const glm::vec3 faceVerts[6][4] = {
		// Front (+Z)
		{{-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}},
		// Back (-Z)
		{{0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}},
		// Right (+X)
		{{0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}},
		// Left (-X)
		{{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}},
		// Top (+Y)
		{{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}},
		// Bottom (-Y)
		{{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}},
	};

	for (int face = 0; face < 6; face++)
	{
		uint32_t baseIndex = mesh.vertices.size();

		// 4 vertices per face
		for (int v = 0; v < 4; v++)
		{
			mesh.vertices.push_back({faceVerts[face][v], normals[face]});
		}

		// 2 triangles per face (CCW)
		mesh.indices.push_back(baseIndex + 0);
		mesh.indices.push_back(baseIndex + 1);
		mesh.indices.push_back(baseIndex + 2);

		mesh.indices.push_back(baseIndex + 0);
		mesh.indices.push_back(baseIndex + 2);
		mesh.indices.push_back(baseIndex + 3);
	}

	return mesh;
}
MeshData generate_sphere(float radius, uint32_t latSegments, uint32_t lonSegments)
{
	MeshData mesh;

	for (uint32_t lat = 0; lat <= latSegments; ++lat)
	{
		float theta	   = lat * M_PI / latSegments;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (uint32_t lon = 0; lon <= lonSegments; ++lon)
		{
			float phi	 = lon * 2.0f * M_PI / lonSegments;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			glm::vec3 normal(sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);

			mesh.vertices.push_back({normal * radius, normal});
		}
	}

	for (uint32_t lat = 0; lat < latSegments; ++lat)
	{
		for (uint32_t lon = 0; lon < lonSegments; ++lon)
		{
			uint32_t current = lat * (lonSegments + 1) + lon;
			uint32_t next	 = current + lonSegments + 1;

			// First triangle: CCW winding
			mesh.indices.push_back(current);
			mesh.indices.push_back(current + 1);
			mesh.indices.push_back(next);

			// Second triangle: CCW winding
			mesh.indices.push_back(current + 1);
			mesh.indices.push_back(next + 1);
			mesh.indices.push_back(next);
		}
	}

	return mesh;
}

MeshData generate_cylinder(float radius, float height, uint32_t segments, bool caps) {
    MeshData data;

    const float half_height = height / 2.0f;

    // Side vertices: two rings
    for (uint32_t i = 0; i <= segments; ++i) {
        float angle = (static_cast<float>(i) / segments) * 2.0f * glm::pi<float>();
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));

        data.vertices.push_back({{x, -half_height, z}, normal}); // Bottom ring
        data.vertices.push_back({{x, half_height, z}, normal});  // Top ring
    }

    // Side indices
    for (uint32_t i = 0; i < segments; ++i) {
        uint32_t bl = i * 2;
        uint32_t tl = i * 2 + 1;
        uint32_t br = (i + 1) * 2;
        uint32_t tr = (i + 1) * 2 + 1;

        data.indices.insert(data.indices.end(), {bl, br, tr, bl, tr, tl});
    }

    if (caps) {
        // Bottom cap
        uint32_t bottom_center = static_cast<uint32_t>(data.vertices.size());
        data.vertices.push_back({{0.0f, -half_height, 0.0f}, {0.0f, -1.0f, 0.0f}});

        uint32_t bottom_ring = static_cast<uint32_t>(data.vertices.size());
        for (uint32_t i = 0; i <= segments; ++i) {
            float angle = (static_cast<float>(i) / segments) * 2.0f * glm::pi<float>();
            data.vertices.push_back({
                {radius * std::cos(angle), -half_height, radius * std::sin(angle)},
                {0.0f, -1.0f, 0.0f}
            });
        }

        for (uint32_t i = 0; i < segments; ++i) {
            data.indices.insert(data.indices.end(), {
                bottom_center, bottom_ring + i + 1, bottom_ring + i
            });
        }

        // Top cap
        uint32_t top_center = static_cast<uint32_t>(data.vertices.size());
        data.vertices.push_back({{0.0f, half_height, 0.0f}, {0.0f, 1.0f, 0.0f}});

        uint32_t top_ring = static_cast<uint32_t>(data.vertices.size());
        for (uint32_t i = 0; i <= segments; ++i) {
            float angle = (static_cast<float>(i) / segments) * 2.0f * glm::pi<float>();
            data.vertices.push_back({
                {radius * std::cos(angle), half_height, radius * std::sin(angle)},
                {0.0f, 1.0f, 0.0f}
            });
        }

        for (uint32_t i = 0; i < segments; ++i) {
            data.indices.insert(data.indices.end(), {
                top_center, top_ring + i, top_ring + i + 1
            });
        }
    }

    return data;
}
MeshData generate_arrow(float shaft_radius, float head_radius, float head_fraction, uint32_t segments)
{
	// head_fraction is how much of the total length=1 is the cone head
	float shaft_length = 1.0f - head_fraction;
	float head_length  = head_fraction;

	MeshData mesh;

	// Shaft: cylinder from y=0 to y=shaft_length
	for (uint32_t i = 0; i <= segments; ++i)
	{
		float theta = 2.0f * 3.14159265f * static_cast<float>(i) / static_cast<float>(segments);
		float x		= std::cos(theta);
		float z		= std::sin(theta);

		glm::vec3 normal{x, 0.0f, z};

		// Bottom vertex
		mesh.vertices.push_back({{x * shaft_radius, 0.0f, z * shaft_radius}, normal});
		// Top vertex
		mesh.vertices.push_back({{x * shaft_radius, shaft_length, z * shaft_radius}, normal});
	}

	// Shaft indices
	for (uint32_t i = 0; i < segments; ++i)
	{
		uint32_t base = i * 2;
		mesh.indices.push_back(base);
		mesh.indices.push_back(base + 1);
		mesh.indices.push_back(base + 2);

		mesh.indices.push_back(base + 1);
		mesh.indices.push_back(base + 3);
		mesh.indices.push_back(base + 2);
	}


	// Cone slope for normal calculation
	float slope = head_radius / head_length;
	float ny	= slope / std::sqrt(1.0f + slope * slope);
	float nxz	= 1.0f / std::sqrt(1.0f + slope * slope);

	// Cone vertices (each triangle gets its own to avoid normal issues)
	for (uint32_t i = 0; i < segments; ++i)
	{
		float theta0 = 2.0f * 3.14159265f * static_cast<float>(i) / static_cast<float>(segments);
		float theta1 = 2.0f * 3.14159265f * static_cast<float>(i + 1) / static_cast<float>(segments);

		float x0 = std::cos(theta0), z0 = std::sin(theta0);
		float x1 = std::cos(theta1), z1 = std::sin(theta1);

		glm::vec3 n0{x0 * nxz, ny, z0 * nxz};
		glm::vec3 n1{x1 * nxz, ny, z1 * nxz};
		glm::vec3 n_avg = glm::normalize(n0 + n1);

		uint32_t base = mesh.vertices.size();

		// Base edge vertices
		mesh.vertices.push_back({{x0 * head_radius, shaft_length, z0 * head_radius}, n0});
		mesh.vertices.push_back({{x1 * head_radius, shaft_length, z1 * head_radius}, n1});
		// Tip
		mesh.vertices.push_back({{0.0f, 1.0f, 0.0f}, n_avg});

		mesh.indices.push_back(base);
		mesh.indices.push_back(base + 2);
		mesh.indices.push_back(base + 1);
	}

	// Cone base cap (flat, pointing down)
	uint32_t cap_center = mesh.vertices.size();
	mesh.vertices.push_back({{0.0f, shaft_length, 0.0f}, {0.0f, -1.0f, 0.0f}});

	for (uint32_t i = 0; i <= segments; ++i)
	{
		float theta = 2.0f * 3.14159265f * static_cast<float>(i) / static_cast<float>(segments);
		mesh.vertices.push_back(
			{{std::cos(theta) * head_radius, shaft_length, std::sin(theta) * head_radius}, {0.0f, -1.0f, 0.0f}});
	}

	for (uint32_t i = 0; i < segments; ++i)
	{
		mesh.indices.push_back(cap_center);
		mesh.indices.push_back(cap_center + 1 + i + 1);
		mesh.indices.push_back(cap_center + 1 + i);
	}

	return mesh;
}