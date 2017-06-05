#include "Cube.h"

CCube::CCube(btSoftRigidDynamicsWorld* _world)
	: world(_world)
{

}

CCube::~CCube()
{

}

btRigidBody* CCube::CreateCube(float width, float height, float depth, float x, float y, float z, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btBoxShape* sphere = new btBoxShape(btVector3(width / 2.0, height / 2.0, depth / 2.0));
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		sphere->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	//bodies.push_back(body);

	return body;
}

void CCube::renderCube(btRigidBody* sphere)
{
	if (sphere->getCollisionShape()->getShapeType() != BOX_SHAPE_PROXYTYPE)
		return;
	glColor3f(1, 0, 0);
	btVector3 extent = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	
	glPushMatrix();
	glMultMatrixf(mat);     //translation,rotation

	glBegin(GL_QUADS);
		glVertex3f(-extent.x(), extent.y(), -extent.z());
		glVertex3f(-extent.x(), -extent.y(), -extent.z());
		glVertex3f(-extent.x(), -extent.y(), extent.z());
		glVertex3f(-extent.x(), extent.y(), extent.z());
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(extent.x(), extent.y(), -extent.z());
		glVertex3f(extent.x(), -extent.y(), -extent.z());
		glVertex3f(extent.x(), -extent.y(), extent.z());
		glVertex3f(extent.x(), extent.y(), extent.z());
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(-extent.x(), extent.y(), extent.z());
		glVertex3f(-extent.x(), -extent.y(), extent.z());
		glVertex3f(extent.x(), -extent.y(), extent.z());
		glVertex3f(extent.x(), extent.y(), extent.z());
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(-extent.x(), extent.y(), -extent.z());
		glVertex3f(-extent.x(), -extent.y(), -extent.z());
		glVertex3f(extent.x(), -extent.y(), -extent.z());
		glVertex3f(extent.x(), extent.y(), -extent.z());
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(-extent.x(), extent.y(), -extent.z());
		glVertex3f(-extent.x(), extent.y(), extent.z());
		glVertex3f(extent.x(), extent.y(), extent.z());
		glVertex3f(extent.x(), extent.y(), -extent.z());
	glEnd();

	glBegin(GL_QUADS);
		glVertex3f(-extent.x(), -extent.y(), -extent.z());
		glVertex3f(-extent.x(), -extent.y(), extent.z());
		glVertex3f(extent.x(), -extent.y(), extent.z());
		glVertex3f(extent.x(), -extent.y(), -extent.z());
	glEnd();

	glPopMatrix();
}
