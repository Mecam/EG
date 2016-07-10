#pragma once
#include "Libs\glm\glm.hpp"
#include "Libs\glm\gtx\quaternion.hpp"
#include "Libs\glm\gtc\matrix_transform.hpp"
#include "Libs\glm\gtx\rotate_vector.hpp"
#include <algorithm>

//Model matrix object
//Used to represent an entity's orientation in the game world
class egModelMatrix
{
public:
	egModelMatrix();

	/*
	Adds a rotation operation to the matrix
	@param  axis  The axis the rotation is performed around
	@param  angle  Rotation angle in degrees
	*/
	void addRotation(glm::vec3 axis, float angle);

	/*
	@return	 the assembled model matrix
	*/
	glm::mat4 getMatrix();

	//Scale of the entity (as fraction)
	float scale;
	//Quaternion which represents all rotations rotations to be performed
	glm::quat rotation;
	//Location of entity in relation to world origin
	glm::vec3 translate;
};