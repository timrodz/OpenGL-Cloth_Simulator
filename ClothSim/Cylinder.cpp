#include "Cylinder.h"

CCylinder::CCylinder(btSoftRigidDynamicsWorld* _world, GLUquadricObj* _quad)
	: world(_world)
	, quad(_quad)
{

}

CCylinder::~CCylinder()
{

}


btRigidBody* CCylinder::CreateCylinder(float d, float h, float x, float y, float z, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btCylinderShape* sphere = new btCylinderShape(btVector3(d / 2.0, h / 2.0, d / 2.0));
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

void CCylinder::renderCylinder(btRigidBody* sphere)
{
	if (sphere->getCollisionShape()->getShapeType() != CYLINDER_SHAPE_PROXYTYPE)
		return;
	glColor3f(1, 0, 0);
	btVector3 extent = ((btCylinderShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat);     //translation,rotation
	glTranslatef(0, extent.y(), 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quad, extent.x(), extent.x(), extent.y()*2.0, 20, 20);
	glPopMatrix();
}
