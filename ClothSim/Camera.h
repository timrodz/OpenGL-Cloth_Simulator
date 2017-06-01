#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include "glew/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UPWARD,
	DOWNWARD
};

//// Default camera values
//const GLfloat YAW = -90.0f;
//const GLfloat PITCH = 0.0f;
//const GLfloat SPEED = 3.0f;
//const GLfloat SENSITIVTY = 0.25f;
//const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{

public:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles (in radians)
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera parameters
	GLfloat CameraSpeed;
	GLfloat MouseSensitivity;
	//GLfloat Zoom;
	GLfloat FOV; // degrees

	GLfloat NearClipping;
	GLfloat FarClipping;
	GLuint width;
	GLuint height;

	Camera();
	Camera(GLfloat _fov, GLuint _width, GLuint _height, GLfloat zNear, GLfloat zFar);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getprojectionMatrix();
	void updateCameraVectors();
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	//void ProcessMouseScroll(GLfloat yoffset);
	void ProcessMouseScroll(int yoffset);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);
	
	glm::vec3 getCameraPosition();
	void setCameraSpeed(GLfloat _speed);
	void moveForward();
	void moveBack();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void update();
	void setCameraFront(glm::vec3 _front);
	glm::vec3 getLook();
	void setPosition(glm::vec3 _position);
};
