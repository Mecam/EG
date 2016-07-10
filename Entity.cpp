#include "Entity.h"
 
void egPositionComponent::setPos(glm::vec3 newPos)
{ 
	//Set the model matrix translation vector to the new position
	this->modelMatrix.translate = newPos;
}

glm::vec3 egPositionComponent::getPos()
{
	//Return the model matrix translation vector
	return this->modelMatrix.translate;
}

_egCamera::_egCamera()
{
	//Set defualt camera properties
	this->location = glm::vec3(-1.0f, 0.0f, 0.0f);
	this->direction = glm::vec3(1.0f, 0.0f, 0.0f);
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->cameraRight = glm::vec3(0.0f, 0.0f, 1.0f);
	this->camMult = 0.25f;
	this->field = 20.0f;
}

glm::vec3 _egCamera::getLocation()
{
	//Return location vector
	return this->location;
}

void _egCamera::setPos(glm::vec3 pos)
{
	//Set location vector to new position
	this->location = pos;
}

void _egCamera::setFOV(float fov)
{
	//Make sure that the field of view is not too wide or narrow, as this may cause visual distortion
	if (45.0f >= fov && fov >= 5.0f)
		//Set new field of view value
		this->field = fov;
}

void _egCamera::setSenitivity(float ammount)
{
	//Set new camera movement sensitivity value
	this->camMult = ammount;
}

void _egCamera::setDirection(glm::vec3 dir)
{
	//Set new direction vector
	this->direction = dir;
	//Update camera right vector based on new direction vector and world up 
	this->cameraRight = glm::normalize(glm::cross(this->direction, glm::vec3(0.0f, 1.0f, 0.0f)));
	//Update camera up vector based on new direction and right vectors
	this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->direction));
}

void _egCamera::updateDirection()
{
	//Get cursor position
	float mPos[2];
	egWindowHandler->getMousePos(mPos);

	//Rotate camera direction vector by mouse y position delta
	this->direction = glm::rotate(this->direction, -glm::radians(max(min(mPos[1] * this->camMult, 90.0f), -90.0f)), this->cameraRight);
	//Rotate camera direction vector by mouse x position delta
	this->direction = glm::rotate(this->direction, -glm::radians(max(min(mPos[0] * this->camMult, 90.0f), -90.0f)), glm::vec3(0.0f, 1.0f, 0.0f));
	//Update camera right vector based on new direction vector and world up 
	this->cameraRight = glm::normalize(glm::cross(this->direction, glm::vec3(0.0f, 1.0f, 0.0f)));
	//Update camera up vector based on new direction and right vectors
	this->cameraUp = glm::normalize(glm::cross(this->cameraRight, this->direction));
	
	//Re-center mouse position 
	egWindowHandler->reCenterMouse();
}

void _egCamera::updatePosition()
{
	//Move camera forward if W key is pressed
	this->location += this->direction * 2.0f * (1.0f / 60.0f) * (float)glfwGetKey(egWindowHandler->getWindow(), GLFW_KEY_W);
	//Move camera backwards if S key is pressed
	this->location -= this->direction * 2.0f * (1.0f / 60.0f) * (float)glfwGetKey(egWindowHandler->getWindow(), GLFW_KEY_S);
	//Move camera right if D key is pressed
	this->location += this->cameraRight * 2.0f * (1.0f / 60.0f) * (float)glfwGetKey(egWindowHandler->getWindow(), GLFW_KEY_D);
	//Move camera left if A key is pressed
	this->location -= this->cameraRight * 2.0f * (1.0f / 60.0f) * (float)glfwGetKey(egWindowHandler->getWindow(), GLFW_KEY_A);
}

glm::mat4 _egCamera::getViewMat()
{
	//Generate camera view matrix
	glm::mat4 view = glm::lookAt(this->location, this->location + this->direction, this->cameraUp);
	return view;
}

glm::mat4 _egCamera::getProjMat()
{
	//Generate camera projection matrix
	glm::mat4 proj = glm::perspective(this->field, (GLfloat)egWindowHandler->getAspectRatio(), 0.1f, 100.0f);
	return proj;
}

void egGameScene::update()
{
	//Update each contained entity
	for (unsigned int i = 0; i < entities.size(); i++)
		entities[i]->update();
}

void egGameScene::render()
{
	//Render each contained entity
	for (unsigned int i = 0; i < entities.size(); i++)
		entities[i]->render();
}

void egGameScene::addEntity(egEntityBase * entity)
{
	//Set this scene as the container scene for the entity
	entity->scene = this;
	//Append entity to container
	entities.push_back(entity);
}

void egGameScene::removeEntity(egEntityBase * entity)
{
	//Search for and remove entity with matching pointer
	for (unsigned int i = 0; i < entities.size(); i++)
		if (entities[i] == entity)
			entities.erase(entities.begin() + i);
}
