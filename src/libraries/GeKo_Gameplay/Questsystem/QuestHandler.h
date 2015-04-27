#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Quest.h"
#include "GeKo_Gameplay/Questsystem/QuestGraph.h"

#include "Goal.h"

///The questandler provides a graph which contains quests. Each quest stands in relation with another quest that has to be finished beforehand.
/**/
class QuestHandler
{
public: 
	QuestHandler();
	~QuestHandler();

	///Returns the list of Quests which was added to the questhandler
	/**/
	std::vector<Quest*> getQuests();
	void addQuest(Quest* q);

	///Returns all quests which are active but not finished
	std::vector<Quest*> getActiveAndNotFinished();
	///Returns all the quests which are not finished but active and have the given gType
	std::vector<Goal*> getQuests(GoalType gType);
	
	///Returns the m_graph of the questhandler
	QuestGraph* getGraph();
	void setGraph(QuestGraph* g);
	

protected:
	std::vector<Quest*> m_questLog;
	QuestGraph* m_questGraph;
};