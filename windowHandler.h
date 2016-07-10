#pragma once
#include <string>
#include"Libs\GL\glew.h"
#include"Libs\GLFW\glfw3.h"
using namespace std;

//Window type definitions
#define EG_W_WINDOWED 1
#define EG_W_FULLSCREEN 2

//Window and input handler class
class _egWindowHandler
{
public:
	/*
	Creates a new window and binds it as the current OpenGL context
	@param	type  type of window (EG_W_*)
	@param  name  title of window
	@param  width  width (in pixels) of window
	@param  heigth  heigth (in pixels) of window
	*/
	void createWindow(int type, string name, int width, int heigth);

	/*
	Changes size of window
	@param  width  new width of window
	@param  height  new height of window
	*/
	void resize(int width, int height);

	/*
	@return aspect ratio of window
	*/
	float getAspectRatio();

	/*
	Get mouse position in relation to the center of the window
	@param  coords  array to which to write the mouse position
	*/
	void getMousePos(float *coords);

	/*
	Sets the mouse position to the center of the window
	*/
	void reCenterMouse();

	/*
	Sets the window dimensions to match the screen
	*/
	void setFullscreen();

	/*
	Sets a new title for the window
	@param  name  new window title
	*/
	void rename(string name);

	/*
	@return pointer to glfw window
	*/
	GLFWwindow* getWindow();

	/*
	Deconstructor
	*/
	~_egWindowHandler();
private:
	GLFWwindow* primaryWindow;
};

//Pointer to global window handler instance
extern _egWindowHandler* egWindowHandler;
 