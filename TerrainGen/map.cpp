#include "map.h"
#include "PerlinNoise.hpp"
#include "noise.h"

// settings for height map
const unsigned int map_size = 100;
const float base_frequency = 0.3f; 
const float base_amplitude = 5.0f; 
const int num_octaves = 6;

float map[map_size][map_size];

//*
// Fill the map grid with a value
// */
void Map::fillMap() {
	for (int y = 0; y < map_size; y++)
	{
		for (int x = 0; x < map_size; x++) {
			double nx = double(x) / map_size - 0.5; // center at (0,0)
			double ny = double(y) / map_size - 0.5;
			
			float total = 0.0f;
			float freq = base_frequency;
			float amp = base_amplitude;

			for (int octave = 0; octave < num_octaves; octave++)
			{
				// sum multiple layers of noise
				total += noise(x * freq, y * freq) * amp;
				freq *= 1.0f;
				amp *= 0.5f;
			}

			map[y][x] = total;
		}
	}
}

//*
//	TODO: Fix height-based coloring
// */
glm::vec3 Map::getTerrainColor(float y) {
	if (y < 1.0) 
		return glm::vec3(0.68, 0.86, 0.67); // grass
	else if (y < 3.0) 
		return glm::vec3(0.74, 0.67, 0.52); // dirt
	else return glm::vec3(0.96, 1, 1); // snow
}

//*
//  Vertex generation based on height map
// */
void Map::generateVertices(std::vector<Vertex>& vertices) {
	vertices.clear();
	for (int y = 0; y < map_size; y++)
	{
		for (int x = 0; x < map_size; x++) {
			Vertex v;

			// position
			v.pos = glm::vec3(
				float(x) - map_size / 2.0f, 
				float(map[y][x]),	
				float(y) - map_size / 2.0f
			);

			// normal
			v.normal = glm::vec3(0.0f);


			// TODO: FIX TEXTURES?
			// texture
			v.tex_coords = glm::vec2();

			// color
			v.color = getTerrainColor(map[y][x]);

			vertices.push_back(v);
		}
	}
}

//*
// Generate triangle indices for mesh
// */
void Map::generateIndices(std::vector<unsigned int>& indices) {
	indices.clear();
	for (int y = 0; y < map_size - 1; y++) {
		for (int x = 0; x < map_size - 1; x++) {
			int topLeft = y * map_size + x;
			int topRight = topLeft + 1;
			int bottomLeft = (y + 1) * map_size + x;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);

			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
}

//*
// Calculate normals for each vertex 
// */
void Map::generateNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	for (auto& v : vertices) {
		v.normal = glm::vec3(0.0f);
	}

	for (size_t i = 0; i < indices.size(); i += 3) {
		unsigned int i0 = indices[i];
		unsigned int i1 = indices[i + 1];
		unsigned int i2 = indices[i + 2];

		glm::vec3 v0 = vertices[i0].pos;
		glm::vec3 v1 = vertices[i1].pos;
		glm::vec3 v2 = vertices[i2].pos;

		// compute face normal
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		// add to each vertex normal
		vertices[i0].normal += normal;
		vertices[i1].normal += normal;
		vertices[i2].normal += normal;
	}

	for (auto& v : vertices) {
		v.normal = glm::normalize(v.normal);
	}
}
