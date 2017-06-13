#pragma once

#include "Utils.h"

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody\btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody\btDefaultSoftBodySolver.h"
#include "BulletSoftBody\btSoftBodyHelpers.h"
#include "BulletSoftBody\btSoftBodyRigidBodyCollisionConfiguration.h"

class Camera;

class Cloth
{
public:
	Cloth();
	Cloth(btSoftRigidDynamicsWorld* _world);
	//Cloth(std::vector<Position> _vecPosition, Camera* _camera);
	~Cloth();

	//void initPhysics();
	//void renderScene();
	//void createEmptyDynamicsWorld();
	//btSoftRigidDynamicsWorld*	getSoftDynamicsWorld();
	//void createSoftBody(const btScalar size, const int num_x, const int num_z, const int fixed = 1 + 2);

	btSoftBody* FallingCloth(int fixed, int width, int height, btScalar _s);
	btSoftBody* CreateCloth(int fixed, int width, int height, btScalar _s);
	void renderSoftbody(btSoftBody* b);

	btSoftBody* WindyCloth(int fixed);
	void Init_CapsuleCollision();


	/*void setProgram(GLuint _program);
	void setColor(glm::vec3 _color);
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setSpeed(GLfloat _speed);

	void update(GLfloat _deltaTime);
	void render();

	glm::vec3 getColor();
	glm::vec3 getPosition();

	glm::vec3 getDirection();*/

	btSoftBody* softBody;

private:

	//	btSoftBodyWorldInfo softBodyWorldInfo;
	btSoftRigidDynamicsWorld* world;
	

	//Camera* camera;
	//ModelType type;
	//GLuint program;
	//glm::vec3 color;
	//glm::vec3 position;
	//glm::vec3 scale;
	//GLfloat speed;

	//std::vector<VertexFormat> vertices;
	//std::vector<GLuint> indices;
	//GLuint vao;
	//GLuint vbo;
	//GLuint ebo;
	//glm::mat4 model;
	//GLfloat deltaTime;

	//glm::vec3 direction;
	
};