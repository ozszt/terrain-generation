#include "modelgeometry.h"

// TODO: add logic to test if corners make proper block/rectangle, #vertices is correct, etc...
Rectangle::Rectangle(std::vector<glm::vec3> rectangle_corners, glm::vec3 RGB_col) :
    rectangle_corners(rectangle_corners), RGB_col(RGB_col) {
}

// overload to simplify access to corners
const glm::vec3& Rectangle::operator[](const size_t& index) const {
    return rectangle_corners[index];
}

Block::Block(std::vector<Rectangle> block_sides) :block_sides(block_sides) {}
