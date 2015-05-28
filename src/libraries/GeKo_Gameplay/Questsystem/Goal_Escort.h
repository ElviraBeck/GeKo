#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"
#include "GeKo_Gameplay/Object/AI.h"


/**A subclass of Goal which can be used for escort quests!*/
class Goal_Escort : public Goal
{
public: 
	///Should have an id
	Goal_Escort(int id);
	Goal_Escort();
	~Goal_Escort();

	void setAiId(int id);
	///Sets the position of the destination of the escort
	void setPosition(glm::vec3 m_position);
	///This method finishes the goal
	void completed();


	protected:
		int m_AiId;
		glm::vec3 m_position;
};