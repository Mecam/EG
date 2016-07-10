#pragma once
#include"Libs\GL\glew.h"
#include"Libs\GLFW\glfw3.h"
#include "Event.h"
#include "windowHandler.h"
class _egInputHandler : public egEventInterface
{
 
friend void keyboard_callback(GLFWwindow*, int, int, int, int);
friend void mousekey_callback(GLFWwindow*, int, int, int);
friend void mousepos_callback(GLFWwindow*, double, double);
friend void scrollOff_callback(GLFWwindow*, double, double);
public:
	void init(GLFWwindow*);
	float* getMousePos();
	double* getScrollOffset();
private:
	float mousePos[4];
	double scrollOff[2];
	
};

extern _egInputHandler *egInputHandler;
 