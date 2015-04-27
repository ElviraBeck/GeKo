#include "GeKo_Gameplay/Questsystem/Reward.h"

Reward::Reward(int id)
{
	m_id = id;
	m_type = Type::REWARD;
}

Reward::Reward()
{
	m_id = -1;
	m_type = Type::REWARD;
}

Reward::~Reward()
{

}

int Reward::getId()
{
	return m_id;
}

void Reward::setID(int id)
{
	m_id = id;
}

float Reward::getExp()
{
	return m_exp;
}


std::vector<Item*>* Reward::getItemList()
{
	return &m_itemList;

}
Type Reward::getType()
{
	return m_type;
}