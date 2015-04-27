#pragma once

#include <glm/ext.hpp>
#include <GeKo_Graphics/Scenegraph/Scenegraph.h>
#include <GeKo_Graphics/Shader/Shader.h>

/// A Scene is needed for a Scenegraph
/*The Scene class contains a scenegraph and represents a piece of a level or a whole level. 
In this class all the objects are contained, cameras and lights included. */ 
class Scene
{
public:

	///A scene just gets a name
	/**The m_sceneName will be set and a scenegraph object m_sceneGraph will be provided automatically*/
	Scene(std::string sceneName);
	~Scene();

	///Returns the string m_sceneName
	/**/
	std::string getSceneName();
	void setSceneName(std::string sceneName);

	///Returns the Scenegraph object m_sceneGraph
	/**/
	Scenegraph* getScenegraph();
	void setScenegraph(Scenegraph* scenegraph);
	Node* getSkyboxNode();
	void setSkyboxNode(Node* skyboxNode);
	bool hasSkybox();

	///The render call which will be forwarded to the scenegraph object of the scene
	/**Each Render call needs a shader-Unit, with which the rendering progress will be startet*/
	void render(ShaderProgram &shader);

protected:
	std::string m_sceneName;
	Node* m_skyboxNode;
	Scenegraph* m_sceneGraph;
};