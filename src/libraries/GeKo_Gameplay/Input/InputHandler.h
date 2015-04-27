#pragma once

#include "InputMap.h"
#include "MapTrackball.h"
#include "MapPilotview.h"
#include "MapPlayerview.h"
#include "MapStrategyCamera.h"
#include "MapObject.h"
#include <cstdlib> 

/** InputHandler creates the various InputMaps and manages them.
For this, you have to include the InputHandler in the main.cpp*/

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	/// The method creates InputMaps.
	/**The method fills the InputMaps specifically with the aid of the indivdual constructor call
	 and pushes them onto a vector which is the member variable m_allInputMaps*/
	void setAllInputMaps(Camera &activeCam);

	// The method sets the InputMap active according to its name and ensures that just this InputMap is active
	void changeActiveInputMap(std::string name);
	// The method sets the InputMap active according to its MapType and ensures that just this InputMap is active
	void changeActiveInputMap(MapType type);

	/// The method returns the InputMap instance with the matching name
	/** If there is any InputMap named as the parameter the according InputMap is returned,
	otherwise an error message is given.
	*/ 
	InputMap* getInputMap(std::string name);
	/// The method returns the InputMap instance with the matching MapType
	/** Each map can only exist once and because of that this method can only return the map of this maptype
	*/
	InputMap* getInputMap(MapType type);

	/// The method returns the active InputMap
	/** Iterates over m_allInputMaps, where all the InputMaps are stored and returns the one,
	whose member variable m_active is set true.
	If there is no active InputMap the program exits with a message saying there's no InputMap active
	If there is more than one active InputMap the program exits with a message saying it's not the only map being active
	*/
	InputMap* getActiveInputMap();

protected:
	// The vector stores all the InputMaps by using setAllInputMaps
	std::vector<InputMap*> m_allInputMaps;
};

