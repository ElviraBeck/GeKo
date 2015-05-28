#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include  "GeKo_Gameplay/Item/Item.h"

enum Type {
	REWARD, REWARD_ITEM, REWARD_EXP
};

///A Quest can have rewards which will be granted to the player!
/**/
class Reward 
{
public: 

	///Each reward should have an ID
	Reward(int id);
	Reward();
	~Reward();

	///Returns m_id of the reward object
	int getId();
	void setID(int id);

	///Returns m_exp of the reward object
	float getExp();

	///Returns m_itemList of the reward object
	/**The item list contains items which can be granted to the player!*/
	std::vector<Item*>* getItemList();

	///Returns m_type of the reward object
	/**Possible types can be ItemReward or ExpReward.*/
	Type getType();

	protected:
		int m_id;

		float m_exp;

		std::vector<Item*> m_itemList;

		Type m_type;
};