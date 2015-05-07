#include "Application.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;


int main() 
{
	Application *app = new Application(vec4(0.25f, 0.25f, 0.25f, 1.0f));
	app->RunGame();
	delete app;

	return 0;
}