#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

// GL Includes
#include "glew/glew.h"

class CCylinder
{
public:
	CCylinder(btSoftRigidDynamicsWorld* _world, GLUquadricObj* _quad);
	~CCylinder();

	btRigidBody* CreateCylinder(float d, float h, float x, float y, float z, float mass);

	void renderCylinder(btRigidBody* cylinder);

private:

	btSoftRigidDynamicsWorld* world;
	GLUquadricObj* quad;
};
