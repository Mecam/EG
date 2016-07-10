#include "Model.h"

void egModel::load(string modelPath)
{
	//Get full path to model file
	char moduleName[100];
	GetModuleFileName(NULL,  moduleName, 100);
	this->path = string(moduleName).substr(0, string(moduleName).find_last_of("\\")+1) + modelPath;

	//Import the model into an aiScene
	Assimp::Importer importer;
	this->scene = importer.ReadFile(this->path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	//Check if model exists, is complete, and has a root node
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  
	{
		//Notify about failure to import model
		printf("Error Importing Model at: %s\n%s\n", path.c_str(), importer.GetErrorString());
		return;
	}

	//Store model directory path for resource loading
	this->path = path.substr(0, this->path.find_last_of("\\")+1);

	//Recursively parse the model node tree starting with the root node
	this->parseNode(scene->mRootNode);
}

void egModel::draw()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw();
	egUnbindTexture();
}

void egModel::parseNode(aiNode *node)
{
	//Parse meshes and textures associated with current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		this->parseMesh(scene->mMeshes[node->mMeshes[i]]);
	}

	//Parse child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->parseNode(node->mChildren[i]);
	}
}

unsigned int egModel::parseResourceNum(aiNode * node)
{
	unsigned int count = 0;

	//Parse meshes and textures associated with current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
		count++;
	
	//Parse child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		count += parseResourceNum(node->mChildren[i]);

	return count;
}

void egModel::parseMesh(aiMesh *mesh)
{
	//Get VAO 
	egVAO *vaoPtr = parseVAO(mesh);

	//Get texture
	shared_ptr<egTexture> texPtr = parseTexture(this->scene->mMaterials[mesh->mMaterialIndex]);

	//Add mesh to model
	this->meshes.push_back(egMesh(vaoPtr, texPtr));
}

egVAO* egModel::parseVAO(aiMesh * mesh)
{
	//Create vertex data array
	egArray<egVertex> verts(mesh->mNumVertices);

	//Create index data array
	unsigned int numIndices = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		numIndices += mesh->mFaces[i].mNumIndices;
	egArray<GLuint> inds(numIndices);

	//Parse vertex data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//Parse vertex position
		verts[i].position.x = mesh->mVertices[i].x;
		verts[i].position.y = mesh->mVertices[i].y;
		verts[i].position.z = mesh->mVertices[i].z;

		//Parse vertex normal
		verts[i].normal.x = mesh->mNormals[i].x;
		verts[i].normal.y = mesh->mNormals[i].y;
		verts[i].normal.z = mesh->mNormals[i].z;

		//Check if texture coordinates are available
		if (mesh->mTextureCoords[0])
		{
			verts[i].texPos.x = mesh->mTextureCoords[0][i].x;
			verts[i].texPos.y = mesh->mTextureCoords[0][i].y;
		}
		else
			verts[i].texPos = glm::vec2(0.0f, 0.0f);
	}

	//Parse Faces
	unsigned int indxI = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		//Parse face indices
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			inds[indxI] = mesh->mFaces[i].mIndices[j];
			indxI++;
		}
	}
	//Create and return VAO
	return new egVAO(verts, inds);
}

shared_ptr<egTexture> egModel::parseTexture(aiMaterial *mat)
{
		aiString str;
		//Get location of texture
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		string texName = string(str.C_Str());

		//Check if texture has not yet been loaded
		if (this->textures.find(texName) == textures.end())
		{
			//Load the texture;
			this->textures.emplace(texName, shared_ptr<egTexture>(new egTexture(this->path + string(texName))));
		}
		return this->textures[texName];
}

egMesh::egMesh(egVAO *vao, shared_ptr<egTexture> texture)
{
	this->meshVAO = unique_ptr<egVAO>(vao);
	this->meshTexture = texture;
}

void egMesh::draw()
{
	this->meshTexture->use();
	this->meshVAO->draw();
}

