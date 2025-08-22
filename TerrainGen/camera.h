#ifndef camera_h
#define camera_h
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
	float fov = 1.57f;
	float aspect_ratio = 4.0f / 3.0f;
	float near_plane = 0.1f;
	float far_plane = 100.0f;
public:
	glm::mat4 view_matrix;
	glm::mat4 proj_matrix = glm::perspective(fov, aspect_ratio, near_plane, far_plane);
	glm::vec3 pos = glm::vec3(10, 10, 1);
	glm::vec3 dir = glm::vec3(0, 0, -1);
	glm::vec3 right = glm::vec3(1, 0, 0); // used for strafing
	glm::vec3 up = glm::vec3(0, 1, 0);

	float yaw = 0.0f;
	float pitch = 0.0f;
	float speed = 0.01f;
	Camera();
	void updateViewMatrix();
};

#endif // !camera_h
