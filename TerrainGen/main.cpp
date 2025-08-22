#include "main.h"
#include <iostream>

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    init(window);

    // shaders
    GLuint vertex_shader = LoadShader(std::string("vertex_shader.txt"), GL_VERTEX_SHADER);
    GLuint fragment_shader = LoadShader(std::string("fragment_shader.txt"), GL_FRAGMENT_SHADER);
    GLuint light_vs = LoadShader(std::string("light_vertex_shader.txt"), GL_VERTEX_SHADER);
    GLuint light_fs = LoadShader(std::string("light_frag_shader.txt"), GL_FRAGMENT_SHADER);

    // instances
    Map map;
    map.fillMap();
    
    std::vector<Vertex> vertices = {};
    std::vector<unsigned int> indices = {};
    std::vector<Texture> textures;
    glm::vec3 light_pos(10.0f, 10.0f, 10.0f);

    map.generateVertices(vertices);
    map.generateIndices(indices);
    map.generateNormals(vertices, indices);

    Model model;
    Camera camera;
    Mesh mesh(vertices, indices, textures);
    View view(model,camera, mesh);
    Lighting light_source(light_pos, light_vs, light_fs);
    
    // capture mouse
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // render loop
    // TODO: add timestep ?
    while (!glfwWindowShouldClose(window))
    {
        // handle input
        processInput(window, model,camera,light_source);

        // rendering
        view.render(window,light_source);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window, Model& model, Camera& camera, Lighting& light)
{
    // close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // change colors based on pressed key
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        model.background_color = std::vector<float>{ 1,0,0,1 };
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        model.background_color = std::vector<float>{ 0,1,0,1 };
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        model.background_color = std::vector<float>{ 0,0,1,1 };
    
    // handle camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += camera.speed * camera.dir;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= camera.speed * camera.right;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= camera.speed * camera.dir;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += camera.speed * camera.right;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // up
        camera.pos[1] += camera.speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // down
        camera.pos[1] -= camera.speed;
   
    // move light source
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        light.pos[1] += camera.speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        light.pos -= camera.speed * camera.right;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        light.pos[1] -= camera.speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        light.pos += camera.speed * camera.right;
}

// called whenever the mouse is moved to compute camera's direction vector
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    // calculate mouse offset since last frame
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;
    last_x = x_pos;
    last_y = y_pos;

    const float sens = 0.1f;
    x_offset *= sens;
    y_offset *= sens;

    // add offset values to yaw and pitch
    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window)); // access camera instance
    camera->yaw -= glm::radians(x_offset);
    camera->pitch += glm::radians(y_offset);

    // add constraints to min/max pitch values
    // no more/less than 89/-89 degrees as that will cause a LookAt flip 
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    // calculate direction vector
    glm::vec3 dir;
    dir.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    dir.y = sin(glm::radians(camera->pitch));
    dir.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->dir = glm::normalize(dir);

    camera->updateViewMatrix();
}




