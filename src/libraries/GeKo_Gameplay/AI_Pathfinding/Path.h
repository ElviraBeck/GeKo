#pragma once 

/** The Path-Class provides the possibility to connect waypoints, referred to as endNode and startNode. Each Path has a variable which determines how long it will take to
travel this path!*/
 template<class T>

class Path 
{
public: 
	///Each Path needs a travel-time, a start- and end point
	/**The distance will be calculated automatially*/
	Path(int timeToTravel, T* startNode, T* endNode)
	{
		m_timeToTravel = timeToTravel;
		m_endNode = endNode;
		m_startNode = startNode;
		glm::vec3 temp = startNode->getPosition() - endNode->getPosition();
		m_distance = sqrt((temp.x * temp.x) + (temp.y * temp.y) + (temp.z * temp.z));
	}
	~Path(){};
	
	///Returns m_timeToTravel
	/**/
	int getTimeToTravel(){ 
		return m_timeToTravel;}
	
	///Returns m_endNode
	/**/
	T* getEndNode(){
		return m_endNode;
	}

	///Returns m_startNode
	/**/
	T* getStartNode(){
		return m_startNode;
	}

	///Returns the distance between the two connected points
	/**/
	float getDistance(){
		return m_distance;
	}

	
protected:
	int m_timeToTravel;
	float m_distance;
	T* m_endNode;
	T* m_startNode;
};