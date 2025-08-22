#ifndef view_h
#define view_h

#include "model.h"
#include "mesh.h"
#include "glinit.h"
#include "camera.h"
#include "lighting.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <string>
#include <iostream>
	
GLuint LoadShader(std::string shader_file_name, GLenum shader_type);

class View {
private:
	Model& model;
	Camera& camera;
	Mesh& mesh;
	GLuint VAO;
	GLuint VBO;
	GLuint lightVAO;
	GLuint color_buffer;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint element_buffer;
	GLuint mvp_uniform_attribute;
public:
	GLuint shader_program;
	View(Model& model,Camera& camera, Mesh& mesh);
	void render(GLFWwindow* window, Lighting& light);
};


#endif // !view_h
