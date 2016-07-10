#include "Graphics.h"


egVAO::egVAO(egArray<egVertex> vertices, egArray<GLuint> indices) : vData(vertices), iData(indices)
{
	//Generate buffers
	GLuint vbo, ebo;
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	
	//Bind Vertex and Element Buffer Objects to the Vertex Array Object
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	//Load data from vector to buffers
	glBufferData(GL_ARRAY_BUFFER, vData.getSize(), vData.getData() , GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iData.getSize(), iData.getData(), GL_STATIC_DRAW);

	//Convigure buffer properties for shader access
	//Vertex Position Attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2 + sizeof(glm::vec2), (GLvoid*)0);
	//Vertex Normal Attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2 + sizeof(glm::vec2), (GLvoid*)sizeof(glm::vec3));
	//Vertex Texture Coord Attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2 + sizeof(glm::vec2), (GLvoid*)(2 * sizeof(glm::vec3)));

	//Delete VBO and EBO as their memory spaces are now available via the VAO
	//glDeleteBuffers(1, &vbo);
	//glDeleteBuffers(1, &ebo);

	egUnbindVAO();
}

 
void egVAO::draw()
{
	//Bind the vao
	this->bind();
	 
	glDrawElements(GL_TRIANGLES, this->iData.getSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}

egVAO::~egVAO()
{
	//Delete the VAO
	glDeleteBuffers(1, &this->vao);
}

void egUnbindVAO()
{
	//Bind null as the active VAO
	glBindVertexArray(0);
}

void egVAO::bind()
{
    //Bind the VAO 
	glBindVertexArray(this->vao);
}

void egShader::attachProg(GLenum type, const GLchar * source)
{
	//Create a shader component instance
	_ShaderProg shader;
	//Compile shader source code
	shader.compile(type, source);
	//Add the component to the container vector
	components.push_back(shader);
}

void egShader::link()
{
	//Create shader program
	this->program = glCreateProgram();
	for (unsigned int i = 0; i < components.size(); i++)
	{
		//Attach all components
		glAttachShader(this->program, this->components[i].shader);
	}
	//Link the program
	glLinkProgram(this->program);

	//Check if program linked successfully
	GLint success;
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetProgramInfoLog(this->program, 512, NULL, info);
		printf("Error Linking Shader Program!\n%s\n", info);
	}
    
	//Delete shader components
	for (unsigned int i = 0; i < components.size(); i++)
	{
		glDeleteShader(this->components[i].shader);
	}
}

void egShader::loadMatrices(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	//Load defualt texture buffer index into sampler uniform
	glUniform1i(glGetUniformLocation(this->program, "tex"), 0);
	//Load matrix uniforms
	glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(this->program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(this->program, "proj"), 1, GL_FALSE, glm::value_ptr(projection));
}

void egShader::configureLighting(glm::vec3 viewPos, glm::vec3 lightPos, glm::vec4 lightColor)
{
	//Load vector uniforms
	glUniform3fv(glGetUniformLocation(this->program, "viewPos"), 1, glm::value_ptr(viewPos));
	glUniform3fv(glGetUniformLocation(this->program, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform3fv(glGetUniformLocation(this->program, "lightColor"), 1, glm::value_ptr(lightColor));
}
 
void egShader::use()
{
	//Set this program as the active shader program
	glUseProgram(this->program);
}

void _ShaderProg::compile(GLenum type, const GLchar * source) 
{
	this->type = type;
	//Create shader component
	this->shader = glCreateShader(type);
	//Compile shader component source code
	glShaderSource(this->shader, 1, &source, NULL);
	glCompileShader(this->shader);

	//Check if compiled
	GLint success;
	glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetShaderInfoLog(this->shader, 512, NULL, info);
		printf("Error Compiling Shader of type: %d\n%s\n", type, info);
	}
}

void egUnbindTexture()
{
	//Bind null to texture buffer
	glBindTexture(GL_TEXTURE_2D, 0);
}
egTexture::egTexture(string path)
{
	//Generate texture
	glGenTextures(1, &this->texture);
	//Bind texture to the default texture buffer
	glBindTexture(GL_TEXTURE_2D, this->texture);
	//Set default texture buffer as active
	glActiveTexture(GL_TEXTURE0);
	//Clamp texture proportions to maximum coordinate sampled
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Color interpolation settings
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h;
	//Load image into string format
	unsigned char* image = SOIL_load_image(path.c_str(), &w, &h, 0, SOIL_LOAD_RGBA);
	//Load texture into buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//Delete texture string
	SOIL_free_image_data(image);
	//Unbind texture
	egUnbindTexture();
}

void egTexture::use()
{
	//Set default texture buffer as active
	glActiveTexture(GL_TEXTURE0);
	//Bind this texture to the active texture buffer
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

egTexture::~egTexture()
{
	glDeleteTextures(1, &this->texture);
}
