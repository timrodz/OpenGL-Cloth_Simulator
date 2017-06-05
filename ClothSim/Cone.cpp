#include "Cone.h"

CCone::CCone(btSoftRigidDynamicsWorld* _world, GLUquadricObj* _quad)
	: world(_world)
	, quad(_quad)
{

}

CCone::~CCone()
{

}

btRigidBody* CCone::CreateCone(float d, float h, float x, float y, float z, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btConeShape* sphere = new btConeShape(d, h);
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

void CCone::renderCone(btRigidBody* sphere)
{
	if (sphere->getCollisionShape()->getShapeType() != CONE_SHAPE_PROXYTYPE)
		return;
	glColor3f(1, 0, 0);
	float r = ((btConeShape*)sphere->getCollisionShape())->getRadius();
	float h = ((btConeShape*)sphere->getCollisionShape())->getHeight();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat);     //translation,rotation
	glTranslatef(0, h / 2.0, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quad, 0, r, h, 20, 20);
	glPopMatrix();
}
