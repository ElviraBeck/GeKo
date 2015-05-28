#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "ItemType.h"

class Item
{
public: 
	Item(int id);
	Item();
	~Item();

	void setId(int id);
	void setName(std::string name);
	void setTypeId(ItemType id);
	ItemType getTypeId();

	protected:
		int m_id;
		std::string m_name;
		ItemType m_typeId;
};