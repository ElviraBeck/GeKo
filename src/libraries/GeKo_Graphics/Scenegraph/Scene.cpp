#include "Scene.h"


Scene::Scene(std::string sceneName)
{
	m_sceneName = sceneName;
	m_sceneGraph = new Scenegraph(m_sceneName);
	m_skyboxNode = NULL;
}

bool Scene::hasSkybox()
{
	return m_skyboxNode;
}

Scene::~Scene()
{

}


std::string Scene::getSceneName()
{
	return m_sceneName;
}

void Scene::setSceneName(std::string sceneName)
{
	m_sceneName = sceneName;
}


Scenegraph* Scene::getScenegraph()
{
	return m_sceneGraph;
}

void Scene::setScenegraph(Scenegraph* scenegraph)
{
	m_sceneGraph = scenegraph;
}

Node* Scene::getSkyboxNode()
{
	return m_skyboxNode;
}

void Scene::setSkyboxNode(Node* skyboxNode)
{
	m_skyboxNode = skyboxNode;
}

void Scene::render(ShaderProgram &shader)
{
	m_sceneGraph->getRootNode()->render(shader);

}