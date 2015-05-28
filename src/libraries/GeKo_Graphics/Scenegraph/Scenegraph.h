#pragma once
#include <GeKo_Graphics/Scenegraph/Node.h>
#include <algorithm>

///Scenegraph contains Node
/**Every scenegraph is connected with one scene and its name is the same as the scenes name it belongs to.
The scenegraph is managing all the objects in the scene which are added to the scenegraph by adding them to the scene.
Each scenegraph provides a rootnode from the beginning*/
class Scenegraph
{
public:
	
	///A scenegraph just gets a Name
	/**The m_scenegraphName will be set and the m_rootNode will be set automatically too*/
	Scenegraph(std::string scenegraphName);
	~Scenegraph();

	
	///Returns m_scenegraphName as a string
	/**/
	std::string getScenegraphName();
	void setScenegraphName(std::string scenegraphName);
	
	/// Returns the m_rootNode as a Node object
	/**/
	Node* getRootNode();
	void setRootNode(Node* rootNode);

	///Returns the node which is asked for
	/**Each node can be found by its unique name*/
	Node* searchNode(std::string name);
	///This Method uses a list of Nodes to find a node
	/**Will be used by the first method searchNode for recursion*/
	Node* searchNode(std::vector<Node*>* list, std::string name);

	///Returns the m_activeCamera Camera-Object
	/**/
	Camera* getActiveCamera();
	void setActiveCamera(std::string cameraName);

	///Returns a Camera-Object
	/**The Camera Name must be known for finding the Camera Object. If there is no such Camera with this name
		an error will be placed and NULL returns*/
	Camera* getCamera(std::string cameraName);
	bool containsCamera(std::string cameraName);
	void addCamera(Camera* camera);

	void addParticleSystem(ParticleSystem* ps);
	std::vector<ParticleSystem*>* getParticleSet();
	void sortParticleSet(std::vector<int> &rvec);

protected:

	std::string m_scenegraphName;
	Node* m_rootNode;
	
	Camera* m_activeCamera;
	std::vector<Camera*> m_cameraSet;

	std::vector<ParticleSystem*> m_particleSet;
};