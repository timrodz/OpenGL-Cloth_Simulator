#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

// GL Includes
#include "glew/glew.h"

static class CCone
{
public:
	CCone(btSoftRigidDynamicsWorld* _world, GLUquadricObj* _quad);
	~CCone();

	btRigidBody* CreateCone(float d, float h, float x, float y, float z, float mass);

	void renderCone(btRigidBody* cone);

private:

	btSoftRigidDynamicsWorld* world;
	GLUquadricObj* quad;

};