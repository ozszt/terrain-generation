#ifndef main_h	
#define main_h

#include "glinit.h"
#include "model.h"
#include "view.h"
#include "mesh.h"
#include "map.h"
#include "lighting.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool first_mouse = true;
float last_x = SCR_WIDTH / 2, last_y = SCR_HEIGHT / 2;

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void processInput(GLFWwindow* window, Model& model, Camera& camera, Lighting& light);

#endif