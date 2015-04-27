#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"

/**A subclass of Goal which can be used for travel quests!*/
class Goal_Travel : public Goal
{
public: 
	///Should have an id
	Goal_Travel(int id);
	Goal_Travel();
	~Goal_Travel();

	///The position to which the Player should go will be set
	void setPosition(glm::vec3 m_position);
	///This method finishes the goal
	void completed();


	protected:
		glm::vec3 m_position;
};