#include "view.h"

View::View(Model& model, Camera& camera, Mesh& mesh):model(model),camera(camera),mesh(mesh){
    GLuint vertex_shader = LoadShader(std::string("vertex_shader.txt"), GL_VERTEX_SHADER);
    GLuint fragment_shader = LoadShader(std::string("fragment_shader.txt"), GL_FRAGMENT_SHADER);

    // link shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    
    glBindFragDataLocation(shader_program, 0, "color_out");
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    // check link status
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // detach and delete individual shaders to free up resources
    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // start using program
    glUseProgram(shader_program);
    mvp_uniform_attribute = glGetUniformLocation(shader_program, "mvp");


}

void View::render(GLFWwindow* window,Lighting& light) {
    using glm::vec3;
    using glm::mat4;
    using glm::mat3;

    glEnable(GL_DEPTH_TEST);
    glClearColor(model.background_color[0], model.background_color[1],
        model.background_color[2], model.background_color[3]); // specify color to clear screen with
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera.updateViewMatrix();
    
    // create model matrix
    vec3 scaling(1.0f, 1.0f, 1.0f);
    vec3 translation(-0.5f, 0, 0.1f);
    vec3 rotation_axis(0, 1.0f, 0);
    float rotation_angle = 0;
    mat4 model_matrix = glm::translate(glm::rotate(glm::scale(
        mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);
    
    mat4 model_view_projection = camera.proj_matrix*camera.view_matrix*model_matrix;

    glUniformMatrix4fv(mvp_uniform_attribute, 1, GL_FALSE, &model_view_projection[0][0]);

    translation = glm::vec3(+0.5f, 0, 0);
    // put models in their right place in the scene
    model_matrix = glm::translate(glm::rotate(glm::scale(
        glm::mat4(1.0f), scaling), rotation_angle, rotation_axis), translation);

    model_view_projection = camera.proj_matrix * camera.view_matrix * model_matrix;;

    // calculate normal
    mat3 normal_matrix = glm::transpose(glm::inverse(mat3(model_matrix)));

    // send values to shader
    glUseProgram(shader_program);
    glUniformMatrix4fv(mvp_uniform_attribute, 1, GL_FALSE, &model_view_projection[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader_program, "model"), 1, GL_FALSE, &model_matrix[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader_program, "normal_matrix"), 1, GL_FALSE, &normal_matrix[0][0]);
    glUniform3f(glGetUniformLocation(shader_program, "light_col"), 1.0f, 1.0f, 1.0f); // white ambient
    glUniform3f(glGetUniformLocation(shader_program, "light_pos"), light.pos[0], light.pos[1], light.pos[2]);
    glUniform3f(glGetUniformLocation(shader_program, "view_pos"), camera.pos[0], camera.pos[1], camera.pos[2]);
    
    // draw objects
    mesh.Draw();

    // draw light source
    light.drawLight(camera.view_matrix, camera.proj_matrix);

    glFlush();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
    glfwPollEvents();
}

//* 
// Load and compile shader. 
// */
GLuint LoadShader(std::string shader_file_name, GLenum shader_type) {
    // open and create input stream file 
    std::ifstream my_stream(shader_file_name);
    // read entire file into a single string 
    std::string contents = std::string((std::istreambuf_iterator<char>(my_stream)), std::istreambuf_iterator<char>());
    // convert string (contents) into raw C string to prepare for OpenGL
    const GLchar* shader_code = contents.c_str();

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);

    // check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}
