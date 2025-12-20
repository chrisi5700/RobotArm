//
// Created by chris on 12/18/25.
//
#include <QtRendering/GLCommon.hpp>

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

			mesh.indices.push_back(current);
			mesh.indices.push_back(next);
			mesh.indices.push_back(current + 1);

			mesh.indices.push_back(current + 1);
			mesh.indices.push_back(next);
			mesh.indices.push_back(next + 1);
		}
	}

	return mesh;
}