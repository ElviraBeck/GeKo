#pragma once

#include "InputMap.h"

/** MapPilotview is a derived class of InputMap and defines the mapping of input to the pilotview camera*/

class MapPilotview : public InputMap
{
public:
	MapPilotview(Camera &activeCam);
	~MapPilotview();

	/// The method fills the m_map considering the active camera
	/** With the help of a lambda function the camera can call its methods.
	The lambda method gets then mapped to a key in m_map
	*/
	void update(Camera &activeCam);
};