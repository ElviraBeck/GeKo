#include "Scene.h"

Scene::Scene()
{

}

Scene::Scene(std::string sceneName)
{
	m_sceneName = sceneName;
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
	return &m_sceneGraph;
}

void Scene::setScenegraph(Scenegraph scenegraph)
{
	m_sceneGraph = scenegraph;
}


