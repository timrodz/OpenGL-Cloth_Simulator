#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

// GL Includes
#include "glew/glew.h"

class CPlane
{
public:
	CPlane(btSoftRigidDynamicsWorld* _world);
	~CPlane();

	btRigidBody* CreatePlane();

	void renderPlane(btRigidBody* plane);

private:

	btSoftRigidDynamicsWorld* world;
};


