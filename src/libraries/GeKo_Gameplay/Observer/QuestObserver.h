#pragma once
#include <glm/ext.hpp>
#include <GeKo_Gameplay/Observer/Observer.h>
#include <GeKo_Graphics/Scenegraph/Level.h>


/**A Questobserver manages all the quest-changes like increasing a collecting-goal-value or finishing a quest.*/
class QuestObserver : public Observer<Quest, Quest_Event>, public Observer<Goal, Quest_Event>, public Observer<FightSystem, Quest_Event>
{
public:
	QuestObserver(Level* level){ m_level = level; }

	~QuestObserver(){}

	void onNotify(Quest& quest, Quest_Event event)  
	 {
		switch (event){
		case Quest_Event::QUEST_FINISHED:

			//Sets children quests active
			if (m_level->getQuestHandler()->getGraph()->hasChildren())
			{
				std::vector<QuestGraphNode*> tmp = m_level->getQuestHandler()->getGraph()->search(&quest)->getChildren();
				for (QuestGraphNode* n : tmp){
					n->getQuest()->setActive(true);
				}
			}

			//Search player node from scenegraph and save in tmp2 to give him rewards
			std::vector<Node*>* tmp = m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet();
			Node* tmp2;
			for (int i = 0; i < tmp->size(); i++)
			{
				if (tmp->at(i)->getType() == ClassType::PLAYER)
				{
					tmp2 = tmp->at(i);
				}
			}

			for (int i = 1; i < quest.getRewards().size(); i++)
			{
				if (quest.getRewards().at(i)->getType() == Type::REWARD_EXP)
				{
					tmp2->getPlayer()->addExp(quest.getRewards().at(i)->getExp());
				}
			
				if (quest.getRewards().at(i)->getType() == Type::REWARD_ITEM)
				{
					for (int j = 0; j < quest.getRewards().at(i)->getItemList()->size(); j++)
					{
						tmp2->getPlayer()->getInventory()->addItem(quest.getRewards().at(i)->getItemList()->at(j));
					}
					
				}
			}

			//Check if all quests are finished

			break;
	
		}
	 }

	void onNotify(Goal& goal, Quest_Event event)
	{
		switch (event)
		{
		case Quest_Event::GOAL_FINISHED:
			
			for (int i = 0; i < m_level->getQuestHandler()->getQuests().size(); i++)
			{
				m_level->getQuestHandler()->getQuests().at(i)->isFinished();
			}
			break;
		}
	}

	void onNotify(FightSystem& fight, Quest_Event event)
	{
		switch (event)
		{
		case Quest_Event::GOAL_FINISHED:


			for (int i = 0; i < m_level->getQuestHandler()->getQuests().size(); i++)
			{
				m_level->getQuestHandler()->getQuests().at(i)->isFinished();
			}
			break;
		}
	
	}

protected: 
	Level* m_level;
};