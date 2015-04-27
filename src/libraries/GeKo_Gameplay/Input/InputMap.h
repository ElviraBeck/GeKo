#pragma once

#include "GeKo_Gameplay/Input/MapType.h"
#include <map>
#include <functional>
#include <GLFW/glfw3.h>
//Includes for different contexts
#include "GeKo_Graphics/Camera/Camera.h"
#include "GeKo_Gameplay/Object/Player.h"

/** InputMap is a superclass of the specific map classes.
To insert an own specific InputMap, you have to add a class containing an update function, which allocates keys or buttons to a method in the map. 
Make sure that your new specific InputMap has a name(m_name) and a boolean variable (m_active = false)
and the filled map(m_map) in its contructor.
In addition you have to adjust the InputHandler by adding the new class instance to the method 'void InputHandler::setAllInputMaps(Camera &activeCam)'.*/

class InputMap
{
public:
	InputMap();
	~InputMap();
	
	/// The method returns the map m_map, in which a key to a function is mapped
	std::map<int, std::function<void()>> getMap();

	/// Enables or disables the map
	/* DO NEVER USE THIS!!
	Please use the method "void changeActiveIputMap(std::string name)" from the class InputHandler to ensure that only one InputMap is active*/
	void setActive(bool a);

	/// The method returns whether the InputMap is active or not
	bool getActive();

	/// The Method names the InputMap
	void setName(std::string name);

	/// The method returns the name of the InputMap
	std::string getName();

	/// The method return the MapType of the InputMap
	MapType getType();
	
	/// The method ensures performing the right action for the mapped keys
	/** It checks if the key already is in the map or not and whether multiple keys are pressed*/
	void checkKeys(int key, GLFWwindow &window);

	/// The method fills the m_map considering the parameter (camera or player)
	// Implemented in the specific Input Maps
	virtual void update(Player &p);
	virtual void update(Camera &cam);

	void setGLFWwindow(GLFWwindow* window);
	GLFWwindow* getWindow();
protected:
	// This map maps a key to a function
	std::map<int, std::function<void()>> m_map;

	// Indicates whether the InputMap is active or inactive
	bool m_active;

	// The naming of the InputMap ensures that a specific InputMap instance can be returned
	std::string m_name;

	GLFWwindow* m_usedWindow;

	MapType m_type;

	std::vector<int> m_mutlipleKeys;

};

