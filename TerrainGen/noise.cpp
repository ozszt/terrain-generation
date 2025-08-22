#include "noise.h"

//*
// Return a float in range [0:1] for input.
// */
float noise(int x, int y) {
	glm::vec2 vec(x,y);
	glm::vec2 i = floor(vec);	// obtain integer coords
	glm::vec2 f = fract(vec);	// obtain fractional coords

	// four corners of a tile in 2D grid
	float a = rand(i);
	float b = rand(i + glm::vec2(1.0, 0.0));
	float c = rand(i + glm::vec2(0.0, 1.0));
	float d = rand(i + glm::vec2(1.0, 1.0));

	// cubic polynomial smoothstep
	glm::vec2 u = f*f*(glm::vec2(3.0f)-2.0f * f);

	// interpolation
	return glm::mix(glm::mix(a, b, u.x), glm::mix(c, d, u.x), u.y);
}

//*
// Pseudo-random function
// source: https://thebookofshaders.com/10/
// */
float rand(glm::vec2 v) {
	return fract(sin(dot(v, glm::vec2(12.9898, 78.233))) * 43758.5453123);
}

//* Helper function to obtain the fraction of x */
float fract(float x) {
	return x - floor(x);
}