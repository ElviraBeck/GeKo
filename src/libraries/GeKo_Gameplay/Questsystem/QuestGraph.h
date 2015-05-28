#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "GeKo_Gameplay/Questsystem/QuestGraphNode.h"
#include "GeKo_Gameplay/Questsystem/Goal_Collect.h"

/**Each QuestGraph consists of QuestGraphNodes. It is used for the questhandler to create a hierarchy of quests.*/
class QuestGraph 
{
public: 
	QuestGraph();
	~QuestGraph();

	///Returns all the nodes in the QuestGraph
	std::vector<QuestGraphNode*> getNodes();
	void addNode(QuestGraphNode* n);

	///This method should be used to create an example of quests.
	/**The Player has to collect branches and cookies!*/
	QuestGraph* setExampleGraph();

	///Returns the QuestGraphNode which contains a given quest-object
	QuestGraphNode* search(Quest* quest);

	///Returns true if the graph has nodes
	bool hasChildren();

protected:
	std::vector<QuestGraphNode*> m_nodes;

	bool m_hasChildren;
		
};