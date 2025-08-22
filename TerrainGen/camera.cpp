#include "camera.h"


Camera::Camera() {}

void Camera::updateViewMatrix() {
	using glm::vec3;

	// rotation
	dir = vec3(-sin(yaw) * cos(pitch), sin(pitch), -cos(pitch) * cos(yaw));
	right = vec3(cos(yaw), 0, -sin(yaw));
	up = glm::cross(right, dir);

	view_matrix = glm::lookAt(pos, pos + dir, up);
}