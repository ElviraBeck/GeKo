#pragma once

#include <vector>
#include <glm/ext.hpp>
#include <iostream>

#include <GeKo_Graphics/Shader/Shader.h>
#include <GeKo_Graphics/Geometry/Geometry.h>
#include <GeKo_Graphics/Material/Texture.h>

#include <GeKo_Physics/Gravity.h>

#include <GeKo_Graphics/ParticleSystem/ParticleSystem.h>

#include <GeKo_Gameplay/Object/AI.h>
#include <GeKo_Gameplay/Object/Player.h>
#include <GeKo_Gameplay/Object/StaticObject.h>

#include <GeKo_Sound/SoundFileHandler.h>

///A Node contains information, which can be rendered in the world
/**A "Node" should be a container for Geometry, Material, Lights, Cameras, KI and Player and provides all the information a shader could need
  like a Modelmatrix for example. It has one parent and can have a lot of children or none. Every Node exists as long as the scenegraph */
class Node
{
public:

	///The constructor for a Node
	/**At the beginning a Node just needs a name, the m_modelMatrix will be set to the identity matrix*/
	Node(std::string nodeName);
	~Node();

//==================Essential Node Functions===========================//

	///Returns the m_name of the Node as a string
	/**/
	std::string getNodeName();
	void setNodeName(std::string nodeName);

	///Returns the m_Node Object which represents the Parent Node
	/**/
	Node* getParentNode();
	void setParentNode(Node* parentNode);

	///Returns a Node-Object with the name nodeName
	/**This Method iterates over the m_childrenSet and returns the Node with the nodeName, but just the direct children!*/
	Node* getChildrenNode(std::string nodeName);

	///A Node-Object will be added to m_childrenSet
	/**The Parent Node will be set automatically to the node which calls the function!*/
	void addChildrenNode(Node* childrenNode);

	///This method deletes a Node-Object in m_childrenSet
	/**The user gives the method a name and the method iterates over the m_childrenSet
	and deletes the Child with the nodeName. It just deletes children and not grand- or great-grand-children of the node!*/
	void deleteChildrenNode(std::string nodeName);

	///Returns the m_childrenSet of the Node
	/**This returns the whole Set of children of the node!*/
	std::vector<Node*>* getChildrenSet();

	///A method to clear m_childrenSet
	/**Every Child Node in m_childrenSet will be deleted!*/
	void clearChildrenSet();

//==================Modelmatrix Functions===========================//

	///Returns the m_modelMatrix as a mat4
	/**In this Modelmatrix all the rotations, scales and transformations of the node are saved!*/
	glm::mat4 getModelMatrix();
	///With this method a completly new modelmatrix will be set
	/**Should not be available for the user! Changes could be done with the add-methods for modelmatrix!*/
	void setModelMatrix(glm::mat4 modelMatrix);

	glm::mat4 getPrevModelMatrix();
	void setPrevModelMatrix(glm::mat4);

	///Returns m_rotationMatrix as a mat4
	/**This matrix includes the last rotation which was used for the Node!*/
	glm::mat4 getRotationMatrix();
	///A rotation will be added to the Modelmatrix
	/**The user tells the method how big the angle should be and around which axis we rotate.
	Then the method calls the rotate(...) Method of opengl and updates the modelmatrix itself!
	This method should only be used once, because every time it is called, it overwrites the rotation from before!*/
	void addRotation(float angle, glm::vec3 axis);

	///Returns m_translateMatrix as a mat4
	/**This matrix includes the last translation which was used for the Node!*/
	glm::mat4 getTranslationMatrix();
	///A translation will be added to the Modelmatrix
	/**The user tells the method on which position an object should be!
	Then the method calls the translate(...) Method of opengl and updates the modelmatrix!
	This method overwrites all translations from before and changes the modelmatrix completely to its new position value!
	Furthermore if the node has an bounding-sphere, the center of it will be updated as well!*/
	void addTranslation(float x, float y, float z);
	void addTranslation(glm::vec3 position);

	///Returns m_scaleMatrix as a mat4
	/**This matrix includes the last scaling which was used for the Node!*/
	glm::mat4 getScaleMatrix();
	///A scale will be added to the Modelmatrix
	/**The user tells the method in which directions (x, y and/or z) the object should be scaled.
	Then the method calls the scale(...) Method of opengl and updates the modelmatrix.
	This method overwrites all scalings from before!
	Furthermore if the node has an bounding-sphere, its radius will be updated as well!*/
	void addScale(float x, float y, float z);

	///The Translationmatrix will be set to the Identity matrix
	/**The Translationsmatrix will be replaced and the Modelmatrix will be updated!*/
	void setIdentityMatrix_Translate();
	///The same as setIdentityMatrix_Translate() just for m_scaleMatrix
	/**/
	void setIdentityMatrix_Scale();
	///The same as setIdentityMatrix_Translate() just for m_rotationMatrix
	/**/
	void setIdentityMatrix_Rotation();
	///The same as setIdentityMatrix_Translate() just for m_modelMatrix
	/**/
	void setIdentityMatrix_ModelMatrix();

	void setIdentityMatrix_PrevModelMatrix();

//==================Boolean Functions===========================//

	///Returns true, if a texture unit has been attached
	bool hasTexture();
	///Returns true, if a normal-map unit has been attached
	bool hasNormalMap();
	///Returns true, if a camera has been attached
	bool hasCamera();
	///Returns true, if a geometry has been attached
	bool hasGeometry();
	///Returns true, if a boundings-sphere has been attached
	bool hasBoundingSphere();
	///Returns true, if a object(Player, Static, AI) has been attached
	bool hasObject();
	///Returns true, if a gravity unit has been attached
	bool hasGravity();
	///Returns true, if a partice system has been attached
	bool hasParticleSystem();
	///Return true, if the particle should be rendered
	bool isParticleActive();
	///Set the m_particleActive to true or false
	/**Should be used to activate or deactivate the render loop for particles*/
	void setParticleActive(bool b);

//==================Functions to add Units to the Node===========================//

	///Returns m_geometry as a Geometry object
	/**If the node does not have a geometry an error will be thrown!*/
	Geometry* getGeometry();
	///A Geometry Object will be linked to the node
	/**A geometry Object will be saved as m_geometry. If the geometry was not loaded yet, the buffers will be loaded with the 
	geometry information like vertices, normals etc.!*/
	void addGeometry(Geometry* geometry);

	///Returns m_texture as a Texture object
	/**If the node does not have a texture an error will be thrown!*/
	Texture* getTexture();
	void addTexture(Texture* texture);

	///Returns m_normalmap as a Texture object
	/**If the node does not have a normal-map an error will be thrown!*/
	Texture* getNormalMap();
	void addNormalMap(Texture* normalmap);

	///Returns m_heightmap as a Texture object
	/**/
	Texture* getHeightMap();
	void addHeightMap(Texture* heightmap, float heightScale = 0.07f, float heightBias = 0.1f, bool useHeightmapShadows = false);

	///Returns m_Camera as a Camera Object
	/**If the node does not have a camera an error will be thrown!*/
	Camera* getCamera();
	void setCamera(Camera* camera);

	///Updates the camera in relation to the player
	/**The cameraposition and its lookat are updated in relation to the player position and viewdirection */
	void setCameraToPlayer();

	///Return m_boundingSphere of the Node
	/**If the node does not have a bounding-sphere an error will be thrown!*/
	BoundingSphere* getBoundingSphere();
	///Returns the list of bounding-spheres of the node, m_boundingList
	/**/
	std::vector<BoundingSphere*>* getBoundingList();

	///A new Bounding-Sphere object will be created
	/**This method takes the geometry m_geometry and the m_modelMatrix and creates a new BoundingSphere object.
	This will be added to the list of bounding-spheres automatically!*/
	void setBoundingSphere();
	///A new Bounding-Sphere object will be created
	/**The user can choose which parameters the bounding sphere should have (radius and center)! 
	No Geometry is needed!*/
	void setBoundingSphere(double radius, glm::vec3 center);

	///Returns the m_ai object 
	/**If the node does not have a AI-unit an error will be thrown!*/
	AI* getAI();
	///A AI-Unit will be set and the node will be handled as a AI from now on!
	/**The AI-Object will know the name of the node it is attached to! Furthermore a viewArea-Bounding-Sphere will be 
	created automatically and will be pushed into the bounding-sphere list!
	Furthermore the geometry will be placed at the position of the AI-Object!*/
	void setObject(AI* object);
	
	///Returns the m_player object 
	/**If the node does not have a Player-unit an error will be thrown!*/
	Player* getPlayer();
	///A Player-Unit will be set and the node will be handled as a Player from now on!
	/**The Player-Object will know the name of the node it is attached to!
	Furthermore the geometry will be placed at the position of the Player-Object!*/
	void setObject(Player* object);

	///Returns the m_staticObject object 
	/**If the node does not have a Static-unit an error will be thrown!*/
	StaticObject* getStaticObject();
	///A Static-Unit will be set and the node will be handled as a Static from now on!
	/**The Static-Object will know the name of the node it is attached to!
	Furthermore the geometry will be placed at the position of the Static-Object!*/
	void setObject(StaticObject* object);

	///Returns the m_type of the Node
	/**The m_type represents a ClassType which will be set when adding a Player, AI or Static-Object to a Node. 
	It is helpfull to determine which type of Object the Node is!
	Returns a OBJECT ClassType if no Player, AI or Static object was added!*/
	ClassType getType();
	
	///Returns the m_particleSystem object 
	/**If the node does not have a Particle-unit an error will be thrown!*/
	ParticleSystem* getParticleSystem();
	///Adds a Particle-System to the node
	/**The Node will not render geometry or other things. It will be only functional as a source for a particle system!*/
	void addParticleSystem(ParticleSystem* particleSystem);


	///Adds a Gravity-Module to the node
	/**The Node will react to gravitational effects from now on!*/
	void addGravity(Gravity* gravity);
	///Sets the gravity-effect to on or off
	/**Even with a gravity module it is possible to switch the gravity on and off as pleased!*/
	void setGravity(bool grav);

	
//==================Render functions===========================//
	///A method to tell the Node to draw itself
	/**The Node will take this call and forward it to the geometry, so the geometry will be drawn*/
	void render();

	///A method to tell the Node to draw itself
	/**The Node will take this call and forward it to the geometry, so the geometry will be drawn*/
	void render(ShaderProgram &shader);

	///A method to tell the Node to render its Particle-System
	/**This Method will be used by the Node if a Particle system was attached to it, only!*/
	void renderParticles();

protected:
	std::string m_nodeName;

	Node* m_parentNode;
	std::vector<Node*> m_childrenSet;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_PrevModelMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_scaleMatrix;
	glm::mat4 m_translateMatrix;

	bool m_hasTexture;
	bool m_hasNormalMap;
	bool m_hasHeightMap;
	bool m_useHeightMapShadows;
	bool m_hasCamera;
	bool m_hasGeometry;
	bool m_hasBoundingSphere;
	bool m_hasObject;
	bool m_hasGravity;
	bool m_hasParticleSystem;
	bool m_particleActive;

	Geometry* m_geometry;
	Texture* m_texture;
	Texture* m_normalmap;
	Texture* m_heightmap;
	float m_heightScale;
	float m_heightBias;
	Camera* m_camera;
	//Camera* m_otherCamera;

	BoundingSphere* m_sphere;
	BoundingSphere* m_viewArea;
	std::vector<BoundingSphere*> m_boundingList;

	AI* m_ai;
	Player* m_player;
	StaticObject* m_staticObject;
	ClassType m_type;

	ParticleSystem* m_particleSystem;

	Gravity* m_Gravity;

private:
	///A method which updates the Modelmatrix
	/**When a new rotation, scale or translation is added to the object, then the modelMatrix needs an update.
	This update will be done by this method, all the single matrices (scale, rotation, translation) will be computed.
	The order of the update will be: translation * rotation * scale!*/
	void updateModelMatrix();
};
