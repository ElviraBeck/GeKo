#include "InputHandler.h"

InputHandler::InputHandler()
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::setAllInputMaps(Camera &activeCam){

	// Instances of the Maps, which are named depending on their context
	MapTrackball* contextTrackball = new MapTrackball(activeCam);
	MapPilotview* contextPilotview = new MapPilotview(activeCam);
	MapPlayerview* contextPlayerview = new MapPlayerview(activeCam);
	MapStrategyCamera* contextStrategy = new MapStrategyCamera(activeCam);
	MapObject* contextObject = new MapObject();

	// The Maps are stored in a vector
	m_allInputMaps.push_back(contextTrackball);
	m_allInputMaps.push_back(contextPilotview);
	m_allInputMaps.push_back(contextPlayerview);
	m_allInputMaps.push_back(contextStrategy);
	m_allInputMaps.push_back(contextObject);
}

void InputHandler::changeActiveInputMap(std::string name){
	// additional loop over all the InputMaps is necessary because the method retrieves always the last active InputMap and for the first time there's no InputMap active
	for (int j = 0; j < m_allInputMaps.size(); j++)
	{
		if (m_allInputMaps.at(j)->getActive() == true)
			getActiveInputMap()->setActive(false);
	}
	getInputMap(name)->setActive(true);
}

void InputHandler::changeActiveInputMap(MapType type){
	// additional loop over all the InputMaps is necessary because the method retrieves always the last active InputMap and for the first time there's no InputMap active
	for (int j = 0; j < m_allInputMaps.size(); j++)
	{
		if (m_allInputMaps.at(j)->getActive() == true)
			getActiveInputMap()->setActive(false);
	}
	getInputMap(type)->setActive(true);
}

// TODO maybe an exception 
InputMap* InputHandler::getInputMap(std::string name) {

	InputMap* instance;

	for (int i = 0; i < m_allInputMaps.size(); i++)
	{
		if (m_allInputMaps.at(i)->getName() == name){
			instance = m_allInputMaps.at(i);
			return instance;
		}
	}
	std::cerr << "No map is named like this" << std::endl;

	return 0;
}

InputMap* InputHandler::getInputMap(MapType type) {

	InputMap* instance;

	for (int i = 0; i < m_allInputMaps.size(); i++)
	{
		if (m_allInputMaps.at(i)->getType() == type){
			instance = m_allInputMaps.at(i);
			return instance;
		}
	}
	std::cerr << "There exist no map with this MapType" << std::endl;

	return 0;
}

InputMap* InputHandler::getActiveInputMap() {

	int i = 0;
	InputMap* activeInputMap;

	for (int j = 0; j < m_allInputMaps.size(); j++)
	{
		if (m_allInputMaps.at(j)->getActive() == true){
			activeInputMap = m_allInputMaps.at(j);
			i++;
		}
	}
	if (i == 0){
		std::cerr << "No InputMap is active." << std::endl;
		exit(1);
	}
	else if (i != 1){
		std::cerr << "Too Many InputMaps are active." << std::endl;
		exit(1);
	}
	return activeInputMap;
}