# include "GeKo_Gameplay/AI_Pathfinding/AStarAlgorithm.h"

AStarAlgorithm::AStarAlgorithm(std::string name) : Algorithm(name)
{
}

AStarAlgorithm::~AStarAlgorithm()
{
}

AStarNode* AStarAlgorithm::startAlgorithm(AStarNode* startNode, AStarNode* endNode)
{
	std::cout << "A*-ALGORITHM WILL BE STARTED" << std::endl;

	std::vector<AStarNode*> pathList;
	std::cout << startNode->getName() << " GOES INTO THE PATHLIST" << std::endl;
	pathList.push_back(startNode);

	std::vector<AStarNode*> wastedList;

	AStarNode* tmp;
	bool wasIn = false;
	bool shorterPathFound = false;

	int distanceTraveled = 0;

	bool secondOut = false;
	AStarNode* returnNode;
	returnNode = startNode;

	while (pathList.back()->getName() != endNode->getName() && !pathList.empty())
	{
		std::cout << "FRONT ELEMENT OF THE LIST GOES OUT: " << pathList.back()->getName()<< "(" << pathList.back()->getTemporary() << ")" << std::endl;
		tmp = pathList.back();
		wastedList.push_back(pathList.back());
		pathList.pop_back();

		std::cout << "THE OTHER ELEMENTS IN THE LIST ARE: ";
		for (int bb = 0; bb < pathList.size(); bb++)
		{
			std::cout << pathList.at(bb)->getName() << "(" << pathList.at(bb)->getTemporary() << ")";
		}
		std::cout << std::endl;

		if (secondOut)
		{
			returnNode = tmp;
			secondOut = false;
		}

		if (tmp->getName() == startNode->getName())
		{
			secondOut = true;
		}

		for (int i = 0; i < tmp->getPaths()->size(); i++)
		{
			//Test, if the endnode of the path that we want to add, was visited beforehand. If this was the case, it would be an returnpath which we do not want here.
			if (tmp->getPaths()->at(i)->getEndNode()->getName() != tmp->getVisitor()->getName() && tmp->getPaths()->at(i)->getEndNode()->getName() != startNode->getName())
			{
				/*Now we have to check if there is a node with the same name in our pathlist.
				*If there is, we want to copy it, because we want to be able to safe more than one path to a node*/
				AStarNode* copyNode = new AStarNode();
				for (int j = 0; j < pathList.size(); j++)
				{
					if (tmp->getPaths()->at(i)->getEndNode()->getName() == pathList.at(j)->getName())
					{
						wasIn = true;
						copyNode->setPathList(*pathList.at(j)->getPaths());
						copyNode->setName(pathList.at(j)->getName());
						break;
					}
				}
				for (int m = 0; m < wastedList.size(); m++)
				{
					if (tmp->getPaths()->at(i)->getEndNode()->getName() == wastedList.at(m)->getName())
					{
						wasIn = true;
						copyNode->setPathList(*wastedList.at(m)->getPaths());
						copyNode->setName(wastedList.at(m)->getName());
						break;
					}
				}

				//If the node was not found in the pathlist, it will added normally
				if (!wasIn){
					tmp->getPaths()->at(i)->getEndNode()->setDistanceTravelled(tmp->getPaths()->at(i)->getTimeToTravel() + tmp->getDistanceTravelled());
					tmp->getPaths()->at(i)->getEndNode()->setTemporary(tmp->getPaths()->at(i)->getEndNode()->getDistanceToGoal() + tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled());
					
					//We just add the path, if there is no path in the list which is shorter
					for (int n = 0; n < pathList.size(); n++)
					{
						//There is a path with a shorter travelled disitance
						if (pathList.at(n)->getName() == tmp->getPaths()->at(i)->getEndNode()->getName() && pathList.at(n)->getDistanceTravelled() < tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled())
						{
							//setting the boolean
							shorterPathFound = true;

						}
						//the existing path to the node which has a longer distanceTravelled will be deleted from the list
						else if (pathList.at(n)->getName() == tmp->getPaths()->at(i)->getEndNode()->getName() && pathList.at(n)->getDistanceTravelled() >= tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled()){
							pathList.erase(pathList.begin() + n);
						}
					}

					if (!shorterPathFound)
					{
						pathList.push_back(tmp->getPaths()->at(i)->getEndNode());
						tmp->getPaths()->at(i)->getEndNode()->setVisitor(tmp);
					}
					else{
						shorterPathFound = false;
					}
				}
				//If the node was reached through another path, we copy the node and add this copy additionally to the list
				else{
					copyNode->setDistanceTravelled(-(copyNode->getDistanceTravelled()));
					copyNode->setDistanceTravelled(tmp->getPaths()->at(i)->getTimeToTravel() + tmp->getDistanceTravelled());
					copyNode->setTemporary(tmp->getPaths()->at(i)->getEndNode()->getDistanceToGoal() + copyNode->getDistanceTravelled());

					//We just add the path, if there is no path in the list with a shorter pathdistance
					for (int n = 0; n < pathList.size(); n++)
					{
						//There is a path with a shorter travelledDistance
						if (pathList.at(n)->getName() == copyNode->getName() && pathList.at(n)->getDistanceTravelled() < copyNode->getDistanceTravelled())
						{
							//setting the boolean
							shorterPathFound = true;

						}
						else if (pathList.at(n)->getName() == tmp->getPaths()->at(i)->getEndNode()->getName() && pathList.at(n)->getDistanceTravelled() >= tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled()){
							pathList.erase(pathList.begin() + n);
						}
					}

					if (!shorterPathFound)
					{
						copyNode->setVisitor(tmp);
						pathList.push_back(copyNode);
					}
					else{
						shorterPathFound = false;
					}
					wasIn = false;
				}
			}
			else{
			}
		}

		//The pathlist must be sorted anew
		AStarNode* save;
		for (int k = 1; k < pathList.size(); k++)
		{
			for (int l = 0; l < pathList.size() - k; l++)
			{
				if (pathList.at(l)->getTemporary() < pathList.at(l + 1)->getTemporary())
				{
					save = pathList.at(l + 1);
					pathList.at(l + 1) = pathList.at(l);
					pathList.at(l) = save;
				}
			}
		}
	}
	std::string test = pathList.back()->getName();

	if (secondOut)
	{
		returnNode = endNode;
	}

	return returnNode;
}


std::vector<AStarNode*>* AStarAlgorithm::startAlgorithm2(AStarNode* startNode, AStarNode* endNode, std::vector<AStarNode*> &path)
{
	std::vector<AStarNode*> pathList;
	pathList.push_back(startNode);

	std::vector<AStarNode*> wastedList;

	AStarNode* tmp;
	bool wasIn = false;
	bool shorterPathFound = false;

	int distanceTraveled = 0;

	bool secondOut = false;
	AStarNode* returnNode;
	returnNode = startNode;

	while (pathList.back()->getName() != endNode->getName() && !pathList.empty())
	{
		tmp = pathList.back();
		wastedList.push_back(pathList.back());
		pathList.pop_back();

		if (secondOut)
		{
			returnNode = tmp;
			secondOut = false;
		}

		if (tmp->getName() == startNode->getName())
		{
			secondOut = true;
		}

		//adding new paths
		for (int i = 0; i < tmp->getPaths()->size(); i++)
		{
			if (tmp->getPaths()->at(i)->getEndNode()->getName() != tmp->getVisitor()->getName() && tmp->getPaths()->at(i)->getEndNode()->getName() != startNode->getName())
			{
				AStarNode* copyNode = new AStarNode();
				for (int j = 0; j < pathList.size(); j++)
				{
					if (tmp->getPaths()->at(i)->getEndNode()->getName() == pathList.at(j)->getName())
					{
						wasIn = true;
						copyNode->setPathList(*pathList.at(j)->getPaths());
						copyNode->setName(pathList.at(j)->getName());
						copyNode->setDistanceToGoal(pathList.at(j)->getDistanceToGoal());
						copyNode->setPosition(wastedList.at(j)->getPosition());
						break;
					}
				}
				for (int m = 0; m < wastedList.size(); m++)
				{
					if (tmp->getPaths()->at(i)->getEndNode()->getName() == wastedList.at(m)->getName())
					{
						wasIn = true;
						copyNode->setPathList(*wastedList.at(m)->getPaths());
						copyNode->setName(wastedList.at(m)->getName());
						copyNode->setDistanceToGoal(wastedList.at(m)->getDistanceToGoal());
						copyNode->setPosition(wastedList.at(m)->getPosition());
						break;
					}
				}
				if (!wasIn){
					tmp->getPaths()->at(i)->getEndNode()->setDistanceTravelled(tmp->getPaths()->at(i)->getTimeToTravel() + tmp->getDistanceTravelled());
					tmp->getPaths()->at(i)->getEndNode()->setTemporary(tmp->getPaths()->at(i)->getEndNode()->getDistanceToGoal() + tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled());
					for (int n = 0; n < pathList.size(); n++)
					{
						if (pathList.at(n)->getName() == tmp->getPaths()->at(i)->getEndNode()->getName() && pathList.at(n)->getDistanceTravelled() < tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled())
						{
							shorterPathFound = true;
						}
						else if (pathList.at(n)->getName() == tmp->getPaths()->at(i)->getEndNode()->getName() && pathList.at(n)->getDistanceTravelled() >= tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled()){
							pathList.erase(pathList.begin() + n);
						}
					}

					if (!shorterPathFound)
					{
						pathList.push_back(tmp->getPaths()->at(i)->getEndNode());
						tmp->getPaths()->at(i)->getEndNode()->setVisitor(tmp);
					}
					else{
						shorterPathFound = false;
					}
				}
				else{
					copyNode->setDistanceTravelled(-(copyNode->getDistanceTravelled()));
					copyNode->setDistanceTravelled(tmp->getPaths()->at(i)->getTimeToTravel() + tmp->getDistanceTravelled());
					copyNode->setTemporary(tmp->getPaths()->at(i)->getEndNode()->getDistanceToGoal() + copyNode->getDistanceTravelled());
					for (int n = 0; n < pathList.size(); n++)
					{
						if (pathList.at(n)->getName() == copyNode->getName() && pathList.at(n)->getDistanceTravelled() < copyNode->getDistanceTravelled())
						{ 
							shorterPathFound = true;
						}
						else if (pathList.at(n)->getName() == tmp->getPaths()->at(i)->getEndNode()->getName() && pathList.at(n)->getDistanceTravelled() >= tmp->getPaths()->at(i)->getEndNode()->getDistanceTravelled()){
							pathList.at(n)->setDistanceTravelled(copyNode->getDistanceTravelled());
							pathList.at(n)->setVisitor(tmp);
						}
					}

					if (!shorterPathFound)
					{
					}
					else{
						shorterPathFound = false;
					}
					wasIn = false;
				}
			}
			else{
			}
		}

		AStarNode* save;
		for (int k = 1; k < pathList.size(); k++)
		{
			for (int l = 0; l < pathList.size() - k; l++)
			{
				if (pathList.at(l)->getTemporary() < pathList.at(l + 1)->getTemporary())
				{
					save = pathList.at(l + 1);
					pathList.at(l + 1) = pathList.at(l);
					pathList.at(l) = save;
				}
			}
		}
	}

	if (secondOut)
	{
		returnNode = endNode;
	}

	path.push_back(pathList.back());
	while (path.back()->getName() != startNode->getName())
	{
		path.push_back(path.back()->getVisitor());
	}
	return &m_path;
}