#include "gameEntities.h"

//Shader: program executed by GPU
//Vertex Shader: runs once for each vertex to be processed
//Fragment Shader: runs once for each fragment to be processed
//Fragment: a 3-dimensional pixel of sort, contains a depth value and is interpolated from area inbetween vectors
//Uniform: variable from program memory rather than buffer, loaded once per program cycle

//Source code for ship vertex shader
char* SVshader = GLSL(

//Vertex data from Vertex Buffer Object
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texPos;

//Output to fragment shader
out vec3 fragPos;
out vec3 fragNormal;
out vec2 texCoords;

//Matrices used to convert from model space to world space to view space to projection space
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	//Output world space position, normal and texture coordinates
	fragPos = vec3(model * vec4(position, 1.0f));
	fragNormal = vec3(normalize(model * vec4(normal, 1.0f)));
	texCoords = texPos;

	//Output final vertex position to render pipeline
	gl_Position = proj * view * vec4(fragPos, 1.0f);
}
);

//Source code for ship fragment shader
char* Fshader = GLSL(
//Data interpolated from vertex shader output
in vec3 fragPos;
in vec3 fragNormal;
in vec2 texCoords;

//Final color of Fragment
out vec4 colorOut;

//Light source properties
uniform vec4 lightColor;
uniform vec3 lightPos;

//Position of camera
uniform vec3 viewPos;

//Sampler for diffuse texture map
uniform sampler2D tex;

void main()
{
	//Read color from texture
	vec4 objectColor = texture(tex, texCoords).rgba;

	//Direction from light source to fragment
	vec3 lightDir = normalize(fragPos - lightPos);
	vec3 reflectDir = normalize(reflect(lightDir, fragNormal));
	//Direction fragment to camera
	vec3 viewDir = normalize(viewPos - fragPos);
	//Vector inbetween lightDir and viewDir
	vec3 midway = normalize(-lightDir + viewDir);

	//Ambient lighting represents residual light rays not accounted for by other calculations
	float amb = 0.5f;
	vec3 ambient = amb * vec3(objectColor);

	//Diffusion lighting represents light radiated by object when hit by photons from the light source
	//The closer the light direction is to the surface normal, the greater the diffuse intensity
	float diff = max(dot(fragNormal, -lightDir), 0.0f);
	vec3 diffuse = diff * vec3(objectColor);
	
	//Specular lighting represents light from the light source which has reflected off of the object
	//The closer midway vector between lightDir and viewDir is to the surface normal, the greater the specular intensity
	float spec = pow(max(dot(midway, fragNormal), 0.0f), 32.0f);
	vec3 specular = spec * vec3(lightColor);

	colorOut = vec4(ambient + diffuse + specular, objectColor.w);
}
);
 
shipEntity::shipEntity()
{
	//Model from: http://www.kevinboone.net/black_pearl.html
	this->model.load("resources\\black_pearl.obj");
	//Attach vertex shader
	this->shader.attachProg(GL_VERTEX_SHADER, SVshader);
	//Attach fragment shader
	this->shader.attachProg(GL_FRAGMENT_SHADER, Fshader);
	//Link shader program
	this->shader.link();
	//Adjust model scale
	this->modelMatrix.scale = 0.1f;
}

//Not implemented
void shipEntity::update()
{
}

void shipEntity::render()
{
	//Activate  and configure shader shader
	this->shader.use();
	//Configure matrices
	this->shader.loadMatrices(this->modelMatrix.getMatrix(), egCamera->getViewMat(), egCamera->getProjMat());
	//Configure lighting
	this->shader.configureLighting(egCamera->getLocation(), glm::vec3(10.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.00f));
	//Draw model
	this->model.draw();
}
