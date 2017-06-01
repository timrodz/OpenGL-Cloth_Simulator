#include "Cloth.h"

#include "Camera.h"

#include "glm/gtc/type_ptr.hpp"

Cloth::Cloth()
	: speed(1.0f)
{
}

Cloth::~Cloth()
{
}

Cloth::Cloth(std::vector<Position> _vecPosition, Camera* _camera)
{
	camera = _camera;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	color = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, -0.9f, -0.17f);

	std::vector<VertexFormat> vertices;

	vertices.push_back(VertexFormat(_vecPosition[0], TexCoord(1, 1), Normals(0.0f, 0.0f, 1.0)));
	vertices.push_back(VertexFormat(_vecPosition[1], TexCoord(0, 1), Normals(0.0f, 0.0f, 1.0)));
	vertices.push_back(VertexFormat(_vecPosition[2], TexCoord(1, 0), Normals(0.0f, 0.0f, 1.0)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
}

void Cloth::update(GLfloat _deltaTime)
{
	deltaTime = _deltaTime;
	model = glm::translate(glm::mat4(), position);
	model = glm::scale(model, scale);
}

void Cloth::render()
{
	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightColor"), color.x, color.y, color.z);

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	glm::mat4 view = camera->getViewMatrix();

	GLint projLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));
	glm::mat4 projection = camera->getprojectionMatrix();

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

glm::vec3 Cloth::getColor()
{
	return color;
}

glm::vec3 Cloth::getPosition()
{
	return position;
}

void Cloth::setProgram(GLuint _program)
{
	program = _program;
}


void Cloth::setColor(glm::vec3 _color)
{
	color = _color;
}

void Cloth::setPosition(glm::vec3 _position)
{
	position = _position;
}

void Cloth::setScale(glm::vec3 _scale)
{
	scale = _scale;
}

void Cloth::setSpeed(GLfloat _speed)
{
	speed = _speed;
}

glm::vec3 Cloth::getDirection()
{
	return direction;
}