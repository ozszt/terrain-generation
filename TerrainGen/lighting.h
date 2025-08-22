#ifndef lighting_h
#define lighting_h

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <vector>
#include "map.h"
#include <fstream>
#include <string>
#include <iostream>

class Lighting {
private:
	unsigned int lightVAO, lightVBO;
	GLuint setupLight() {
		float light_vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		};
		glGenVertexArrays(1, &lightVAO);
		glGenBuffers(1, &lightVBO);
		glBindVertexArray(lightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// link shaders
		GLuint shader_program = glCreateProgram();
		glAttachShader(shader_program, light_vs);
		glAttachShader(shader_program, light_fs);
		glLinkProgram(shader_program);

		// check link status
		int success;
		char infoLog[512];
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			std::cout << "ERROR::LIGHT_SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// detachto free up resources
		glDetachShader(shader_program, light_vs);
		glDetachShader(shader_program, light_fs);
		glDeleteShader(light_vs);
		glDeleteShader(light_fs);

		// start using program
		glUseProgram(shader_program);

		return shader_program;
	}

public:
	glm::vec3 pos;
	GLuint light_vs, light_fs;
	GLuint light_shader;
	GLuint mvp_uniform_attribute;

	// constructor
	Lighting(glm::vec3 pos, GLuint light_vs, GLuint light_fs) {
		this->pos = pos;
		this->light_vs = light_vs;
		this->light_fs = light_fs;
		this->light_shader = setupLight();
		this->mvp_uniform_attribute = glGetUniformLocation(light_shader, "mvp");
	}

	void drawLight(glm::mat4& view, glm::mat4& proj) {
		glUseProgram(light_shader);

		// model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);

		glm::mat4 model_view_projection = proj * view * model;

		glUniformMatrix4fv(glGetUniformLocation(light_shader, "mvp"), 1, GL_FALSE, glm::value_ptr(model_view_projection));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
};

#endif // !lighting_h
