#pragma once
#include "Graphics.h"
#include "Libs\assimp\Importer.hpp"
#include "Libs\assimp\scene.h"
#include "Libs\assimp\postprocess.h"
#include <Windows.h>
#include <unordered_map>
#include <memory>

//3D mesh class
class egMesh
{
public:
	egMesh(egVAO *vao, shared_ptr<egTexture> texture);
	void draw();
private:
	unique_ptr<egVAO> meshVAO;
	shared_ptr<egTexture> meshTexture;
};

//3D model class
class egModel
{
public:
	/*
	Load model data from file
	@param  modelPath  path of model file in relation to 
	*/
	void load(string modelPath);

	/*
	Draws the model
	*/
	void draw();

private:
	/*
	Function for recursively parsing the model node tree
	@param  node  pointer to the node to be parsed
	@param  scene  pointer to the model scene
	*/
	void parseNode(aiNode *node);

	/*
	Function for recursively parsing model node tree
	and counting the number of Resources
	@param  node  pointer to the node to be parsed
	@return	 total  number of resources in the section of node tree starting at this node
	*/
	unsigned int parseResourceNum(aiNode *node);

	/*
	Function for parsing and converting meshes into Vertex Array Objects
	@param  mesh  pointer to the mesh to be parsed
	@return  parsed mesh VAO
	*/
	void parseMesh(aiMesh *mesh);

	/*
	Function for parsing vertex/element data from meshes
	@param  mesh  pointer for mesh to be parsed
	@return  pointer to VAO containing parsed data
	*/
	egVAO* parseVAO(aiMesh *mesh);

	/*
	Function for parsing textures from materials
	@param  mat  pointer to material to be parsed
	@return  pointer to texture object
	*/
	shared_ptr<egTexture> parseTexture(aiMaterial *mat);

	//Vector for model meshes
	vector<egMesh> meshes;

	//Path to folder containing model file
	string path;

	//Pointer to scene containing mesh and material data
	const aiScene* scene;

	//Map for texture data
	unordered_map<string, shared_ptr<egTexture>> textures;
};
