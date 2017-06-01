#pragma once

#include "Utils.h"
//class Utils;

class Camera;

class Light
{
public:
	Light();
	Light(ModelType _type, Camera* _camera);
	~Light();

	void setProgram(GLuint _program);
	void setColor(glm::vec3 _color);
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setSpeed(GLfloat _speed);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void update(GLfloat _deltaTime);
	void render();

	glm::vec3 getColor();
	glm::vec3 getPosition();

	glm::vec3 getDirection();

private:

	Camera* camera;
	ModelType type;
	GLuint program;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 scale;
	GLfloat speed;

	std::vector<VertexFormat> vertices;
	std::vector<GLuint> indices;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	glm::mat4 model;
	GLfloat deltaTime;

	glm::vec3 direction;
};


