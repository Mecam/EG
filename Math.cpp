#include "Math.h"
 
//Set scale to default
egModelMatrix::egModelMatrix() : scale(1.0f){}

void egModelMatrix::addRotation(glm::vec3 axis, float angle)
{
	//Applies new rotation to the rotation quaternion
	this->rotation = glm::angleAxis(angle, axis) * this->rotation;
}

glm::mat4 egModelMatrix::getMatrix()
{
	//Initialize matrix instance
	glm::mat4 model;
	//Apply scaling transformation
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	//Apply rotation transformation
	model *= glm::mat4_cast(this->rotation);
	//Apply translation transformation
	glm::translate(model, translate);
	//Return complete model matrix
	return model;
}
