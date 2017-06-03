//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// File Name	: main.cpp
// Description	: main file
// Author		: Charmaine Lim 
// Mail			: charmaine.lim@mediadesign.school.nz
//

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"


#include <iostream>

#include "ShaderLoader.h"
#include "Camera.h"
#include "Light.h"
#include "Cloth.h"
#include "Plane.h"
#include "Sphere.h"


float currentTime;
float prevTime;
float deltaTime;

unsigned char keyState[255];

#define BUTTON_UP		0
#define BUTTON_DOWN		1

#define MOUSE_LEFT 0
#define MOUSE_MIDDLE 1
#define MOUSE_RIGHT 2

unsigned char mouseState[3];

//GLfloat yaw = -90.0f;
//GLfloat pitch = 0.0f;

int lastX = Utils::WIDTH / 2.0;
int lastY = Utils::HEIGHT / 2.0;

bool firstMouse = true;
glm::vec3 rayDirection;
int mouseX;
int mouseY;

Camera* camera;

Light* light;

GLuint flatShaderProgram;

GLUquadricObj* quad;
btSoftRigidDynamicsWorld* world;
btDispatcher* dispatcher; //collision algorithm
btCollisionConfiguration* collisionConfig; //deals with dispatcher
btBroadphaseInterface* broadface; //collision algorithm, check if each object collides with you
btConstraintSolver* solver; //calculates everything
btSoftBodySolver* softbodySolver;
std::vector<btRigidBody*> bodies;

btSoftBodyWorldInfo softBodyWorldInfo;
CPlane* ground;
CSphere* ball;
Cloth* cloth;

void createEmptyDynamicsWorld()
{
	collisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
	dispatcher = new	btCollisionDispatcher(collisionConfig);

	broadface = new btDbvtBroadphase(); //divides the space into different quads to only check for collisions within each quad

	solver = new btSequentialImpulseConstraintSolver;

	softbodySolver = new btDefaultSoftBodySolver();

	//world = new btSoftRigidDynamicsWorld(dispatcher, broadface, solver, collisionConfig);
	world = new btSoftRigidDynamicsWorld(dispatcher, broadface, solver, collisionConfig, softbodySolver);

	world->setGravity(btVector3(0, -10, 0));

	softBodyWorldInfo.m_broadphase = broadface;
	softBodyWorldInfo.m_dispatcher = dispatcher;
	softBodyWorldInfo.m_gravity = world->getGravity();
	softBodyWorldInfo.m_sparsesdf.Initialize();
}

void initPhysics()
{
	createEmptyDynamicsWorld();

	ground = new CPlane(world);
	btRigidBody* groundBody = ground->CreatePlane();
	bodies.push_back(groundBody);

	ball = new CSphere(world, quad);
	//btRigidBody* sphereBody = ball->CreateSphere(0.75, 0, 2, 0, 1.0);
	//bodies.push_back(sphereBody);

	cloth = new Cloth(world);
	btSoftBody* clothBody = cloth->CreateCloth();

	btRigidBody* body = ball->CreateSphere(0.10, -2, 5, 0, 0.0);
	bodies.push_back(body);

	btRigidBody* body1 = ball->CreateSphere(0.10, -1, 5, 0, 1000.0);
	bodies.push_back(body1);
	body1->setGravity(btVector3(0, 0, 0));
	body1->setLinearVelocity(btVector3(-1.0f, 0, 0));

	btRigidBody* body2 = ball->CreateSphere(0.10, 0, 5, 0, 1000.0);
	bodies.push_back(body2);
	body2->setGravity(btVector3(0, 0, 0));
	body2->setLinearVelocity(btVector3(-1.0f, 0, 0));

	btRigidBody* body3 = ball->CreateSphere(0.10, 1, 5, 0, 1000.0);
	bodies.push_back(body3);
	body3->setGravity(btVector3(0, 0, 0));
	body3->setLinearVelocity(btVector3(-1.0f, 0, 0));

	btRigidBody* body4 = ball->CreateSphere(0.10, 2, 5, 0, 1000.0);
	bodies.push_back(body4);
	body4->setGravity(btVector3(0, 0, 0));
	body4->setLinearVelocity(btVector3(-1.0f, 0, 0));


	clothBody->appendAnchor(0, body);
	clothBody->appendAnchor(7, body1);
	clothBody->appendAnchor(14, body2);
	clothBody->appendAnchor(21, body3);
	clothBody->appendAnchor(28, body4);

}

void init()
{
	quad = gluNewQuadric();

	//collisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
	//dispatcher = new btCollisionDispatcher(collisionConfig);
	//broadface = new btDbvtBroadphase(); //divides the space into different quads to only check for collisions within each quad
	//solver = new btSequentialImpulseConstraintSolver();
	//softbodySolver = new btDefaultSoftBodySolver();
	//world = new btSoftRigidDynamicsWorld(dispatcher, broadface, solver, collisionConfig, softbodySolver);
	//world->setGravity( btVector3(0, -10, 0) );

	initPhysics();

	//AddSphere(1.0, 0, -10, 0, 1.0);

	//float s = 1; //position and size
	//float h = 4; //height
	//btSoftBody* softBody = btSoftBodyHelpers::CreatePatch(
	//		world->getWorldInfo(), btVector3(-s, h, -s), btVector3(s, h, -s),
	//		btVector3(-s, h, s), btVector3(s, h, s), 50, 50, 4 + 8, true);
	//softBody->m_cfg.viterations = 10; //increase to 100 for ball not to go through
	//softBody->m_cfg.piterations = 10;
	//softBody->setTotalMass(3.0);
	////softBody->setMass(100, 100); //make vertex 100 static
	//btVector3 wind =  softBody->getWindVelocity();
	//softBody->setWindVelocity(btVector3(0.0f, 100.0f, -100.0f));
	//softBody->applyForces();
	//world->addSoftBody(softBody);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE for wireframe mode
											   //glEnable(GL_DEPTH_TEST); // enalbe the depth testing
											   //glDepthFunc(GL_LESS);


											   //camera = new Camera(90.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 10000.0f);
	camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 100.0f);
	camera->setCameraSpeed(1.0f);
	//camera->setPosition(glm::vec3(0, 0, 100));

	glTranslatef(0, -1, -5);

	//glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90, Utils::WIDTH / Utils::HEIGHT, 0.1f, 1000.0f);
	gluPerspective(90, Utils::WIDTH / Utils::HEIGHT, 1, 1000);
	glMatrixMode(GL_MODELVIEW);

	// Display with depth cues to prevent the sphere or cloth from incorrectly covering eachother
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	// Used to light the bottom back side
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = { 1.0,-0.5,-0.2,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

	// Lights the top left
	glEnable(GL_LIGHT1);
	GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
	GLfloat lightPos1[4] = { -1.0,1.0,0,0.0 };
	GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	//ShaderLoader shader;
	//flatShaderProgram = shader.CreateProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	//////GLuint litTexturedShaderProgram = shader.CreateProgram("Assets/Shaders/LitTexturedModel.vs", "Assets/Shaders/LitTexturedModel.fs");
	////


	//light = new Light(ModelType::kSphere, camera);
	//light->setProgram(flatShaderProgram);
	//light->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	//light->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//light->setSpeed(10.0f);

	//triangle = new Light(ModelType::kTriangle, camera);
	////triangle = new GameModel(ModelType::kTriangle, camera, "Assets/Images/wall.jpg", light, 0.1f, 0.5f);
	//triangle->setProgram(litTexturedShaderProgram);
	//////triangle->setPosition(glm::vec3(2.0f, -2.0f, 0.0f));
	//triangle->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	////triangle->setScale(glm::vec3(3.0f, 3.0f, 3.0f));

	//ground = new GameModel(ModelType::kQuad, camera, "Assets/images/container.jpg", light, 0.1f, 0.5f);
	//ground->setProgram(litTexturedShaderProgram);
	//ground->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	////ground->setPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	//ground->setPosition(glm::vec3(-5.0f, 40.0f, -25.0f));
	//ground->setScale(glm::vec3(2.0f, 2.0f, 1.0f));

	//cube = new GameModel(ModelType::kCube, camera, "Assets/images/Rayman.jpg", light, 0.1f, 0.5f);
	//cube->setProgram(litTexturedShaderProgram);
	//cube->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	////cube->setPosition(glm::vec3(-2.0f, 2.0f, 0.0f));
	//cube->setPosition(glm::vec3(10.0f, 40.0f, -26.0f));
	////cube->setRotation(0.0f);
	//cube->setScale(glm::vec3(2.0f, 2.0f, 1.0f));
}

void updateControls()
{
	//camera controls
	if (keyState[(unsigned char) 'w'] == BUTTON_DOWN) {
		camera->moveForward();
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}if (keyState[(unsigned char) 's'] == BUTTON_DOWN) {
		//camera->moveBack();
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}if (keyState[(unsigned char) 'a'] == BUTTON_DOWN) {
		//camera->moveLeft();
		camera->ProcessKeyboard(LEFT, deltaTime);
	}if (keyState[(unsigned char) 'd'] == BUTTON_DOWN) {
		//camera->moveRight();
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}if (keyState[(unsigned char) 'q'] == BUTTON_DOWN) {
		//camera->moveUp();
		camera->ProcessKeyboard(UPWARD, deltaTime);
	}if (keyState[(unsigned char) 'e'] == BUTTON_DOWN) {
		//camera->moveDown();
		camera->ProcessKeyboard(DOWNWARD, deltaTime);
	}


	/*if (keyState[(unsigned char) 'z'] == BUTTON_DOWN) {
	cube->rotate(glm::vec3(1.0f, 0.0f, 0.0f));
	}if (keyState[(unsigned char) 'x'] == BUTTON_DOWN) {
	cube->rotate(glm::vec3(-1.0f, 0.0f, 0.0f));
	}if (keyState[(unsigned char) 'n'] == BUTTON_DOWN) {
	cube->rotate(glm::vec3(0.0f, 0.0f, 1.0f));
	}if (keyState[(unsigned char) 'm'] == BUTTON_DOWN) {
	cube->rotate(glm::vec3(0.0f, 0.0f, -1.0f));
	}*/
	if (keyState[(unsigned char) 'z'] == BUTTON_DOWN)
	{
		//btRigidBody* sphere = ball->CreateSphere(1.0, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z, 10.0);
		//glm::vec3 look = camera->getLook() * 40.0f;
		//sphere->setLinearVelocity(btVector3(look.x, look.y, look.z));
	}
}

//
void update()
{
	world->stepSimulation(1 / 60.0);

	GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME); // get current time
	currentTime = currentTime / 1000; // convert millisecond to seconds

	GLfloat dt = currentTime - prevTime;
	deltaTime = currentTime - prevTime;

	updateControls();

	//camera->update();
	//light->update(dt);
	/*triangle->update(dt);
	ground->update(dt);
	cube->update(dt);*/

	prevTime = currentTime;

	//	UpdateMousePicking();

	glutPostRedisplay(); // the render function is called
}

void display()
{
	// Clear the screen with the blue color set in the init
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glLoadIdentity;

	//cam.Control();
	//cam.UpdateCamera();

	//light->render();
	/*ground->render();
	triangle->render();

	cube->render();
	sphere->render();*/

	//glDisable(GL_CULL_FACE);

	//glColor3f(1.0f, 1.0f, 0.0f);
	//gluSphere(quad, 1.0, 30, 30);

	//glTranslatef(0.0f, -0.01f, -0.1f);

	for (int i = 0; i < bodies.size(); i++)
	{
		if (bodies[i]->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			ground->renderPlane(bodies[i]);

		}
		if (bodies[i]->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			ball->renderSphere(bodies[i]);
		}
		//else if (bodies[i]->body->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
		//	renderCylinder(bodies[i]);
		//else if (bodies[i]->body->getCollisionShape()->getShapeType() == CONE_SHAPE_PROXYTYPE)
		//	renderCone(bodies[i]);
		//else if (bodies[i]->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
		//	renderBox(bodies[i]);
	}
	for (int i = 0; i < world->getSoftBodyArray().size(); i++)
	{
		cloth->renderSoftbody(world->getSoftBodyArray()[i]);
	}

	glutSwapBuffers();
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_DOWN;
}

void keyboard_up(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_UP;
}

void mouseMove(int xpos, int ypos)
{
	if (firstMouse)
	{
		mouseX = xpos;
		mouseY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - mouseX;
	GLfloat yoffset = mouseY - ypos;

	mouseX = xpos;
	mouseY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void mouseScroll(int button, int state, int x, int y)
{
	// Used for wheels, has to be up
	if (state == GLUT_UP)
	{
		if (button == 3 || button == 4)
		{
			int dir;
			if (button == 3)
			{
				dir = 1;
			}
			else if (button == 4)
			{
				dir = -1;
			}

			const double ZOOM_SENSITIVITY = -3.0;
			//double fov = camera.getFOV() + dir * ZOOM_SENSITIVITY;

			camera->ProcessMouseScroll(dir);
		}
	}
}

void reshape(int w, int h)
{
	// Adjust the viewport if the user resizes the window
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
		gluPerspective(80, (float)w, 1.0, 5000.0);
	else
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void FunctionKeyDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		break;
	case GLUT_KEY_F2:
		break;
	case GLUT_KEY_F3:
		break;
	case GLUT_KEY_F4:
		break;
	case GLUT_KEY_F5:
		break;
	case GLUT_KEY_F6:
		break;
	case GLUT_KEY_F7:
		break;
	case GLUT_KEY_F8:
		break;
	}
}

void FunctionKeyUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		break;
	case GLUT_KEY_F2:
		break;
	case GLUT_KEY_F3:
		break;
	case GLUT_KEY_F4:
		break;
	case GLUT_KEY_F5:
		break;
	case GLUT_KEY_F6:
		break;
	case GLUT_KEY_F7:
		break;
	case GLUT_KEY_F8:
		break;
	}
}

int main(int argc, char **argv)
{
	// Initialize a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	//glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Utils::WIDTH, Utils::HEIGHT);
	glutCreateWindow("Cloth Simulator");

	//init GLEW
	glewInit();

	// Initialize the GL context with predefined values
	init();

	// Link the window to the rest of the code
	glutIdleFunc(update);
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	//glutSpecialFunc(FunctionKeyDown);
	//glutSpecialUpFunc(FunctionKeyUp);


	//glutIdleFunc(update);
	//// register callbacks
	//glutDisplayFunc(render);
	//glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	//glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouseScroll);

	glutMainLoop();

	//btBoxShape* box = new btBoxShape(btVector3(1, 1, 1));

	for (int i = 0; i < bodies.size(); i++)
	{
		world->removeCollisionObject(bodies[i]);
		btMotionState* motionState = bodies[i]->getMotionState();
		btCollisionShape* shape = bodies[i]->getCollisionShape();
		delete bodies[i];
		delete shape;
		delete motionState;
	}

	for (int i = 0; i < world->getSoftBodyArray().size(); i++)
	{
		world->removeSoftBody(world->getSoftBodyArray()[i]);
		delete (world->getSoftBodyArray()[i]);
	}

	delete cloth;
	delete ball;
	delete ground;

	delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadface;
	delete softbodySolver;
	delete world;

	gluDeleteQuadric(quad);

	return 0;

}