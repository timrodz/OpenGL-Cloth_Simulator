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

#include <iostream>

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

//#include "ShaderLoader.h"
#include "Camera.h"
#include "Cloth.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"
#include "Cone.h"

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

//Light* light;

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
CCylinder* cylinder;
CCone* cone;
CCube* cube;

float gNear = 1.0f;
float gFar = 1000.0f;

btRigidBody* m_pickedBody;
btTypedConstraint* m_pickedConstraint;
int	m_savedState;
btVector3 m_oldPickingPos;
btVector3 m_hitPos;
btScalar m_oldPickingDist;

//set node position directly
/*
btVector3   delta=targetPosition - softDemo->m_node->m_x;
softDemo->m_node->m_x = targetPosition;
softDemo->m_node->m_v+=delta/timeStep;
*/

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
	btRigidBody* sphereBody = ball->CreateSphere(0.5, -1, 1, 2, 1.0);
	bodies.push_back(sphereBody);

	cylinder = new CCylinder(world, quad);
	btRigidBody* cylinderBody = cylinder->CreateCylinder(1, 1, 0, 1, 2, 1.0);
	bodies.push_back(cylinderBody);

	cone = new CCone (world, quad);
	btRigidBody* coneBody = cone->CreateCone(1, 2, -2, 1, 2, 1.0);
	bodies.push_back(coneBody);

	cube = new CCube (world);
	btRigidBody* cubeBody = cube->CreateCube(1, 1, 1, 1, 1, 2, 1.0);
	bodies.push_back(cubeBody);

	cloth = new Cloth(world);
	btSoftBody* clothBody = cloth->CreateCloth(0);

	//Cloth* cloth2 = new Cloth(world);
	//btSoftBody* clothBody2 = cloth2->CreateCloth(0);

	btRigidBody* body = ball->CreateSphere(0.10, -2, 5, 0, 0.0);
	//bodies.push_back(body);

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

	//btSliderConstraint(btRigidBody& rbA,
	//	btRigidBody& rbB,
	//	const btTransform& frameInA,
	//	const btTransform& frameInB,
	//	bool useLinearReferenceFrameA);

}

void init()
{
	quad = gluNewQuadric();

	initPhysics();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE for wireframe mode
											   //glEnable(GL_DEPTH_TEST); // enalbe the depth testing
											   //glDepthFunc(GL_LESS);


	//camera = new Camera(90.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 10000.0f);
	//camera = new Camera(45.0f, Utils::WIDTH, Utils::HEIGHT, 0.1f, 100.0f);
	//camera->setCameraSpeed(1.0f);
	//camera->setPosition(glm::vec3(0, 0, 100));
	camera = new Camera();

	//glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90, Utils::WIDTH / Utils::HEIGHT, 0.1f, 1000.0f);
	gluPerspective(90, Utils::WIDTH / Utils::HEIGHT, gNear, gFar);
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

void reset()
{
	for (int i = 0; i < bodies.size(); i++)
	{
		world->removeCollisionObject(bodies[i]);
		btMotionState* motionState = bodies[i]->getMotionState();
		btCollisionShape* shape = bodies[i]->getCollisionShape();
		delete bodies[i];
		delete shape;
		delete motionState;
	}
	bodies.clear();

}

void updateControls()
{
	//camera controls
	//if (keyState[(unsigned char) 'w'] == BUTTON_DOWN) {
	//	camera->moveForward();
	//	camera->ProcessKeyboard(FORWARD, deltaTime);
	//}if (keyState[(unsigned char) 's'] == BUTTON_DOWN) {
	//	//camera->moveBack();
	//	camera->ProcessKeyboard(BACKWARD, deltaTime);
	//}if (keyState[(unsigned char) 'a'] == BUTTON_DOWN) {
	//	//camera->moveLeft();
	//	camera->ProcessKeyboard(LEFT, deltaTime);
	//}if (keyState[(unsigned char) 'd'] == BUTTON_DOWN) {
	//	//camera->moveRight();
	//	camera->ProcessKeyboard(RIGHT, deltaTime);
	//}if (keyState[(unsigned char) 'q'] == BUTTON_DOWN) {
	//	//camera->moveUp();
	//	camera->ProcessKeyboard(UPWARD, deltaTime);
	//}if (keyState[(unsigned char) 'e'] == BUTTON_DOWN) {
	//	//camera->moveDown();
	//	camera->ProcessKeyboard(DOWNWARD, deltaTime);
	//}


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

	if (keyState[(unsigned char) 'r'] == BUTTON_DOWN) 
	{
		reset();
		init();
		
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

	camera->Control(keyState);
	camera->UpdateCamera();

	//glm::vec3 direction = camera->getVector() * 10000.0f;

	//btVector3 cameraPosition = btVector3(camera->getLocation().x, camera->getLocation().y, camera->getLocation().z);
	//btVector3 cameraLook = btVector3(direction.x, direction.y, direction.z);

	//btCollisionWorld::AllHitsRayResultCallback rayCallback(cameraPosition, cameraLook);
	//
	//world->rayTest(cameraPosition, cameraLook, rayCallback);
	//
	//if (rayCallback.hasHit())
	//{
	//	for (int i = 0; i<rayCallback.m_collisionObjects.size(); i++)
	//	{
	//		//((bulletObject*)(rayCallback.m_collisionObjects[i]->getUserPointer()))->hit = true;
	//		//rayCallback.m_collisionObjects[i]->getUserPointer()

	//		//std::cout << rayCallback.m_hitFractions[i] << std::endl;
	//	}
	//}

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
		else if (bodies[i]->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE)
		{
			cylinder->renderCylinder(bodies[i]);
		}			
		else if (bodies[i]->getCollisionShape()->getShapeType() == CONE_SHAPE_PROXYTYPE)
		{
			cone->renderCone(bodies[i]);
		}		
		else if (bodies[i]->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
		{
			cube->renderCube(bodies[i]);
		}
			
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

	//camera->ProcessMouseMovement(xoffset, yoffset);
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

			//camera->ProcessMouseScroll(dir);
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

bool pickBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
{
	if (world == 0)
		return false;

	btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);
	world->rayTest(rayFromWorld, rayToWorld, rayCallback);
	if (rayCallback.hasHit()) {
		btVector3 pickPos = rayCallback.m_hitPointWorld;

		btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
		//btSoftBody* body = (btSoftBody*)btSoftBody::upcast(rayCallback.m_collisionObject);
		if (body) {
			if (!(body->isStaticObject() || body->isKinematicObject())) {
				m_pickedBody = body;
				m_savedState = m_pickedBody->getActivationState();
				m_pickedBody->setActivationState(DISABLE_DEACTIVATION);

				btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
				btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
				world->addConstraint(p2p, true);
				//btGeneric6DofConstraint* m_pickedConstraint = p2p;
				//btTypedConstraint* m_pickedConstraint = p2p;
				m_pickedConstraint = p2p;
				btScalar mousePickClamping = 30.f;
				p2p->m_setting.m_impulseClamp = mousePickClamping;
				p2p->m_setting.m_tau = 0.001f;
			}
		}
		m_oldPickingPos = rayToWorld;
		m_hitPos = pickPos;
		m_oldPickingDist = (pickPos - rayFromWorld).length();
	}
}

void removePickingConstraint() 
{
	if (m_pickedConstraint) 
	{
		m_pickedBody->forceActivationState(m_savedState);
		m_pickedBody->activate();
		world->removeConstraint(m_pickedConstraint);
		delete m_pickedConstraint;
		m_pickedConstraint = 0;
		m_pickedBody = 0;
	}
}

btVector3 getRayTo(int x, int y)
{
	// calculate the field-of-view
	float tanFov = 1.0f / gNear;
	float fov = btScalar(2.0) * btAtan(tanFov);

	glm::vec3 cameraLoc = camera->getLocation();
	glm::vec3 cameraFront = camera->getVector();
	btVector3 cameraPosition = btVector3(cameraLoc.x, cameraLoc.y, cameraLoc.z);
	btVector3 cameraTarget = btVector3(cameraFront.x, cameraFront.y, cameraFront.z);

	// get a ray pointing forward from the 
	// camera and extend it to the far plane	
	btVector3 rayFrom = cameraPosition;
	btVector3 rayForward = (cameraTarget - cameraPosition);
	rayForward.normalize();
	rayForward *= gFar;

	glm::vec3 cameraUp = camera->getCameraUp();
	// find the horizontal and vertical vectors 
	// relative to the current camera view
	btVector3 ver = btVector3(cameraUp.x, cameraUp.y, cameraUp.z);
	btVector3 hor = rayForward.cross(ver);
	hor.normalize();
	ver = hor.cross(rayForward);
	ver.normalize();
	hor *= 2.f * gFar * tanFov;
	ver *= 2.f * gFar * tanFov;

	// calculate the aspect ratio
	btScalar aspect = Utils::WIDTH / (btScalar)Utils::HEIGHT;

	// adjust the forward-ray based on
	// the X/Y coordinates that were clicked
	hor *= aspect;
	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.f / float(Utils::WIDTH);
	btVector3 dVert = ver * 1.f / float(Utils::HEIGHT);
	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * ver;
	rayTo += btScalar(x) * dHor;
	rayTo -= btScalar(y) * dVert;

	// return the final result
	return rayTo;
}

void CreatePickingConstraint(int x, int y)
{
	//if (!m_pWorld)
	//	return;

	//// perform a raycast and return if it fails
	//RayResult output;
	//if (!Raycast(m_cameraPosition, GetPickingRay(x, y), output))
	//	return;

	//// store the body for future reference
	//m_pPickedBody = output.pBody;

	//// prevent the picked object from falling asleep
	//m_pPickedBody->setActivationState(DISABLE_DEACTIVATION);

	//// get the hit position relative to the body we hit 
	//btVector3 localPivot = m_pPickedBody->getCenterOfMassTransform().inverse()
	//	* output.hitPoint;

	//// create a transform for the pivot point
	//btTransform pivot;
	//pivot.setIdentity();
	//pivot.setOrigin(localPivot);

	//// create our constraint object
	//btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*m_pPickedBody,
	//	pivot, true);
	//bool bLimitAngularMotion = true;
	//if (bLimitAngularMotion)
	//{
	//	dof6->setAngularLowerLimit(btVector3(0, 0, 0));
	//	dof6->setAngularUpperLimit(btVector3(0, 0, 0));
	//}

	//// add the constraint to the world
	//m_pWorld->addConstraint(dof6, true);

	//// store a pointer to our constraint
	//m_pPickConstraint = dof6;

	//// define the 'strength' of our constraint (each axis)
	//float cfm = 0.5f;
	//dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 0);
	//dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 1);
	//dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 2);
	//dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 3);
	//dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 4);
	//dof6->setParam(BT_CONSTRAINT_STOP_CFM, cfm, 5);

	//// define the 'error reduction' of our constraint (each axis)
	//float erp = 0.5f;
	//dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 0);
	//dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 1);
	//dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 2);
	//dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 3);
	//dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 4);
	//dof6->setParam(BT_CONSTRAINT_STOP_ERP, erp, 5);

	//// save this data for future reference
	//m_oldPickingDist = (output.hitPoint - m_cameraPosition).length();
}

void RemovePickingConstraint()
{
	//// exit in erroneous situations
	//if (!m_pPickConstraint || !m_pWorld)
	//	return;

	//// remove the constraint from the world
	//m_pWorld->removeConstraint(m_pPickConstraint);

	//// delete the constraint object
	//delete m_pPickConstraint;

	//// reactivate the body
	//m_pPickedBody->forceActivationState(ACTIVE_TAG);
	//m_pPickedBody->setDeactivationTime(0.f);

	//// clear the pointers
	//m_pPickConstraint = 0;
	//m_pPickedBody = 0;
}

//void BulletOpenGLApplication::CheckForCollisionEvents()
//{
//	// keep a list of the collision pairs we
//	// found during the current update
//	CollisionPairs pairsThisUpdate;
//
//	// iterate through all of the manifolds in the dispatcher
//	for (int i = 0; i < m_pDispatcher->getNumManifolds(); ++i)
//	{
//
//		// get the manifold
//		btPersistentManifold* pManifold =
//			m_pDispatcher->getManifoldByIndexInternal(i);
//
//		// ignore manifolds that have 
//		// no contact points.
//		if (pManifold->getNumContacts() > 0)
//		{
//			// get the two rigid bodies involved in the collision
//			const btRigidBody* pBody0 =
//				static_cast<const btRigidBody*>(pManifold->getBody0());
//			const btRigidBody* pBody1 =
//				static_cast<const btRigidBody*>(pManifold->getBody1());
//
//			// always create the pair in a predictable order
//			// (use the pointer value..)
//			bool const swapped = pBody0 > pBody1;
//			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
//			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;
//
//			// create the pair
//			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);
//
//			// insert the pair into the current list
//			pairsThisUpdate.insert(thisPair);
//
//			// if this pair doesn't exist in the list
//			// from the previous update, it is a new
//			// pair and we must send a collision event
//			if (m_pairsLastUpdate.find(thisPair) == m_pairsLastUpdate.end())
//			{
//				CollisionEvent((btRigidBody*)pBody0, (btRigidBody*)pBody1);
//			}
//		}
//	}
//
//	// create another list for pairs that
//	// were removed this update
//	CollisionPairs removedPairs;
//
//	// this handy function gets the difference beween
//	// two sets. It takes the difference between
//	// collision pairs from the last update, and this 
//	// update and pushes them into the removed pairs list
//	std::set_difference(m_pairsLastUpdate.begin(), m_pairsLastUpdate.end(),
//		pairsThisUpdate.begin(), pairsThisUpdate.end(),
//		std::inserter(removedPairs, removedPairs.begin()));
//
//	// iterate through all of the removed pairs
//	// sending separation events for them
//	for (CollisionPairs::const_iterator iter = removedPairs.begin();
//		iter != removedPairs.end(); ++iter)
//	{
//		SeparationEvent((btRigidBody*)iter->first,
//			(btRigidBody*)iter->second);
//	}
//
//	// in the next iteration we'll want to
//	// compare against the pairs we found
//	// in this iteration
//	m_pairsLastUpdate = pairsThisUpdate;
//}

void mouseButtonCallback(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			btVector3 camPos = btVector3(camera->getLocation().x, camera->getLocation().y, camera->getLocation().z);
			//renderer->getActiveCamera()->getCameraPosition(camPos);

			btVector3 rayFrom = camPos;

			btVector3 rayTo = getRayTo(int(x), int(y));
			pickBody(rayFrom, rayTo);
		}
		if (button == GLUT_RIGHT_BUTTON)
		{
			removePickingConstraint();
		}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			//removePickingConstraint();
			//remove p2p
		}
	}

	//switch (button)
	//{
	//case GLUT_LEFT_BUTTON:  // left mouse button
	//{
	//	if (state == GLUT_DOWN)
	//	{ // button down
	//	  // create the picking constraint when we click the LMB
	//		CreatePickingConstraint(x, y);
	//	}
	//	else
	//	{ // button up
	//	  // remove the picking constraint when we release the LMB
	//		RemovePickingConstraint();
	//	}
	//	break;
	//}
	//case GLUT_RIGHT_BUTTON: // right mouse button
	//{
	//	if (state == GLUT_DOWN)
	//	{ // pressed down
	//	  // shoot a box
	//		//ShootBox(GetPickingRay(x, y));
	//	}

	//	break;
	//}
	//}
}

bool movePickedBody(const btVector3& rayFromWorld, const btVector3& rayToWorld)
{
	if (m_pickedBody  && m_pickedConstraint)
	{
		btPoint2PointConstraint* pickCon = static_cast<btPoint2PointConstraint*>(m_pickedConstraint);
		if (pickCon)
		{
			//keep it at the same picking distance

			btVector3 newPivotB;

			btVector3 dir = rayToWorld - rayFromWorld;
			dir.normalize();
			dir *= m_oldPickingDist;

			newPivotB = rayFromWorld + dir;
			pickCon->setPivotB(newPivotB);
			return true;
		}
	}
	return false;
}

void mouseMoveCallback(int x, int y)
{
	{
		btVector3 rayTo = getRayTo(int(x), int(y));

		btVector3 rayFrom = btVector3(camera->getLocation().x, camera->getLocation().y, camera->getLocation().z);

		movePickedBody(rayFrom, rayTo);
	}

	{
		//if (m_pPickedBody)
		//{
		//	btGeneric6DofConstraint* pickCon =
		//		static_cast<btGeneric6DofConstraint*>(m_pPickConstraint);
		//	if (!pickCon)
		//		return;

		//	// use another picking ray to get the target direction
		//	btVector3 dir = GetPickingRay(x, y) - m_cameraPosition;
		//	dir.normalize();

		//	// use the same distance as when we originally picked the object
		//	dir *= m_oldPickingDist;
		//	btVector3 newPivot = m_cameraPosition + dir;

		//	// set the position of the constraint
		//	pickCon->getFrameOffsetA().setOrigin(newPivot);
		//}
	}

}

bool callbackFunc(btManifoldPoint& cp, const btCollisionObject* obj1, int id1, int index1, const btCollisionObject* obj2, int id2, int index2)
{
	//((bulletObject*)obj1->getUserPointer())->hit = true;

	//((bulletObject*)obj2->getUserPointer())->hit = true;
	return false;
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

	//gContactAddedCallback = callbackFunc;

	//glTranslatef(0, -1, -5);
	camera->setLocation(glm::vec3(0, 1, 5));	//the player will be top of the terrain

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
	
	glutMouseFunc(mouseButtonCallback);
	//glutPassiveMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMoveCallback);
	//glutMouseFunc(mouseScroll);
	

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