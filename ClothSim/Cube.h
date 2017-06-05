#pragma once

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

#include "glew/glew.h"

class CCube
{
public:
	CCube(btSoftRigidDynamicsWorld* _world);
	~CCube();

	btRigidBody* CreateCube(float width, float height, float depth, float x, float y, float z, float mass);

	void renderCube(btRigidBody* cube);

private:

	btSoftRigidDynamicsWorld* world;
};