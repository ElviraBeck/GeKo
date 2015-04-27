#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"
#include "GeKo_Gameplay/Item/Item.h"

/**A subclass of Goal which can be used for collect quests!*/
class Goal_Collect : public Goal
{
public: 
	///Should have an id
	Goal_Collect(int id);
	Goal_Collect();
	~Goal_Collect();

	///Increases the collection which was collected by the player
	void setActualCount(int actualC);
	///The number of collected items which has to be collected by the player
	void setGoalCount(int goalC);

	///Returns the itemtype which has to be collected
	ItemType getItemType();
	void setItemType(ItemType iType);

	///Increases the counter
	void increase();


	protected:
		int m_actualCount;
		int m_goalCount;
		ItemType m_itemType;
};