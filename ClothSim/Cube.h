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

	void move(float _fDirection);
	void moveRight();
	void moveLeft();

	void setPosition(int _iPosition);

private:

	btSoftRigidDynamicsWorld* world;

	btRigidBody* m_pRigidbody;

	int m_iPosition = 2;
};