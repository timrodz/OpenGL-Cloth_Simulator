#include "Plane.h"

CPlane::CPlane(btSoftRigidDynamicsWorld* _world)
	: world(_world)
{

}

CPlane::~CPlane()
{

}

btRigidBody* CPlane::CreatePlane()
{
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, 0, 0));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0), 0); //creates the shape
	btMotionState* motion = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane); //mass = 0 is static, 
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	//bodies.push_back(body);
	//collision
	//bodies.push_back(new bulletObject(body, 4, 0.8, 0.8, 0.8));
	//body->setUserPointer(bodies[bodies.size() - 1]);

	return body;
}

void CPlane::renderPlane(btRigidBody* plane)
{
	if (plane->getCollisionShape()->getShapeType() != STATIC_PLANE_PROXYTYPE)
	{
		return;
	}
	//glColor3f(0.8, 0.8, 0.8);
	glColor3f(0, 0, 0);

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
	glVertex3f(1000, 0, 1000);
	glEnd();
	glPopMatrix();

}