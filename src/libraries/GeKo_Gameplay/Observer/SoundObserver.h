#pragma once
#include <glm/ext.hpp>
#include <GeKo_Gameplay/Observer/Observer.h>
#include <GeKo_Graphics/Scenegraph/Level.h>
#include <sstream>

/**This Observer handles all the sound actions like the move-sound or the fight-sounds.*/
class SoundObserver : public Observer<AI, Object_Event>, public Observer<Player, Object_Event>, public Observer<Node, Collision_Event>, public Observer<Quest, Quest_Event>
{
public:
	SoundObserver(Level* level){ m_level = level; m_fires = 0; }

	~SoundObserver(){}

	void  onNotify(AI& node, Object_Event event)
	{
		std::string name = node.getNodeName();
		std::string soundName;
		std::stringstream streamName;

		Node* tmp = m_level->getActiveScene()->getScenegraph()->searchNode(name);
		switch (event)
		{

		case Object_Event::OBJECT_MOVED:
			soundName = node.getSourceName(MOVESOUND_AI);
		//	node.getSoundHandler()->updateListenerPosition(node.getPosition());
			node.updateSourcesInMap();
			if (soundName != "oor")
			{
				if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->playSource(soundName);
				}
			}
			break;

		case Object_Event::OBJECT_STOPPED:
			soundName = node.getSourceName(MOVESOUND_AI);
			if (soundName != "oor")
			{
				if ((node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->pauseSource(soundName);
				}
			}
			break;

		case Object_Event::OBJECT_DIED:

			soundName = node.getSourceName(DEATHSOUND_AI);
			if (soundName != "oor")
			{
				if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->playSource(soundName);
				}
			}

			soundName = node.getSourceName(DEATHSOUND_FLIES_AI);
			if (soundName != "oor")
			{
				if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				{
					node.getSoundHandler()->playSource(soundName);
				}
			}

				
			break;

		}
	}

	void onNotify(Player& node, Object_Event event)
	 {
		 std::string name = node.getNodeName();
		 std::string soundName;
		 std::stringstream streamName;
		 switch (event)
		 {

		 case Object_Event::OBJECT_MOVED:
			 soundName =  node.getSourceName(MOVESOUND);
			 node.getSoundHandler()->updateListenerPosition(glm::vec3(node.getPosition()));
			 node.updateSourcesInMap();
			 if (soundName != "oor")
			 {
				 if (!(node.getSoundHandler()->sourceIsPlaying(soundName)))
				 {
					 node.getSoundHandler()->playSource(soundName);
				 }
			 }
	
			 break;

		 case Object_Event::OBJECT_ROTATED:
			 node.getSoundHandler()->updateListenerOrientation(glm::vec3(node.getViewDirection()), glm::vec3(0.0, 1.0, 0.0));

		 case Object_Event::OBJECT_STOPPED:
			 soundName = node.getSourceName(MOVESOUND);
			 if (soundName != "oor")
			 {
				 if ((node.getSoundHandler()->sourceIsPlaying(soundName)))
				 {
					 node.getSoundHandler()->pauseSource(soundName);
				 }
			 }
			 break;

		 case Object_Event::PLAYER_COLLECT:
			soundName = node.getSourceName(ITEMSOUND);
			 if (soundName != "oor")
			 {
					 node.getSoundHandler()->playSource(soundName);
			 }
			 break;

		 case Object_Event::PLAYER_SET_ON_FIRE:
			 streamName << "Fire" << ++m_fires;
			 name = streamName.str();
			 node.getSoundHandler()->generateSource(name, glm::vec3(node.getPosition()), RESOURCES_PATH "/Sound/Feuer_kurz.wav");
			 node.getSoundHandler()->playSource(name);
			 streamName.str("");
			 break;
		 }
	 }

	void onNotify(Node& nodeA, Node& nodeB, Collision_Event event)
	{
		std::string tmp;
		switch (event)
		{
		case Collision_Event::COLLISION_AI_FIGHT_PLAYER:
			tmp = nodeB.getPlayer()->getSourceName(FIGHTSOUND);

			if (glm::length(nodeA.getBoundingSphere()->center - nodeB.getBoundingSphere()->center) <= 4.5)
			{

				if (nodeA.getAI()->getHealth() > 0)
				{
					if (tmp != "oor")
					{
						if (!nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(tmp))
						{
							nodeB.getPlayer()->getSoundHandler()->playSource(tmp);
						}

					}

				}
				else{
					nodeB.getPlayer()->getSoundHandler()->stopSource(tmp);
				}
			}

			else{
				if (nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(tmp))
				{
					nodeB.getPlayer()->getSoundHandler()->stopSource(tmp);
				}
			}
			break;

		case Collision_Event::COLLISION_KI_PLAYER:
			if (!nodeA.getAI()->getStates(States::HEALTH) && nodeA.getAI()->getInventory()->countItem(ItemType::COOKIE) > 0)
			{
				std::string soundName = nodeB.getPlayer()->getSourceName(EATSOUND);
				if (soundName != "oor")
				{
					if (!(nodeB.getPlayer()->getSoundHandler()->sourceIsPlaying(soundName)))
					{
						nodeB.getPlayer()->getSoundHandler()->playSource(soundName);
					}
				}
			}
		}
	}

	void  onNotify(Quest& quest, Quest_Event event)
	{
		switch (event){
		case Quest_Event::QUEST_FINISHED:
			std::vector<Node*>* tmp = m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet();
			Node* tmp2;
			for (int i = 0; i < tmp->size(); i++)
			{
				if (tmp->at(i)->getType() == ClassType::PLAYER)
				{
					tmp2 = tmp->at(i);
				}
			}

			std::string soundName = tmp2->getPlayer()->getSourceName(QUESTSOUND);

			if (soundName != "oor")
			{
				tmp2->getPlayer()->getSoundHandler()->playSource(soundName);
			}
		}
	}
protected: 
	Level* m_level;
	int m_fires;
	int m_flies;
};