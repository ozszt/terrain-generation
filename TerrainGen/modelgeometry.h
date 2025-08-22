#ifndef modelgeometry_h
#define modelgeometry_h

#include <vector>
#include "glm/glm.hpp"

//*
// Consists of four points in 3D space and a color
// */
class Rectangle {
public:
	glm::vec3 RGB_col;
	std::vector<glm::vec3> rectangle_corners;
	Rectangle(std::vector<glm::vec3> rectangle_corners, glm::vec3 RGB_col);
	const glm::vec3& operator[](const size_t& index) const; // to easily access the vertices as rectangle[index]
};


//*
// Consists of six rectangles
// */
class Block {
public:
	std::vector<Rectangle> block_sides;
	Block(std::vector<Rectangle> block_sides);
};

#endif // !modelgeometry_h
