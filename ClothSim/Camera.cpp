#include "Camera.h"

#include "Utils.h"

#define BUTTON_UP		0
#define BUTTON_DOWN		1

Camera::Camera()
{
	camPitch = 0;
	camYaw = 0;
	movevel = 0.2;
	mousevel = 0.2;
	mi = ismoved = false;
}

Camera::Camera(glm::vec3 l)
{
	loc = l;
	camPitch = 0;
	camYaw = 0;
	movevel = 0.2;
	mousevel = 0.2;
	mi = ismoved = false;
}

Camera::Camera(glm::vec3 l, float yaw, float pitch)
{
	loc = l;
	camPitch = pitch;
	camYaw = yaw;
	movevel = 0.2;
	mousevel = 0.2;
	mi = ismoved = false;
}

Camera::Camera(glm::vec3 l, float yaw, float pitch, float mv, float mov)
{
	loc = l;
	camPitch = pitch;
	camYaw = yaw;
	movevel = mv;
	mousevel = mov;
	mi = false;
}


void Camera::lockCamera()
{
	if (camPitch>90)
		camPitch = 90;
	if (camPitch<-90)
		camPitch = -90;
	if (camYaw<0.0)
		camYaw += 360.0;
	if (camYaw>360.0)
		camYaw -= 360;
}

void Camera::moveCamera(float dir)
{
	//float rad = (camYaw + dir)*M_PI / 180.0;
	float rad = glm::radians(camYaw + dir);
	loc.x -= sin(rad)*movevel;
	loc.z -= cos(rad)*movevel;
}

void Camera::moveCameraUp(float dir)
{
	//float rad = (camPitch + dir)*M_PI / 180.0;
	float rad = glm::radians(camPitch + dir);
	loc.y += sin(rad)*movevel;
}


void Camera::Control(unsigned char state[255])
{
	/*if (mi)
	{*/
		int MidX = Utils::WIDTH; //320;
		int MidY = Utils::HEIGHT; // 240;

		//SDL_ShowCursor(SDL_DISABLE);

		//int tmpx, tmpy;
		//SDL_GetMouseState(&tmpx, &tmpy);
		//camYaw += mousevel*(MidX - tmpx);
		//camPitch += mousevel*(MidY - tmpy);
		lockCamera();
		//SDL_WarpMouse(MidX, MidY);
		//Uint8* state = SDL_GetKeyState(NULL);
		ismoved = false;

		if (state[(unsigned char) 'w'] == BUTTON_DOWN)
		{
			ismoved = true;
			if (camPitch != 90 && camPitch != -90)
				moveCamera(0.0);
			moveCameraUp(0.0);
		}
		else if (state[(unsigned char) 's'] == BUTTON_DOWN)
		{
			ismoved = true;
			if (camPitch != 90 && camPitch != -90)
				moveCamera(180.0);
			moveCameraUp(180.0);
		}
		if (state[(unsigned char) 'a'] == BUTTON_DOWN)
		{
			ismoved = true;
			moveCamera(90.0);
		}
		else if (state[(unsigned char) 'd'] == BUTTON_DOWN)
		{
			ismoved = true;
			moveCamera(270);
		}
	//}
	glRotatef(-camPitch, 1.0, 0.0, 0.0);
	glRotatef(-camYaw, 0.0, 1.0, 0.0);
}

void Camera::UpdateCamera()
{
	glLoadIdentity();
	glTranslatef(-loc.x, -loc.y, -loc.z);
}

//change the spherical coordinate system to cartesian
glm::vec3 Camera::getVector()
{
	//Google->spherical to cartesian
	return (glm::vec3(-cos(glm::radians(camPitch))*sin(glm::radians(camYaw)),
		sin(glm::radians(camPitch)),
		-cos(glm::radians(camPitch)*cos(glm::radians(camYaw)))));
}
glm::vec3 Camera::getLocation()
{
	return loc;
}

float Camera::getPitch()
{
	return camPitch;
}

float Camera::getYaw()
{
	return camYaw;
}
float Camera::getMovevel()
{
	return movevel;
}
float Camera::getMousevel()
{
	return mousevel;
}

bool Camera::isMouseIn()
{
	return mi;
}

void Camera::setLocation(glm::vec3 vec)
{
	//loc.change(vec);
	loc = vec;
}

void Camera::lookAt(float pitch, float yaw)
{
	camPitch = pitch;
	camYaw = yaw;
}

void Camera::mouseIn(bool b)
{
	mi = b;
}

void Camera::setSpeed(float mv, float mov)
{
	movevel = mv;
	mousevel = mov;
}

bool Camera::isMoved()
{
	return ismoved;
}


//
//
//Camera::Camera()
//	: Position(glm::vec3(0.0f, 0.0f, 0.0f))
//	, Front(glm::vec3(0.0f, 0.0f, -1.0f))
//	, Up(glm::vec3(0.0f, 1.0f, 0.0f))
//	, Right(1.0f, 0.0f, 0.0f)
//	, WorldUp(0.0f, 1.0f, 0.0f)
//	, Yaw(-90.0f)
//	, Pitch(0.0f)
//	, FOV(45.0f)
//	, CameraSpeed(3.0f)
//	, MouseSensitivity(0.25f)
//	, width(Utils::WIDTH)
//	, height(Utils::HEIGHT)
//	, NearClipping(0.1f)
//	, FarClipping(1000.0f)
//{
//	updateCameraVectors();
//}
//
//Camera::Camera(GLfloat _fov, GLuint _width, GLuint _height, GLfloat zNear, GLfloat zFar)
//{
//	FOV = _fov;
//	width = _width;
//	height = _height;
//	NearClipping = zNear;
//	FarClipping = zFar;
//
//	Position = glm::vec3(0.0f, 0.0f, 10.0f);
//	Front = glm::vec3(0.0f, 0.0f, -1.0f);
//	Up = glm::vec3(0.0f, 1.0f, 0.0f);
//
//	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
//	Yaw = -90.0f;
//	Pitch = 0.0f;
//
//	MouseSensitivity = 0.25f;
//}
//
//Camera::~Camera()
//{
//
//}
//
//glm::mat4 Camera::getprojectionMatrix()
//{
//	return glm::perspective(glm::radians(FOV), (GLfloat)width / (GLfloat)height, NearClipping, FarClipping);
//}
//
//glm::mat4 Camera::getViewMatrix()
//{
//	return glm::lookAt(Position, Position + Front, Up);
//}
//
//void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
//{
//	GLfloat velocity = CameraSpeed * deltaTime;
//	if (direction == FORWARD)
//	{
//		Position += Front * velocity;
//	}		
//	if (direction == BACKWARD)
//	{
//		Position -= Front * velocity;
//	}		
//	if (direction == LEFT)
//	{
//		Position -= Right * velocity;
//	}		
//	if (direction == RIGHT)
//	{
//		Position += Right * velocity;
//	}
//	if (direction == UPWARD)
//	{
//		Position += Up * velocity;
//	}
//	if (direction == DOWNWARD)
//	{
//		Position -= Up * velocity;
//	}
//
//	//glTranslatef(Position.x, Position.y, Position.z);
//
//	//glRotatef(-Pitch, 1.0, 0.0, 0.0);
//	//glRotatef(-Yaw, 0.0, 1.0, 0.0);
//}
//
//void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
//{
//	xoffset *= MouseSensitivity;
//	yoffset *= MouseSensitivity;
//
//	Yaw += xoffset;
//	Pitch += yoffset;
//
//	Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
//
//	// Update Front, Right and Up Vectors using the updated Eular angles
//	//this->updateCameraVectors();
//
//	//glTranslatef(Position.x, Position.y, Position.z);
//
//	//glRotatef(-Pitch, 1.0, 0.0, 0.0);
//	//glRotatef(-Yaw, 0.0, 1.0, 0.0);
//}
//
//// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
////void Camera::ProcessMouseScroll(GLfloat yoffset)
//void Camera::ProcessMouseScroll(int yoffset)
//{
//	if (FOV >= 1.0f && FOV <= 120.0f)
//	{
//		FOV -= yoffset;
//	}
//	FOV = glm::clamp(FOV, 1.0f, 120.0f);
//}
//
//// Calculates the front vector from the Camera's (updated) Eular Angles
//void Camera::updateCameraVectors()
//{
//	// Calculate the new Front vector
//	glm::vec3 look;
//	look.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//	look.y = sin(glm::radians(Pitch));
//	look.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//	Front = glm::normalize(look);
//	// Also re-calculate the Right and Up vector
//	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//	Up = glm::normalize(glm::cross(Right, Front));
//
//}
//
//glm::vec3 Camera::getCameraPosition()
//{
//	return Position;
//}
//
//void Camera::setCameraSpeed(GLfloat _speed)
//{
//	CameraSpeed = _speed;
//}
//
//void Camera::moveForward()
//{
//	Position += Front * CameraSpeed;
//}
//
//void Camera::moveBack()
//{
//	Position -= Front * CameraSpeed;
//}
//
//void Camera::moveLeft()
//{
//	Position -= Right * CameraSpeed;
//}
//
//void Camera::moveRight()
//{
//	Position += Right * CameraSpeed;
//}
//
//void Camera::moveUp()
//{
//	Position += Up * CameraSpeed;
//}
//
//void Camera::moveDown()
//{
//	Position -= Up * CameraSpeed;
//}
//
//void Camera::update()
//{
//	updateCameraVectors();
//}
//
//void Camera::setCameraFront(glm::vec3 _front)
//{
//	Front = _front;
//}
//
//glm::vec3 Camera::getLook()
//{
//	return Front;
//}
//
//void Camera::setPosition(glm::vec3 _position)
//{
//	Position = _position;
//}









