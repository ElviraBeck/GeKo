#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>
#include  <GeKo_Gameplay/Questsystem/Reward.h>


/**A subclass of Reward which will give the player just EXP as a reward!*/
class ExpReward : public Reward
{
public: 
	///Should need an id
	ExpReward(int id);
	ExpReward();
	~ExpReward();

	///Set the exp which will the player will gain by completing the quest
	void setExp(float exp);

};