#include "Cloth.h"

#include "Camera.h"

#include "glm/gtc/type_ptr.hpp"

Cloth::Cloth()
	//: speed(1.0f)
{
}

Cloth::Cloth(btSoftRigidDynamicsWorld* _world)
	: world(_world)
{

}

Cloth::~Cloth()
{
}

btSoftBody* Cloth::CreateCloth(int fixed)
{
	//TRACEDEMO
	const btScalar	s = 5;
	//psb->getWorldInfo()->m_gravity.setValue(0,0,0);

	const int		segments = 10;
	const int		count = 5;
	btVector3 pos(-s*segments, 0, 0);
	btScalar gap = 0.5;

	btSoftBody*		body;

	for (int i = 0; i < count; ++i)
	{
		/*btSoftBody*		*/body = btSoftBodyHelpers::CreatePatch(world->getWorldInfo(), btVector3(-s, 0, -s * 3),
			btVector3(+s, 0, -s * 3),
			btVector3(-s, 0, +s),
			btVector3(+s, 0, +s),
			segments, segments * 3,
			1 + 2, true);

		body->getCollisionShape()->setMargin(0.5);
		btSoftBody::Material* pm = body->appendMaterial();
		pm->m_kLST = 0.0004;
		pm->m_flags -= btSoftBody::fMaterial::DebugDraw;
		body->generateBendingConstraints(2, pm);

		body->m_cfg.kLF = 0.05;
		body->m_cfg.kDG = 0.01;

		//psb->m_cfg.kLF			=	0.004;
		//psb->m_cfg.kDG			=	0.0003;

		body->m_cfg.piterations = 2;
		body->m_cfg.aeromodel = btSoftBody::eAeroModel::V_TwoSidedLiftDrag;


		body->setWindVelocity(btVector3(4, -12.0, -25.0));

		btTransform		trs;
		btQuaternion	rot;
		pos += btVector3(s * 2 + gap, 0, 0);
		rot.setRotation(btVector3(1, 0, 0), btScalar(SIMD_PI / 2));
		trs.setIdentity();
		trs.setOrigin(pos);
		trs.setRotation(rot);
		body->transform(trs);
		body->setTotalMass(2.0);



		//this could help performance in some cases
		btSoftBodyHelpers::ReoptimizeLinkOrder(body);

		world->addSoftBody(body);
	}


	//const btScalar s = 4; //size of cloth patch
	//const int NUM_X = 31; //vertices on X axis
	//const int NUM_Z = 31; //vertices on Z axis

	//					  //fixed = 1 + 2;
	//					  //fixed = 0;
	//btSoftBody* body = btSoftBodyHelpers::CreatePatch(world->getWorldInfo(),
	//	btVector3(-s / 2, s + 1, 0),
	//	btVector3(+s / 2, s + 1, 0),
	//	btVector3(-s / 2, s + 1, +s),
	//	btVector3(+s / 2, s + 1, +s),
	//	NUM_X, NUM_Z,
	//	fixed, true);
	////body->getCollisionShape()->setMargin(0.001f);
	//body->generateBendingConstraints(2, body->appendMaterial());
	//body->setTotalMass(10);
	////body->m_cfg.citerations = 10;
	////body->m_cfg.diterations = 10;
	//body->m_cfg.piterations = 5;
	//body->m_cfg.kDP = 0.005f;

	//btVector3 wind = body->getWindVelocity();
	////body->rayTest
	//body->m_cfg.aeromodel = btSoftBody::eAeroModel::V_TwoSidedLiftDrag;
	////body->setWindVelocity(btVector3(4, -12.0, -25.0));
	//body->setWindVelocity(btVector3(0.0f, 0.0f, 1000.0f));
	//body->addAeroForceToFace(btVector3(0.0f, 0.0f, 1000.0f), 20);
	//body->addAeroForceToNode(btVector3(0.0f, 0.0f, 1000.0f), 20);
	//body->applyForces();

	//world->addSoftBody(body);

	//body->randomizeConstraints

	//body->appendAnchor();

	//int fixed = 4 + 8;
	//float s = 1; //position and size
	//float h = 4; //height
	//btSoftBody* body = btSoftBodyHelpers::CreatePatch(
	//	world->getWorldInfo(), btVector3(-s, h, -s), btVector3(s, h, -s),
	//	btVector3(-s, h, s), btVector3(s, h, s), 50, 50, fixed, true);
	//body->m_cfg.viterations = 10; //increase to 100 for ball not to go through
	//body->m_cfg.piterations = 10;
	//body->setTotalMass(3.0);
	////body->setMass(100, 100); //make vertex 100 static
	////btVector3 wind = body->getWindVelocity();
	////body->setWindVelocity(btVector3(0.0f, 100.0f, -100.0f));
	////body->applyForces();
	//world->addSoftBody(body);

	return body;
}


void Cloth::renderSoftbody(btSoftBody* b)
{
	b->addAeroForceToFace(btVector3(0.0f, 0.0f, 1000.0f), 50);
	b->addAeroForceToNode(btVector3(0.0f, 0.0f, 1000.0f), 50);
	b->applyForces();

	//btSoftBodyHelpers::Draw(b, world->getDebugDrawer(), world->getDrawFlags());

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < b->m_faces.size(); i++)
	{
		std::vector<Position> vecPosition;
		for (int j = 0; j < 3; j++)
		{
			//glNormal3f(b->m_faces[i].m_normal[j],
			//	b->m_faces[i].m_normal[j],
			//	b->m_faces[i].m_normal[j]);

			glNormal3fv(&b->m_faces[i].m_normal[j]);

			glVertex3f(b->m_faces[i].m_n[j]->m_x.x(),
				b->m_faces[i].m_n[j]->m_x.y(),
				b->m_faces[i].m_n[j]->m_x.z());

			Position pos = Position(b->m_faces[i].m_n[j]->m_x.x(), b->m_faces[i].m_n[j]->m_x.y(), b->m_faces[i].m_n[j]->m_x.z());
			vecPosition.push_back(pos);
		}

		//GLuint vao;
		//glGenVertexArrays(1, &vao);
		//glBindVertexArray(vao);

		//Cloth* triangles = new Cloth(vecPosition, camera);
		//triangles->setProgram(flatShaderProgram);
		//triangles->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		//triangles->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//triangles->update(deltaTime);
		//triangles->render();
	}
	glEnd();
	/*glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < b->m_links.size(); i++)
	{
	for (int j = 0; j < 2; j++)
	{
	glVertex3f(b->m_links[i].m_n[j]->m_x.x(),
	b->m_links[i].m_n[j]->m_x.y(),
	b->m_links[i].m_n[j]->m_x.z());
	}
	}
	glEnd();*/
}

//Cloth::Cloth(std::vector<Position> _vecPosition, Camera* _camera)
//{
//	camera = _camera;
//
//	scale = glm::vec3(1.0f, 1.0f, 1.0f);
//	position = glm::vec3(0.0, 0.0, 0.0);
//	color = glm::vec3(0.0f, 0.0f, 0.0f);
//	direction = glm::vec3(0.0f, -0.9f, -0.17f);
//
//	std::vector<VertexFormat> vertices;
//
//	vertices.push_back(VertexFormat(_vecPosition[0], TexCoord(1, 1), Normals(0.0f, 0.0f, 1.0)));
//	vertices.push_back(VertexFormat(_vecPosition[1], TexCoord(0, 1), Normals(0.0f, 0.0f, 1.0)));
//	vertices.push_back(VertexFormat(_vecPosition[2], TexCoord(1, 0), Normals(0.0f, 0.0f, 1.0)));
//
//	indices.push_back(0);
//	indices.push_back(1);
//	indices.push_back(2);
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	//glGenBuffers(1, &vbo);
//	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
//
//	//glGenBuffers(1, &ebo);
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
//
//	////Attributes
//	//glEnableVertexAttribArray(0); //position
//	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)0);
//
//	//glEnableVertexAttribArray(1); //texcoord
//	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));
//
//	//glEnableVertexAttribArray(2); //normals
//	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));
//
//	//glBindBuffer(GL_ARRAY_BUFFER, 0);
//	//glBindVertexArray(0);
//}

//void Cloth::update(GLfloat _deltaTime)
//{
//	deltaTime = _deltaTime;
//	model = glm::translate(glm::mat4(), position);
//	model = glm::scale(model, scale);
//}

//void Cloth::render()
//{
//	glUseProgram(program);
//
//	glUniform3f(glGetUniformLocation(program, "lightColor"), color.x, color.y, color.z);
//
//	GLint modelLoc = glGetUniformLocation(program, "model");
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//	//GLint viewLoc = glGetUniformLocation(program, "view");
//	//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
//	//glm::mat4 view = camera->getViewMatrix();
//
//	//GLint projLoc = glGetUniformLocation(program, "projection");
//	//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getprojectionMatrix()));
//	//glm::mat4 projection = camera->getprojectionMatrix();
//
//	glBindVertexArray(vao);
//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//}
//
//glm::vec3 Cloth::getColor()
//{
//	return color;
//}
//
//glm::vec3 Cloth::getPosition()
//{
//	return position;
//}
//
//void Cloth::setProgram(GLuint _program)
//{
//	program = _program;
//}
//
//
//void Cloth::setColor(glm::vec3 _color)
//{
//	color = _color;
//}
//
//void Cloth::setPosition(glm::vec3 _position)
//{
//	position = _position;
//}
//
//void Cloth::setScale(glm::vec3 _scale)
//{
//	scale = _scale;
//}
//
//void Cloth::setSpeed(GLfloat _speed)
//{
//	speed = _speed;
//}
//
//glm::vec3 Cloth::getDirection()
//{
//	return direction;
//}