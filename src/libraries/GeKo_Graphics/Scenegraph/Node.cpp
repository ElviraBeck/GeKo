#include "Node.h"


Node::Node(std::string nodeName)
{
	m_nodeName = nodeName;
	m_parentNode = nullptr;
	m_modelMatrix = glm::mat4(1.0);
	m_scaleMatrix = glm::mat4(1.0);
	m_rotationMatrix = glm::mat4(1.0);

	m_hasTexture = false;
	m_hasNormalMap = false;
	m_hasHeightMap = false;
	m_hasCamera = false;
	m_hasGeometry = false;
	m_hasBoundingSphere = false;
	m_hasGravity = false;
	m_hasObject = false; 
	m_hasParticleSystem = false;
	m_particleActive = false;

	m_type = ClassType::OBJECT;
}


Node::~Node()
{
	if (m_hasObject && m_type == ClassType::AI)
	{
		m_ai->stopSourcesInMap();
	}

}

std::string Node::getNodeName()
{
	return m_nodeName;
}

void Node::setNodeName(std::string nodeName)
{
	m_nodeName = nodeName;
}

Node* Node::getParentNode()
{
	return m_parentNode;
}

void Node::setParentNode(Node* parentNode)
{
	m_parentNode = parentNode;
}

Node* Node::getChildrenNode(std::string nodeName)
{

	for (int i = 0; i < m_childrenSet.size(); i++)
	{
		if (m_childrenSet.at(i)->getNodeName() == nodeName)
		{
			return m_childrenSet.at(i);
		}
	}
	std::cout << "ERROR: The Node with the name " << nodeName << " does not exist!" << std::endl;
	return NULL;
}

void Node::addChildrenNode(Node* node)
{
	m_childrenSet.push_back(node);
	getChildrenNode(node->getNodeName())->setParentNode(this);
}

void Node::deleteChildrenNode(std::string nodeName)
{
	bool success = false;

	for (int i = 0; i < m_childrenSet.size(); i++)
	{
		if (m_childrenSet.at(i)->getNodeName() == nodeName)
		{
			m_childrenSet.at(i)->~Node();
			m_childrenSet.at(i) = NULL;
			m_childrenSet.erase(m_childrenSet.begin() + i);
			success = true;
		
		}
	}

	if (success)
	{
		std::cout << "SUCESS: The Node with the name " << nodeName << " was deleted!" << std::endl;
	}
	else{
		std::cout << "ERROR: The Node with the name " << nodeName << " was not found!" << std::endl;
	}
}

std::vector<Node*>* Node::getChildrenSet()
{
	return &m_childrenSet;
}

void Node::clearChildrenSet()
{
	m_childrenSet.clear();
}

glm::mat4 Node::getModelMatrix()
{
	return m_modelMatrix;
}

void Node::setModelMatrix(glm::mat4 modelMatrix)
{
	m_modelMatrix = modelMatrix;
}

glm::mat4 Node::getPrevModelMatrix()
{
	return m_PrevModelMatrix;
}

void Node::setPrevModelMatrix(glm::mat4 modelMatrix)
{
	m_PrevModelMatrix = modelMatrix;
}

glm::mat4 Node::getRotationMatrix()
{
	return m_rotationMatrix;
}

void Node::addRotation(float angle, glm::vec3 axis)
{
	glm::mat4 newRotationMatrix = glm::rotate(m_rotationMatrix, angle, axis);
	m_rotationMatrix = newRotationMatrix;

	updateModelMatrix();
}

glm::mat4 Node::getTranslationMatrix()
{
	return m_translateMatrix;
}

void Node::addTranslation(float x, float y, float z)
{
	glm::vec3 transfer = glm::vec3(x, y, z);

	m_translateMatrix = (glm::translate(glm::mat4(1.0), transfer));

	updateModelMatrix();

	for (int i = 0; i < m_boundingList.size(); i++)
	{
		m_boundingList.at(i)->update(m_modelMatrix);
	}

}

void Node::addTranslation(glm::vec3 position)
{
	glm::vec3 transfer = position;

	m_translateMatrix = (glm::translate(glm::mat4(1.0), transfer));

	updateModelMatrix();

	for (int i = 0; i < m_boundingList.size(); i++)
	{
		m_boundingList.at(i)->update(m_modelMatrix);
	}
}

glm::mat4 Node::getScaleMatrix()
{
	return m_scaleMatrix;
}

void Node::addScale(float x, float y, float z)
{
	glm::vec3 scaleVector = glm::vec3(x, y, z);
	m_scaleMatrix = glm::scale(glm::mat4(1.0), scaleVector);

	for (int i = 0; i < m_boundingList.size(); i++)
	{
		m_boundingList.at(i)->radius = m_boundingList.at(i)->radius * x;
	}

	updateModelMatrix();
}

void Node::setIdentityMatrix_Translate()
{
	glm::mat4 inverseMatrix = glm::inverse(m_translateMatrix);
	m_translateMatrix = inverseMatrix * m_translateMatrix;

	updateModelMatrix();
}

void Node::setIdentityMatrix_Scale()
{
	glm::mat4 inverseMatrix = glm::inverse(m_scaleMatrix);
	m_scaleMatrix = inverseMatrix * m_scaleMatrix;

	updateModelMatrix();
}

void Node::setIdentityMatrix_Rotation()
{
	glm::mat4 inverseMatrix = glm::inverse(m_rotationMatrix);
	m_rotationMatrix = inverseMatrix * m_rotationMatrix;

	updateModelMatrix();
}

void Node::setIdentityMatrix_ModelMatrix()
{
	m_modelMatrix = glm::mat4(1);
}

void Node::setIdentityMatrix_PrevModelMatrix()
{
	m_PrevModelMatrix = glm::mat4(1);
}

void Node::updateModelMatrix()
{
	m_modelMatrix = m_translateMatrix *  m_rotationMatrix * m_scaleMatrix  * glm::mat4(1.0);
}

bool Node::hasTexture()
{
	return m_hasTexture;
}

bool Node::hasNormalMap()
{
	return m_hasNormalMap;
}

bool Node::hasCamera()
{
	return m_hasCamera;
}

bool Node::hasGeometry()
{
	return m_hasGeometry;
}

bool Node::hasBoundingSphere()
{
	return m_hasBoundingSphere;
}

bool Node::hasObject()
{
	return m_hasObject;
}

bool Node::hasGravity()
{
	return m_hasGravity;
}

bool Node::hasParticleSystem()
{
	return m_hasParticleSystem;
}

bool Node::isParticleActive(){
	return m_particleActive;
}

void Node::setParticleActive(bool b){
	m_particleActive = b;
}

Geometry* Node::getGeometry()
{
	if (m_hasGeometry)
	{
		return m_geometry;
	}
	else{
		std::cout << "ERROR: The Node has no geometry!" << std::endl;
		return 0;
	}
	;
}

void Node::addGeometry(Geometry* geometry)
{
	m_geometry = geometry;
	m_hasGeometry = true;

	if (!m_geometry->isLoaded())
	{
		m_geometry->loadBufferData();
		m_geometry->setLoaded();
	}

	if (!m_hasBoundingSphere)
	{
		m_hasBoundingSphere = true;
		setBoundingSphere();
	}

}

Texture* Node::getTexture()
{
	if (m_hasTexture)
	{
		return m_texture;
	}
	else
	{
		std::cout << "ERROR: The Node has no texture!" << std::endl;
		return 0;
	}

}

void Node::addTexture(Texture* texture)
{
	m_texture = texture;
	m_hasTexture = true;
}

Texture* Node::getNormalMap()
{
	if (m_hasNormalMap)
	{
		return m_normalmap;
	}
	else
	{
		std::cout << "ERROR: The Node has no normal-map!" << std::endl;
		return 0;
	}
}

void Node::addNormalMap(Texture* normalmap)
{
	m_normalmap = normalmap;
	m_hasNormalMap = true;
}

Texture* Node::getHeightMap()
{
	return m_heightmap;
}

void Node::addHeightMap(Texture* heightmap, float heightScale, float heightBias, bool useHeightmapShadows)
{
	m_heightScale = heightScale;
	m_heightBias = heightBias;
	m_heightmap = heightmap;
	m_useHeightMapShadows = useHeightmapShadows;
	m_hasHeightMap = true;
}

Camera* Node::getCamera()
{
	if (m_hasCamera)
	{
		return m_camera;
	}
	else
	{
		std::cout << "ERROR: The Node has no camera!" << std::endl;
		return 0;
	}
}

void Node::setCamera(Camera* camera)
{
	m_camera = camera;
	m_hasCamera = true;

	setCameraToPlayer();
}

void Node::setCameraToPlayer(){
	if (m_hasObject)
	{
		if (m_type == ClassType::PLAYER)
		{
			glm::vec3 camPosition;
			camPosition = glm::vec3(m_player->getPosition() + (m_player->getViewDirection()*glm::vec4(-5.0, -5.0, -5.0, 1.0)));
			camPosition.y += 3.0;
			m_camera->setPosition(glm::vec4(camPosition, 1.0));
			m_camera->setLookAt(glm::vec3(m_player->getPosition() + m_player->getViewDirection()));
		}
	}
}



BoundingSphere* Node::getBoundingSphere()
{
	if (m_hasGeometry)
	{
		return m_sphere;
	}
	else
	{
		std::cout << "ERROR: The Node has no bounding-sphere!" << std::endl;
		return 0;
	}
}

std::vector<BoundingSphere*>* Node::getBoundingList()
{
	return &m_boundingList;
}

void Node::setBoundingSphere()
{
	if (m_hasGeometry)
	{
		m_sphere = new BoundingSphere(getGeometry(), glm::mat4(1.0));
		m_boundingList.push_back(m_sphere);
		//std::cout << "SUCCESS: A Bounding Sphere was created!" << std::endl;
	}
	else
	{
		std::cout << "ERROR: No geometry was set, yet. Cannot create a Bounding-Sphere!" << std::endl;
	}

}

void Node::setBoundingSphere(double radius, glm::vec3 center)
{
	m_sphere = new BoundingSphere(radius, center);
	m_boundingList.push_back(m_sphere);
	//std::cout << "SUCCESS: A Bounding Sphere was created!" << std::endl;
}


AI* Node::getAI()
{
	if (m_hasObject && m_type == ClassType::AI)
	{
		return m_ai;
	}
	else
	{
		std::cout << "ERROR: The Node is no AI-Unit!" << std::endl;
		return 0;
	}

}

void Node::setObject(AI* object)
{
	object->setNodeName(m_nodeName);

	m_type = object->getClassType();
	m_ai = object;
	m_hasObject = true;

	m_viewArea = new BoundingSphere(m_sphere->radius, m_sphere->center);
	m_viewArea->center += glm::vec3(object->getViewDirection()) * glm::vec3(m_sphere->radius, m_sphere->radius, m_sphere->radius);
	m_boundingList.push_back(m_viewArea);

	addTranslation(glm::vec3(object->getPosition()));
}

Player* Node::getPlayer()
{
	if (m_hasObject && (m_type == ClassType::PLAYER))
	{
		return m_player;
	}
	else
	{
		std::cout << "ERROR: The Node is no Player-Unit!" << std::endl;
		return 0;
	}
}

void Node::setObject(Player* object)
{
	object->setNodeName(m_nodeName);

	m_type = object->getClassType();
	m_player = object;
	m_hasObject = true;

	if (m_hasCamera)
	{
		m_player->setPosition(glm::vec4(m_camera->getCenter().x, 0.0f, m_camera->getCenter().z, 1.0));
	}

	addTranslation(glm::vec3(m_player->getPosition()));
}


StaticObject* Node::getStaticObject()
{
	if (m_hasObject && m_type == ClassType::STATIC)
	{
		return m_staticObject;
	}
	else
	{
		std::cout << "ERROR: The Node is no Static-Unit!" << std::endl;
		return 0;
	}
}

void Node::setObject(StaticObject* object)
{
	object->setNodeName(m_nodeName);

	m_type = object->getClassType();
	m_hasObject = true;
	m_staticObject = object;

	addTranslation(glm::vec3(object->getPosition()));
}

ClassType Node::getType()
{
	if (m_hasObject)
	{
		return m_type;
	}
	else
	{
		std::cout << "ERROR: The Node is no Object-Unit and has no specific ClassType!" << std::endl;
		return ClassType::OBJECT;
	}
}

ParticleSystem* Node::getParticleSystem()
{
	if (m_hasParticleSystem)
	{
		return m_particleSystem;
	}
	else
	{
		std::cout << "ERROR: The Node has no Particle-System attached!" << std::endl;
		return 0;
	}
}

void Node::addParticleSystem(ParticleSystem* particleSystem)
{
	m_particleSystem = particleSystem;
	m_hasParticleSystem = true;
}

void Node::addGravity(Gravity* gravity)
{
	m_hasGravity = true;
	m_Gravity = gravity;
}

void Node::setGravity(bool grav)
{
	m_hasGravity = grav;
}

void Node::render()
{
  if (hasGeometry())
  {
    m_geometry->renderGeometry();
  }

  for (int i = 0; i < m_childrenSet.size(); i++)
  {
    m_childrenSet.at(i)->render();
  }
}

void Node::render(ShaderProgram &shader)
{

	if (!m_hasParticleSystem)
	{
		if (!(m_nodeName == "Root"))
		{
			glm::mat4 modelMatrix(1.0);

			if (m_hasGravity){
				if ((m_type == ClassType::PLAYER | m_type == ClassType::PLAYER) & m_type != ClassType::OBJECT)
				{
					m_player->setPosition(glm::vec4(glm::vec3(m_player->getPosition()) + m_Gravity->getGravity(), 1.0));
					addTranslation(glm::vec3(m_player->getPosition()));
					if (m_hasCamera)
					{
						setCameraToPlayer();
					}
				}
				else if ((m_type == ClassType::AI))
				{
					m_ai->setPosition(glm::vec4(glm::vec3(m_ai->getPosition()) + m_Gravity->getGravity(), 1.0));
					addTranslation(glm::vec3(m_ai->getPosition()));
				}
				else if (m_type == ClassType::OBJECT)
				{
					m_modelMatrix = m_Gravity->addGravity(m_modelMatrix);
				}
			}

			if ((m_type == ClassType::PLAYER) & m_type != ClassType::OBJECT)
			{
				if (m_hasCamera)
				{
					//addRotation(-(m_camera->getRotationAngle()), glm::vec3(0.0, 1.0, 0.0));
					//m_player->rotateView((m_camera->getRotationAngle()), 0.0f);

					/*glm::vec3 camPosition;
					camPosition = glm::vec3(m_player->getPosition() + (m_player->getViewDirection()*glm::vec4(-5.0)));
					camPosition.y += 3.0;
					m_camera->setPosition(glm::vec4(camPosition, 1.0));
					m_camera->setLookAt(glm::vec3(m_player->getPosition() + m_player->getViewDirection()));*/
				}
			}
			if(m_parentNode)
				modelMatrix = getParentNode()->getModelMatrix() * m_modelMatrix;
			else
				modelMatrix = m_modelMatrix;
			shader.sendMat4("modelMatrix", modelMatrix);
			shader.sendMat4("previousModelMatrix", m_PrevModelMatrix);
			m_PrevModelMatrix = modelMatrix;
		}

		if (m_hasTexture)
		{
			shader.sendSampler2D("testTexture", getTexture()->getTexture(), 0);
			shader.sendInt("useTexture", 1);
		}
		else
			shader.sendInt("useTexture", 0);

		if (m_hasNormalMap)
		{
			shader.sendSampler2D("normalMap", getNormalMap()->getTexture(), 1);
			shader.sendInt("useNormalMap", 1);
		}
		else
			shader.sendInt("useNormalMap", 0);

		if (m_hasHeightMap)
		{
			shader.sendSampler2D("heightMap", getHeightMap()->getTexture(), 2);
			shader.sendInt("useHeightMap", 1);
			shader.sendFloat("parallaxScale", m_heightScale);
			shader.sendFloat("parallaxBias", m_heightBias);

			if (m_useHeightMapShadows)
				shader.sendInt("useHeightMapShadows", 1);
			else
				shader.sendInt("useHeightMapShadows", 0);
		}

		else
		{
			shader.sendInt("useHeightMap", 0);
			shader.sendInt("useHeightMapShadows", 0);
		}


		if (m_hasGeometry)
		{
			m_geometry->renderGeometry();
		}

		for (int i = 0; i < m_childrenSet.size(); i++)
		{
			m_childrenSet.at(i)->render(shader);
		}

	}
	else {
		//Extra Render-Methode für ein Partikel-System
		if (m_particleActive){
			renderParticles();
		}
	}
}

void Node::renderParticles()

{
	m_particleSystem->update(*m_camera);
	m_particleSystem->render(*m_camera);
}