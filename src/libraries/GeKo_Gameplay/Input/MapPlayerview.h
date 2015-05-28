#pragma once

#include "InputMap.h"

/** MapPlayerview is a derived class of InputMap and defines the mapping of input to the playerview camera*/

class MapPlayerview : public InputMap
{
public:
	MapPlayerview(Camera &activeCam);
	~MapPlayerview();

	/// The method fills the m_map considering the active camera
	/** With the help of a lambda function the camera can call its methods.
	The lambda method gets then mapped to a key in m_map
	*/
	void update(Camera &activeCam);
};

