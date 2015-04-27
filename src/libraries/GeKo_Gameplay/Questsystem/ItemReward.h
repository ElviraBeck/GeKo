#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include  "GeKo_Gameplay/Questsystem/Reward.h"

/**A subclass of Reward which will just grant multiple items to the player!*/
class ItemReward: public Reward
{
public: 
	///A ItemReward should have an id
	ItemReward(int id);
	ItemReward();
	~ItemReward();


	void addItem(Item* item);



protected:
	
};