#include "GeKo_Gameplay/Questsystem/ItemReward.h"

ItemReward::ItemReward(int id)
{
	m_id = id;
	m_exp = 0.0;
	m_type = Type::REWARD_ITEM;
}

ItemReward::ItemReward()
{
	m_id = -1;
	m_exp = 0.0;
	m_type = Type::REWARD_ITEM;
}

ItemReward::~ItemReward()
{

}


void ItemReward::addItem(Item* item)
{
	m_itemList.push_back(item);
}

