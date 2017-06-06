#include "Light.h"

//#include "Utils.h"
#include "Camera.h"

#include "glm/gtc/type_ptr.hpp"

Light::Light()
	: speed(1.0f)
{
}

Light::~Light()
{
}

Light::Light(ModelType _type, Camera* _camera)
{
	type = _type;
	camera = _camera;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	direction = glm::vec3(0.0f, -0.9f, -0.17f);

	if (type == kTriangle)
	{
		Utils::setTriData(vertices, indices);
	}
	else if (type == kQuad)
	{
		Utils::setQuadData(vertices, indices);
	}
	else if (type == kCube)
	{
		Utils::setCubeData(vertices, indices);
	}
	else if (type == kSphere)
	{
		Utils::setSphereData(vertices, indices);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	//Attributes
	glEnableVertexAttribArray(0); //position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);

	glEnableVertexAttribArray(1); //texcoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2); //normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Light::update(GLfloat _deltaTime)
{
	deltaTime = _deltaTime;
	model = glm::translate(glm::mat4(), position);
	model = glm::scale(model, scale);
}

void Light::render()
{
	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightColor"), color.x, color.y, color.z);

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//GLint viewLoc = glGetUniformLocation(program, "view");
	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	//glm::mat4 xx = camera->getViewMatrix();

	//GLint projLoc = glGetUniformLocation(program, "projection");
	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));
	//glm::mat4 yy = camera->getprojectionMatrix();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Light::moveForward()
{
	position -= glm::vec3(1.0f, 0.0f, 1.0f) * speed * deltaTime;
}

void Light::moveBackward()
{
	position += glm::vec3(1.0f, 0.0f, 1.0f) * speed * deltaTime;
}

void Light::moveLeft()
{
	position -= glm::vec3(1.0f, 0.0f, 0.0f) * speed * deltaTime;
}

void Light::moveRight()
{
	position += glm::vec3(1.0f, 0.0f, 0.0f) * speed * deltaTime;
}

void Light::moveUp()
{
	position += glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
}

void Light::moveDown()
{
	position -= glm::vec3(0.0f, 1.0f, 0.0f) * speed * deltaTime;
}


glm::vec3 Light::getColor()
{
	return color;
}

glm::vec3 Light::getPosition()
{
	return position;
}

void Light::setProgram(GLuint _program)
{
	program = _program;
}


void Light::setColor(glm::vec3 _color)
{
	color = _color;
}

void Light::setPosition(glm::vec3 _position)
{
	position = _position;
}

void Light::setScale(glm::vec3 _scale)
{
	scale = _scale;
}

void Light::setSpeed(GLfloat _speed)
{
	speed = _speed;
}

glm::vec3 Light::getDirection()
{
	return direction;
}