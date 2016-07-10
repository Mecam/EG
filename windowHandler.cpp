#include "windowHandler.h"
 
void _egWindowHandler::createWindow(int type, string name, int width, int height)
{
	//Get monitor video mode
	const GLFWvidmode* scrMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//Configure framebuffer based on current video mode properties
	glfwWindowHint(GLFW_RED_BITS, scrMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, scrMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, scrMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, scrMode->refreshRate);

	//Configure OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Make window non-resizeable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	 
	switch (type)
	{
	//Create window of specified dimensions
	case EG_W_WINDOWED:
		this->primaryWindow = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		break;
	//Create window of screen dimensions, making it fullscreen 
	case EG_W_FULLSCREEN:
		this->primaryWindow = glfwCreateWindow(scrMode->width, scrMode->height, name.c_str(), glfwGetPrimaryMonitor(), NULL);
		break;
	}

	//Make mouse cursor invisible
	glfwSetInputMode(this->primaryWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//Set this window as the active OpenGL context
	glfwMakeContextCurrent(this->primaryWindow);
	//Set OpenGL render area to the entire area of the window
	glViewport(0, 0, width, height);
}

void _egWindowHandler::resize(int width, int height)
{
	//Set new window size
	glfwSetWindowSize(this->primaryWindow, width, height);
}

float _egWindowHandler::getAspectRatio()
{
	int w, h;
	//Get window dimensions
	glfwGetWindowSize(this->primaryWindow, &w, &h);
	//Return aspect ratio
	if (h == 0)
		return 1;
	else
		return w / h;
}

void _egWindowHandler::getMousePos(float *coords)
{
	int dim[2], pos[2];
	double crd[2];

	//Get window dimensions
	glfwGetWindowSize(this->primaryWindow, &dim[0], &dim[1]);
	//Get window positions
	glfwGetWindowPos(this->primaryWindow, &pos[0], &pos[1]);
	//Get cursor position
	glfwGetCursorPos(this->primaryWindow, &crd[0], &crd[1]);

	//Set array to the vector from the window center to the cursor position
	coords[0] = float(crd[0] - pos[0] - (dim[0] / 2));
	coords[1] = float(crd[1] - pos[1] - (dim[1] / 2));
}

void _egWindowHandler::reCenterMouse()
{
	int dim[2], pos[2];
	//Get window dimensions
	glfwGetWindowSize(this->primaryWindow, &dim[0], &dim[1]);
	//Getwindow size
	glfwGetWindowPos(this->primaryWindow, &pos[0], &pos[1]);
	
	//Set the cursor position to the center of the screen
	glfwSetCursorPos(this->primaryWindow, pos[0] + (dim[0] / 2), pos[1] + (dim[1] / 2));
}
void _egWindowHandler::setFullscreen()
{ 
	//Get screen properties
	const GLFWvidmode* scrMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//Set window dimensions to the dimensions of the screen
	glfwSetWindowSize(this->primaryWindow, scrMode->width, scrMode->height);
}

void _egWindowHandler::rename(string name)
{
	//Set new window title
	glfwSetWindowTitle(this->primaryWindow, name.c_str());
}

GLFWwindow* _egWindowHandler::getWindow()
{
	//return return pointer to window
	return this->primaryWindow;
}
_egWindowHandler::~_egWindowHandler()
{
	//Destroy the window
	glfwDestroyWindow(this->primaryWindow);
}

 

