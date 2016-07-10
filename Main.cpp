 
#define GLEW_STATIC
//Game Engine 
#include "gameCore.h"
//Game entity definitions
#include "gameEntities.h"


int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
	_CrtSetDbgFlag(_CRTDBG_CHECK_CRT_DF);
	//Initialize window and input handling
	egInitCore();
	//Create window
	egWindowHandler->createWindow(EG_W_WINDOWED, "Test", 800, 800);
	//Initialize OpenGL extensions
	//(Done after window creation as it requires an active OpenGL context)
	egInitGraphics();

	//Initialize ship entity, game scene, and loop objects
	shipEntity ship;
	egGameScene scene;
	egGameLoop loop;

	//Set initial camera location and direction
	egCamera->setPos(glm::vec3(0.0f, 0.0f, 3.0f));
	egCamera->setDirection(glm::vec3(0.0f, 0.0f, -1.0f));

	//Add ship entity to game scene
	scene.addEntity(&ship);
	//Set the scene as the current scene for the game loop
	loop.setScene(&scene);

	//Run the loop
	loop.run();

	//Terminate engine
	egQuit();
	return 0;
}