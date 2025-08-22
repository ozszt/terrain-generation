#ifndef map_h
#define map_h

#include "mesh.h"
#include "glm/glm.hpp"

class Map {
public:
	void fillMap();
	glm::vec3 getTerrainColor(float y);
	void generateVertices(std::vector<Vertex>& vertices);
	void generateIndices(std::vector<unsigned int>& indices);
	void generateNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
};

#endif // !map_h
