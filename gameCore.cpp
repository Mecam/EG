#include "gameCore.h"

//Pointer to global window handler instance
_egWindowHandler* egWindowHandler;
//Pointer to global camera instance
_egCamera *egCamera;

void handleInput()
{
	//Terminate the program if the escape key is pressed
	if (glfwGetKey(egWindowHandler->getWindow(), GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(egWindowHandler->getWindow(), true);
}
 
void egInitCore()
{
	//Initialize GLFW library
	glfwInit();
	
	//Initialize global window handler instance
	egWindowHandler = new _egWindowHandler();
	//Initialize global camera instance
	egCamera = new _egCamera();
}

void egInitGraphics()
{
	//Enable OpenGL 3.0+ functionality
	glewExperimental = GL_TRUE;

	//Make sure OpenGL extensions have loaded properly
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		printf("Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	//Enable testing of fragment depth values
	glEnable(GL_DEPTH_TEST);
}
void egQuit()
{
	//Delete global instances
	delete egWindowHandler;
	delete egCamera;
}

void egGameLoop::setScene(egGameScene* scene)
{
	//Set the new current scene
	this->currentScene = scene;
}
void egGameLoop::run()
{
	//Frequency to update and render program at (in clocks)
	clock_t updateTime = CLOCKS_PER_SEC / 60;
	//Time accumulator
	clock_t acc = 0;
	//Current time (in clocks)
	clock_t currentTime = clock();

	//While the program should not close
	while (!glfwWindowShouldClose(egWindowHandler->getWindow()))
	{
		//Get new current time
		clock_t newTime = clock();
		//Add the delta time to the accumulator
		acc += newTime - currentTime;
		//Set the new current time as the current time
		currentTime = newTime;

		//If it is time for a update/render procedure
		if (acc >= updateTime)
		{
			//Check for new events
			glfwPollEvents();
			//Update scene entities
			this->currentScene->update();
			//Update camera position
			egCamera->updatePosition();
			//Update camera direction
			egCamera->updateDirection();
			//Handle input for loop
			handleInput();

			//Clear framebuffer color and depth values
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//Render scene entities
			this->currentScene->render();
			//Swap window framebuffers
			glfwSwapBuffers(egWindowHandler->getWindow());
			 
			//Decrease accumulator value
			acc -= updateTime;
		}
	}
}


