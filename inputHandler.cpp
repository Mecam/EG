#include "inputHandler.h"

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	egInputHandler->sendEvent(&egKeyPressEvent(key, action, mods));
}
void mousekey_callback(GLFWwindow* window, int button, int action, int mods)
{
	//sendEvent(&egGameLoop, egKeyPressEvent())
}
void mousepos_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	egInputHandler->mousePos[2] = egInputHandler->mousePos[0] - xpos;
	egInputHandler->mousePos[3] = egInputHandler->mousePos[1] - ypos;
	egInputHandler->mousePos[0] = xpos;
	egInputHandler->mousePos[1] = ypos;
	if (egInputHandler->mousePos[2], egInputHandler->mousePos[3])
	{
		egWindowHandler->reCenterMouse();
		egInputHandler->sendEvent(&egMouseMoveEvent(egInputHandler->mousePos[2], egInputHandler->mousePos[3]));
	}
	 
}
void scrollOff_callback(GLFWwindow* window, double xoff, double yoff)
{
	egInputHandler->scrollOff[0] = xoff;
	egInputHandler->scrollOff[1] = yoff;
}
 
void _egInputHandler::init(GLFWwindow* window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetMouseButtonCallback(window, mousekey_callback);
	glfwSetScrollCallback(window, scrollOff_callback);
	glfwSetCursorPosCallback(window, mousepos_callback);
}

float* _egInputHandler::getMousePos()
{
	return this->mousePos;
}
double* _egInputHandler::getScrollOffset()
{
	return this->scrollOff;
}

 
 