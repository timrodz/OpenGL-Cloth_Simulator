#include "Camera.h"

#include "Utils.h"


Camera::Camera()
	: Position(glm::vec3(0.0f, 0.0f, 0.0f))
	, Front(glm::vec3(0.0f, 0.0f, -1.0f))
	, Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, Right(1.0f, 0.0f, 0.0f)
	, WorldUp(0.0f, 1.0f, 0.0f)
	, Yaw(-90.0f)
	, Pitch(0.0f)
	, FOV(45.0f)
	, CameraSpeed(3.0f)
	, MouseSensitivity(0.25f)
	, width(Utils::WIDTH)
	, height(Utils::HEIGHT)
	, NearClipping(0.1f)
	, FarClipping(1000.0f)
{
	updateCameraVectors();
}

Camera::Camera(GLfloat _fov, GLuint _width, GLuint _height, GLfloat zNear, GLfloat zFar)
{
	FOV = _fov;
	width = _width;
	height = _height;
	NearClipping = zNear;
	FarClipping = zFar;

	Position = glm::vec3(0.0f, 0.0f, 10.0f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = -90.0f;
	Pitch = 0.0f;

	MouseSensitivity = 0.25f;
}

Camera::~Camera()
{

}

glm::mat4 Camera::getprojectionMatrix()
{
	return glm::perspective(glm::radians(FOV), (GLfloat)width / (GLfloat)height, NearClipping, FarClipping);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = CameraSpeed * deltaTime;
	if (direction == FORWARD)
	{
		Position += Front * velocity;
	}		
	if (direction == BACKWARD)
	{
		Position -= Front * velocity;
	}		
	if (direction == LEFT)
	{
		Position -= Right * velocity;
	}		
	if (direction == RIGHT)
	{
		Position += Right * velocity;
	}
	if (direction == UPWARD)
	{
		Position += Up * velocity;
	}
	if (direction == DOWNWARD)
	{
		Position -= Up * velocity;
	}

	//glTranslatef(Position.x, Position.y, Position.z);

	//glRotatef(-Pitch, 1.0, 0.0, 0.0);
	//glRotatef(-Yaw, 0.0, 1.0, 0.0);
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

	// Update Front, Right and Up Vectors using the updated Eular angles
	//this->updateCameraVectors();

	//glTranslatef(Position.x, Position.y, Position.z);

	//glRotatef(-Pitch, 1.0, 0.0, 0.0);
	//glRotatef(-Yaw, 0.0, 1.0, 0.0);
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
//void Camera::ProcessMouseScroll(GLfloat yoffset)
void Camera::ProcessMouseScroll(int yoffset)
{
	if (FOV >= 1.0f && FOV <= 120.0f)
	{
		FOV -= yoffset;
	}
	FOV = glm::clamp(FOV, 1.0f, 120.0f);
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 look;
	look.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	look.y = sin(glm::radians(Pitch));
	look.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(look);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));

}

glm::vec3 Camera::getCameraPosition()
{
	return Position;
}

void Camera::setCameraSpeed(GLfloat _speed)
{
	CameraSpeed = _speed;
}

void Camera::moveForward()
{
	Position += Front * CameraSpeed;
}

void Camera::moveBack()
{
	Position -= Front * CameraSpeed;
}

void Camera::moveLeft()
{
	Position -= Right * CameraSpeed;
}

void Camera::moveRight()
{
	Position += Right * CameraSpeed;
}

void Camera::moveUp()
{
	Position += Up * CameraSpeed;
}

void Camera::moveDown()
{
	Position -= Up * CameraSpeed;
}

void Camera::update()
{
	updateCameraVectors();
}

void Camera::setCameraFront(glm::vec3 _front)
{
	Front = _front;
}

glm::vec3 Camera::getLook()
{
	return Front;
}

void Camera::setPosition(glm::vec3 _position)
{
	Position = _position;
}