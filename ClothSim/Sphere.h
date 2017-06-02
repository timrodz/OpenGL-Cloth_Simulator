#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

// GL Includes
#include "glew/glew.h"

class CSphere
{
public:
	CSphere(btSoftRigidDynamicsWorld* _world, GLUquadricObj* _quad);
	~CSphere();

	btRigidBody* CreateSphere(float rad, float x, float y, float z, float mass);

	void renderSphere(btRigidBody* sphere);

private:

	btSoftRigidDynamicsWorld* world;
	GLUquadricObj* quad;
};