#pragma once
#include <glm/ext.hpp>
#include <GeKo_Gameplay/Observer/Collision_Event.h>
#include <GeKo_Gameplay/Observer/Object_Event.h>
#include <GeKo_Gameplay/Observer/Quest_Event.h>


template <class Entity, class Event>

///A Observer observes the subject-classes.
/**A Observer observes the subject-classes. It is an interface which provides the two necessary methods to send notifications.
The methods are virtual and have to be implemented by specific observer-classes.*/
class Observer
{
public:
	Observer(){}

	~Observer(){}

	virtual void onNotify(Entity& node, Event event){}

	virtual void onNotify(Entity& nodeA, Entity& nodeB, Event event){}

};