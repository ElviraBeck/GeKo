#include "GeKo_Gameplay/Questsystem/ExpReward.h"

ExpReward::ExpReward(int id)
{
	m_id = id;
	m_type = REWARD_EXP;
}

ExpReward::ExpReward()
{
	m_id = -1;
	m_type = Type::REWARD_EXP;
}

ExpReward::~ExpReward()
{

}


void ExpReward::setExp(float exp)
{
	m_exp = exp;
}
