#pragma once
#include <glm/ext.hpp>
#include <vector>
#include <GeKo_Gameplay/Observer/Observer.h>
#include <typeinfo>


template <class Entity, class Event>

///A Subject is required to send notifications to Observers.
/**Every class can be an subject, it just has to inherit this class. Each Subject has a list for observers, every type of obsever can
be attached. Notifications will be send to all observers in the list.*/
class Subject
{
public:
	Subject() {}

	~Subject(){}

	void addObserver(Observer<Entity, Event>* observer)
	{
		m_myObserver.push_back(observer);
	}

	void removeObserver(Observer<Entity, Event>* observer)
	{
		for (int i = 0; i < m_myObserver.size(); i++)
		{
			if (typeid(m_myObserver.at(i)) == typeid(observer))
				m_myObserver.erase(m_myObserver.begin() + i);
		}
	}

protected:
	void notify(Entity& node, Event event)
	{
		for (int i = 0; i < m_myObserver.size(); i++)
		{
			m_myObserver.at(i)->onNotify(node, event);
		}
	}

	void notify(Entity& nodeA, Entity& nodeB, Event event)
	{
		for (int i = 0; i < m_myObserver.size(); i++)
		{
			m_myObserver.at(i)->onNotify(nodeA,nodeB, event);
		}
	}

private:
	std::vector<Observer<Entity, Event>*> m_myObserver;
	
};