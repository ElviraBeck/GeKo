#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Object.h"

class StaticObject : public Object
{
public: 
	StaticObject();
	~StaticObject();

	void setTree();
	void setTree(int i);
	void setExampleAntHome();

};