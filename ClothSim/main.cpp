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


//#include "glew/glew.h"
//#include "freeglut/freeglut.h"
//#include "soil/SOIL.h"

//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"

//#include "fmod/fmod.hpp"

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
//Light* triangle;
//GameModel* ground;
//GameModel* cube;
//GameModel* sphere;
//GameModel* quad;

GLuint flatShaderProgram;

GLUquadricObj* quad;
btSoftRigidDynamicsWorld* world;
btDispatcher* dispatcher; //collision algorithm
btCollisionConfiguration* collisionConfig; //deals with dispatcher
btBroadphaseInterface* broadface; //collision algorithm, check if each object collides with you
btConstraintSolver* solver; //calculates everything
btSoftBodySolver* softbodySolver;
std::vector<btRigidBody*> bodies;

void renderPlane(btRigidBody* plane)
{
	if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
	{
		return;
	}
	glColor3f(0.8, 0.8, 0.8);

	btTransform t;
	plane->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat); //translation, rotation
	glBegin(GL_QUADS);
	glVertex3f(-1000, 0, 1000);
	glVertex3f(-1000, 0, -1000);
	glVertex3f(1000, 0, -1000);
	glVertex3f(1000, 0, -000);
	glEnd();
	glPopMatrix();

}

btRigidBody* AddSphere(float rad, float x, float y, float z, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btSphereShape* sphere = new btSphereShape(rad); //creates the shape
	btVector3 inertia(0, 0, 0);
	if (mass != 0)
	{
		sphere->calculateLocalInertia(mass, inertia);
	}
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);

	return body;
}

void renderSphere(btRigidBody* sphere)
{
	if (sphere->getCollisionShape()->getShapeType() != SPHERE_SHAPE_PROXYTYPE)
	{
		return;
	}
	glColor3f(0, 0, 1);

	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat); //translation, rotation
	gluSphere(quad, r, 20, 20);
	//glutSolidSphere(10 - 0.1, 50, 50);
	glPopMatrix();

	//btVector3 position = t.getOrigin();
	//light->setPosition(glm::vec3(position.getX(), position.getY(), position.getZ()));
	//light->setRadius();
}
void renderSoftbody(btSoftBody* b)
{
	//btSoftBodyHelpers::Draw(b, world->getDebugDrawer(), world->getDrawFlags());

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < b->m_faces.size(); i++)
	{
		std::vector<Position> vecPosition;
		for (int j = 0; j < 3; j++)
		{
			//glNormal3f(b->m_faces[i].m_normal[j],
			//	b->m_faces[i].m_normal[j],
			//	b->m_faces[i].m_normal[j]);

			//glNormal3fv(&b->m_faces[i].m_normal[j]);

			glVertex3f(b->m_faces[i].m_n[j]->m_x.x(),
				b->m_faces[i].m_n[j]->m_x.y(),
				b->m_faces[i].m_n[j]->m_x.z());

			Position pos = Position(b->m_faces[i].m_n[j]->m_x.x(), b->m_faces[i].m_n[j]->m_x.y(), b->m_faces[i].m_n[j]->m_x.z());
			vecPosition.push_back(pos);
		}

		//GLuint vao;
		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

		//Cloth* triangles = new Cloth(vecPosition, camera);
		//triangles->setProgram(flatShaderProgram);
		//triangles->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//triangles->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//triangles->update(deltaTime);
		//triangles->render();
	}
	glEnd();
	/*glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < b->m_links.size(); i++)
	{
	for (int j = 0; j < 2; j++)
	{
	glVertex3f(b->m_links[i].m_n[j]->m_x.x(),
	b->m_links[i].m_n[j]->m_x.y(),
	b->m_links[i].m_n[j]->m_x.z());
	}
	}
	glEnd();*/
}


void init()
{
	quad = gluNewQuadric();

	collisionConfig = new btSoftBodyRigidBodyCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadface = new btDbvtBroadphase(); //divides the space into different quads to only check for collisions within each quad
	solver = new btSequentialImpulseConstraintSolver();
	softbodySolver = new btDefaultSoftBodySolver();
	world = new btSoftRigidDynamicsWorld(dispatcher, broadface, solver, collisionConfig, softbodySolver);
	world->setGravity( btVector3(0, -10, 0) );

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0); //creates the shape
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane); //mass = 0 is static, 
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	bodies.push_back(body);
	//bodies.push_back(new bulletObject(body, 4, 0.8, 0.8, 0.8));
	//body->setUserPointer(bodies[bodies.size() - 1]);

	AddSphere(1.0, 0, -10, 0, 1.0);
	float s = 1; //position and size
	float h = 4; //height
	btSoftBody* softBody = btSoftBodyHelpers::CreatePatch(
			world->getWorldInfo(), btVector3(-s, h, -s), btVector3(s, h, -s),
			btVector3(-s, h, s), btVector3(s, h, s), 50, 50, 4 + 8, true);
	softBody->m_cfg.viterations = 10; //increase to 100 for ball not to go through
	softBody->m_cfg.piterations = 10;
	softBody->setTotalMass(3.0);
	//softBody->setMass(100, 100); //make vertex 100 static
	btVector3 wind =  softBody->getWindVelocity();
	softBody->setWindVelocity(btVector3(0.0f, 100.0f, -100.0f));
	softBody->applyForces();
	world->addSoftBody(softBody);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE for wireframe mode
	//glEnable(GL_DEPTH_TEST); // enalbe the depth testing
	//glDepthFunc(GL_LESS);


	//camera = new Camera(90.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 10000.0f);
	camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 100.0f);
	camera->setCameraSpeed(15.0f);
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
}

void updateControls()
{
	//camera controls
	if (keyState[(unsigned char) 'w'] == BUTTON_DOWN) {
		//camera->moveForward();
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

	if (keyState[(unsigned char) 'z'] == BUTTON_DOWN)
	{
		btRigidBody* sphere = AddSphere(1.0, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z, 10.0);
		glm::vec3 look = camera->getLook() * 40.0f;
		sphere->setLinearVelocity(btVector3(look.x, look.y, look.z));
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

	camera->update();
	
	prevTime = currentTime;

	glutPostRedisplay(); // the render function is called
}

void display()
{
	// Clear the screen with the blue color set in the init
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glLoadIdentity;


	for (int i = 0; i < bodies.size(); i++)
	{
		if (bodies[i]->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			//renderPlane(bodies[i]);
		}
		if (bodies[i]->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			renderSphere(bodies[i]);
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
		renderSoftbody(world->getSoftBodyArray()[i]);
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

int main(int argc, char **argv)
{
	// Initialize a window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
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
	

	//glutIdleFunc(update);
	//// register callbacks
	//glutDisplayFunc(render);
	//glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);
	glutPassiveMotionFunc(mouseMove);
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
	delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadface;
	delete softbodySolver;
	delete world;	

	gluDeleteQuadric(quad);

	return 0;

}