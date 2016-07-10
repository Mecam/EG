#pragma once
#include <vector>
#include "Libs\GL\glew.h"
#include "Math.h"
#include "Model.h"
#include "windowHandler.h"
#include "Libs\glm\gtx\string_cast.hpp"
class egGameScene;

//Camera class
class _egCamera
{
public:
	//Constructor
	_egCamera();

	/*
	@return  position of camera
	*/
	glm::vec3 getLocation();

	/*
	Set new camera position
	@param  pos  new camera position
	*/
	void setPos(glm::vec3 pos);

	/*
	Set new field of view range
	@param fov  new field of view range (degrees)
	*/
	void setFOV(float fov);

	/*
	Set new mouse sensitivity value
	@param  ammount  new sensitivity value
	*/
	void setSenitivity(float ammount);

	/*
	Set new camera direction
	@param  dir  new camera direction
	*/
	void setDirection(glm::vec3 dir);

	//Per frame camera position and direction update functions
	void updatePosition();
	void updateDirection();

	/*
	@return  camera view matrix
	*/
	glm::mat4 getViewMat();

	/*
	@return  camera projection matrix
	*/
	glm::mat4 getProjMat();
private:
	//Camera position vector
	glm::vec3 location;
	//Camera direction vector
	glm::vec3 direction;
	//Camera up vector
	glm::vec3 cameraUp;
	//Camera right vector
	glm::vec3 cameraRight;
	//Mouse sensitivity
	float camMult;
	//Field of view range
	float field;
};
//Pointer to global camera instance
extern _egCamera *egCamera;

//Abstract class for game entities
class egEntityBase
{
public:
	//Per frame update and render functions
	virtual void update() = 0;
	virtual void render() = 0;
	//Pointer to game scene containing the entity
	egGameScene* scene;
};

//Game scene class
class egGameScene
{
public:
	//Update entities
	void update();
	//Draw entities
	void render();
	/*
	Add Entity to scene
	@param  entity  pointer to entity
	*/
	void addEntity(egEntityBase *entity);
	/*
	Remove Entity from scene
	@param  entity  pointer to entity which is to be removed
	*/
	void removeEntity(egEntityBase *entity);
private:
	//Vector for entities in the scene
	vector<egEntityBase*> entities;
};


 
class egPositionComponent
{
public:
	void setPos(glm::vec3 newPos);
	glm::vec3 getPos();
protected:
	egModelMatrix modelMatrix;
};



 