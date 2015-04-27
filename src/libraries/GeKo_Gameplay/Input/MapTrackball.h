#pragma once

#include "InputMap.h"

/** MapTrackball is a derived class of InputMap and defines the mapping of input to the trackball camera*/

class MapTrackball : public InputMap
{
public:
	MapTrackball(Camera &activeCam);
	~MapTrackball();

	/// The method fills the m_map considering the active camera
	/** With the help of a lambda function the camera can call its methods.
	The lambda method gets then mapped to a key in m_map
	*/
	void update(Camera &activeCam);
};