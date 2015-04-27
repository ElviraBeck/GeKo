#include "InputMap.h"

InputMap::InputMap()
{
	m_active = false;
	m_type = MapType::DEFAULTMAP;
}

InputMap::~InputMap()
{
}

std::map<int, std::function<void()>> InputMap::getMap(){
	return m_map;
}

void InputMap::setActive(bool a){
	m_active = a;
}

bool InputMap::getActive(){
	return m_active;
}

void InputMap::setName(std::string name){
	m_name = name;
}

std::string InputMap::getName(){
	return m_name;
}

MapType InputMap::getType(){
	return m_type;
}

void InputMap::checkKeys(int key, GLFWwindow &window){
	// You go over the active InputMap, if it's the key that is pressed, a method is called and the mapped action is executed else the key is ignored
	for (std::map<int, std::function<void()>>::iterator it = m_map.begin(); it != m_map.end(); it++){
		if (it->first == key){
			//If the key that is pressed is mapped, check if it is a multipleKeyPressed or not
			bool noMultipleKeyPressed = true;
			for (int i : m_mutlipleKeys){
				if (glfwGetKey(&window, i) == GLFW_PRESS){
					//multipleKey is pressed
					m_map.at(i)();
				}
				if (key = i){
					noMultipleKeyPressed = false;
				}
			}
			if (noMultipleKeyPressed){
				//no multipleKey is pressed
				m_map.at(key)();
			}
		}
		if (it == m_map.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}	
}

void InputMap::setGLFWwindow(GLFWwindow* window)
{
	m_usedWindow = window;
}

GLFWwindow* InputMap::getWindow()
{
	return m_usedWindow;
}

void InputMap::update(Player &p){}
void InputMap::update(Camera &cam){}