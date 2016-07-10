#pragma once
#define GLSL(src) "#version 330 core\n" #src
#include "Libs\glm\glm.hpp"
#include "Libs\glm\gtc\type_ptr.hpp"
#include "Libs\GL\glew.h"
#include "Libs\SOIL\SOIL.h"
#include <vector>
#include <memory>

using namespace std;

//Dynamic Array Class
template<class T>
class egArray
{
public:
	egArray(GLuint size) : size(size)
	{
		this->ptr = shared_ptr<T>(new T[size]);
	}

	GLuint getSize()
	{
		return this->size * sizeof(T);
	}

	GLvoid* getData()
	{
		return (GLvoid*)ptr.get();
	}

	T& operator[](GLuint index)
	{
		return ptr.get()[index];
	}
private:
	shared_ptr<T> ptr;
	GLuint size;
};

//Vertex class
struct egVertex
{
	//Vertex position
	glm::vec3 position;
	//Vertex normal (vector perpendicular to the face composed by the vertex)
	glm::vec3 normal;
	//Texture coordinates 
	glm::vec2 texPos;
};

/*
Binds null to texture buffer 0
*/
void egUnbindTexture();

//Texture class
class egTexture
{
public:
	/*
	Constructor
	@param  path  complete path to texture file
	*/
	egTexture(string path);

	/*
	Binds txture to texture buffer 0
	*/
	void use();

	/*
	Deconstructor
	*/
	~egTexture();
private:
	//Texture ID
	GLuint texture;
};

//Shader component class
class _ShaderProg
{
public:
	/*
	Compile shader component
	@param  type  shader component type
	@param  source  string containing shader source code
	*/
	void compile(GLenum type, const GLchar* source);
	
	//Shader component type
	GLenum type;
	//Shader component ID
	GLuint shader;
};

//Shader program class
class egShader
{
public:
	/*
	Attach shader component to program
	@param  type  shader component type
	@param  source  string containing shader source code
	*/
	void attachProg(GLenum type, const GLchar* source);

	/*
	Link program
	*/
	void link();

	/*
	Set this program as the active shader program
	*/
	void use();

	/*
	Configure matrix uniforms
	@param  model  transformation matrix
	@param  view  view space matrix
	@param  projection  projection space matrix
	*/
	void loadMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

	/*
	Configure lighting setting uniforms
	@param  viewPos  position of camera
	@param  lightPos  position of light source
	@param  lightColor  color of light
	*/
	void configureLighting(glm::vec3 viewPos, glm::vec3 lightPos, glm::vec4 lightColor);
private:
	//Program ID
	GLuint program;
	//Vector for shader components
	vector<_ShaderProg> components;
};
void egUnbindVAO();

//Vertex Array Object Class
class egVAO
{
public:
	/*
	Constructor
	@param vertices array containing vertex data
	@param indices array containing index data
	*/
	egVAO(egArray<egVertex> vertices, egArray<GLuint> indices);

	/*
	Bind this VAO
	*/
	void bind();

	/*
	Draw this VAO
	*/
	void draw();

	//Deconstructor
	~egVAO();
protected:
	//VAO, vertex buffer, and element buffer IDs
	GLuint vao;
	//Array for vertex data
	egArray<egVertex> vData;
	//Array for index data
	egArray<GLuint> iData;
};