#pragma once
#include "Entity.h"
#include "windowHandler.h"
#include <time.h>

//Initialize engine window handling and I/O
void egInitCore();
//Initialize OpenGL graphics extensions
void egInitGraphics();
//Terminate engine
void egQuit();


//Game loop class
class egGameLoop
{
public:
	/*
	Set current game scene
	@param  scene  pointer to game scene
	*/
	void setScene(egGameScene* scene);

	//Run game loop
	void run();
private:
	//Pointer to current game scene
    egGameScene* currentScene;
};

 