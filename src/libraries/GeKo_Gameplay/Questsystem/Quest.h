#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "GeKo_Gameplay/Observer/Subject.h"
#include "Goal.h"
#include "GeKo_Gameplay/Questsystem/Reward.h"


/**Each Quest contains multiple Goals and rewards. A Quest can be active or finished and should contain a quest-text for the GUI!*/
class Quest: public Subject<Quest, Quest_Event>
{
public: 
	///A Quest should have an id
	Quest(int id);
	Quest();
	~Quest();

	///Returns all goals attached to the Quest
	std::vector<Goal*> getGoals();
	void addGoal(Goal* g);

	///Returns all rewards attached to the Quest
	std::vector<Reward*> getRewards();
	void addReward(Reward* r);

	///Returns true if the quest is active
	bool getActive();
	void setActive(bool activity);

	///Returns true if the quest is finished
	bool getFinished();
	///Tests if all goals are finished
	/**If all goals are finished, the quest will be set inactive and finished!*/
	void isFinished();
	
	///Return m_id of the Quest
	int getID();

protected:

	std::vector<Goal*> m_goals;
	std::vector<Reward*> m_rewards;
	bool m_active;
	int m_id;
	bool m_finished;
	std::string m_text;
	
};