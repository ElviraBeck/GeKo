#pragma once
#include <string>
#include <vector>
#include <glm/ext.hpp>
#include "GeKo_Gameplay/AI_Pathfinding/Path.h"
#include "GraphNodeType.h"

/** A GraphNode-class provides the possibility to create waypoints, which can be used by AI-Units and Pathfinding algorithms.
Every Node has a Name and knows its distance to a goal-Waypoint. It knows which paths are connected with it and it will recognize when a AI-Unit passes over it.
Over all it has a position on the terrain.*/
template<class T>

class GraphNode
{
public: 
	///Every GraphNode needs a name
	/**/
	GraphNode(std::string name)
	{
		m_name = name;
		m_type = GraphNodeType::DEFAULT;
	}
	~GraphNode(){}
	
	///Adds a Path to the m_paths vector-list
	/**/
	void addPath(Path<T>* path)
	{
		m_paths.push_back(path);
	}
	
	///Returns all the Paths of m_paths list
	/**/
	std::vector<Path<T>*>* getPaths()
	{
		return &m_paths;
	}

	///Returns the Name of the Node
	/**/
	std::string getName()
	{
		return m_name;
	}

	///Sets m_name to name
	/**/
	void setName(std::string name)
	{
		m_name = name;
	}

	///Sets m_paths to paths
	/**/
	void setPathList(std::vector<Path<T>*> paths)
	{
		m_paths = paths;
	}

	///Sets m_position to position
	/**/
	void setPosition(glm::vec3 position)
	{
		posX = position.x;
		posY = position.y;
		posZ = position.z;

		m_position = position;
	}

	///Returns the position on the terrain
	/**/
	glm::vec3 getPosition()
	{
		return m_position;
	}

	///Returns the type of the Node like Food or Home
	/**/
	GraphNodeType getNodeType(){
		return m_type;
	}
	void setNodeType(GraphNodeType type){
		m_type = type;
	}
	
protected:
	std::vector<Path<T>*> m_paths;
	std::string m_name;
	GraphNodeType m_type;
	

	glm::vec3 m_position;
	float posX;
	float posY;
	float posZ;
};