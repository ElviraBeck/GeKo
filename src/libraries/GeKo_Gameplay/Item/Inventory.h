#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Item.h"
#include "ItemType.h"

class Inventory
{
public: 
	Inventory();
	~Inventory();

	void addItem(Item* item);
	void addItem(ItemType typeId);
	void addItem(Item* item, int count);
	void print();
	ItemType getItemTypeIdAt(int position);
	void deleteItem(int position);
	void deleteItem(Item* item);
	void reduceItem(int position, int count);
	void reduceItem(ItemType typeId, int count);
	void clearInventory();
	int countItem(ItemType typeId);

	protected:
		int m_id;
		std::vector<std::pair<int, ItemType>> m_content;
};