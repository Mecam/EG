#pragma once
#include "Entity.h"
#include "Model.h"

//Ship class
class shipEntity : public egEntityBase, public egPositionComponent
{
public:
	//Constructor
	shipEntity();
	
	//Per frame update and render procedures (overriding abstract declarations from egEntityBase)
	void update();
	void render();
private:
	//Ship model object
	egModel model;
	//Ship shader program
	egShader shader;
};