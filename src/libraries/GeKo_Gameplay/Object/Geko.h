#pragma once

#include "GeKo_Gameplay/Object/Player.h"

/**This class is a first generalisation of the Player class.*/
class Geko : public Player
{
public:
	///Same Constructors as the Player-class
	/** The Speed will be set to 0.8 and viewDirection and movement vector will be set automatically!*/
	Geko(std::string gekoName);
	Geko(std::string gekoName, glm::vec3 spawnPoint);
	Geko();
	~Geko();

	///Sets a fire into the world
	/**This method deletes one branch in the inventory and creates a fire with the help of the particlesystem*/
	void setFire();
};