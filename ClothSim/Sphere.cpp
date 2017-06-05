#include "Sphere.h"

CSphere::CSphere(btSoftRigidDynamicsWorld* _world, GLUquadricObj* _quad)
	: world(_world)
	, quad(_quad)
{
}

CSphere::~CSphere()
{

}

btRigidBody* CSphere::CreateSphere(float rad, float x, float y, float z, float mass)
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
	//bodies.push_back(body);

	return body;
}

void CSphere::renderSphere(btRigidBody* sphere)
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