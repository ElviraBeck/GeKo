#include "Scenegraph.h"

Scenegraph::Scenegraph(std::string scenegraphName)
{
	m_scenegraphName = scenegraphName;
	setRootNode(new Node("Root"));
	getRootNode()->setIdentityMatrix_ModelMatrix();
}

Scenegraph::~Scenegraph()
{

}


std::string Scenegraph::getScenegraphName()
{
	return m_scenegraphName;
}

void Scenegraph::setScenegraphName(std::string scenegraphName)
{
	m_scenegraphName = scenegraphName;
}


Node* Scenegraph::getRootNode()
{
	return m_rootNode;
}

void Scenegraph::setRootNode(Node* rootNode)
{
	m_rootNode = rootNode;
}

Camera* Scenegraph::getActiveCamera()
{
	return m_activeCamera;
}

void Scenegraph::setActiveCamera(std::string cameraName)
{
	bool success = false;

	for (int i = 0; i < m_cameraSet.size(); i++)
	{
		if (m_cameraSet.at(i)->getName() == cameraName){
			m_activeCamera = m_cameraSet.at(i);
			success = true;
		}
	}

	if (success)
	{
		std::cout << "SUCCESS: The Camera with the name " << cameraName << " is now active!" << std::endl;
	}
	else
	{
		std::cout << "ERROR: The Camera with the name " << cameraName << " does not exist!" << std::endl;
	}
}

Camera* Scenegraph::getCamera(std::string cameraName)
{
	for (int i = 0; i < m_cameraSet.size(); i++)
	{
		if (m_cameraSet.at(i)->getName() == cameraName)
			return m_cameraSet.at(i);
	}

	std::cout << "ERROR: The Camera with the name " << cameraName << " does not exist!" << std::endl;
	return NULL;
}

bool Scenegraph::containsCamera(std::string cameraName)
{
	for (int i = 0; i < m_cameraSet.size(); i++)
	{
		if (m_cameraSet.at(i)->getName() == cameraName)
			return true;
	}
	return false;
}

void Scenegraph::addCamera(Camera* camera)
{
	m_cameraSet.push_back(camera);
}

Node* Scenegraph::searchNode( std::string name)
{

	for (int i = 0; i < m_rootNode->getChildrenSet()->size(); i++)
	{
		if (m_rootNode->getChildrenSet()->at(i)->getNodeName() == name)
		{
			return m_rootNode->getChildrenSet()->at(i);
		}
	}

	if (m_rootNode->getChildrenSet()->size() > 0)
	{
		return searchNode(m_rootNode->getChildrenSet(), name);
	}
	else
	{
		std::cout << "ERROR: The Node with the name " << name << " does not exist!" << std::endl;
		return NULL;
	}
}

Node* Scenegraph::searchNode(std::vector<Node*>* list, std::string name)
{
	for (int i = 0; i < list->size(); i++)
	{
		if (list->at(i)->getNodeName() == name)
		{
			return list->at(i);
		}
		else if (list->at(i)->getChildrenSet()->size() > 0)
		{
			return searchNode(list->at(i)->getChildrenSet(), name);
		}
	}
}

void Scenegraph::addParticleSystem(ParticleSystem* ps)
{
	m_particleSet.push_back(ps);
}

std::vector<ParticleSystem*>* Scenegraph::getParticleSet()
{
	return &m_particleSet;
}
