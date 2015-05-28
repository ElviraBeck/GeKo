#include "BoundingBox.h"

BoundingBox::BoundingBox()
{

}

BoundingBox::BoundingBox(Node* object){
	m_vertices = object->getGeometry()->getVertices();
}

BoundingBox::~BoundingBox(){
    m_vertices.clear();
}

void BoundingBox::boundingBox(Node* object){
	float maxX = m_vertices.at(0).x;
	float minX = m_vertices.at(0).x;
	
	float maxY = m_vertices.at(0).y;
	float minY = m_vertices.at(0).y;

	float maxZ = m_vertices.at(0).z;
	float minZ = m_vertices.at(0).z;
	
	for (int i = 1; i < m_vertices.size(); i++){
		if (m_vertices.at(i).x > maxX){
			maxX = m_vertices.at(i).x;
		}
		if (m_vertices.at(i).y > maxY){
			maxY = m_vertices.at(i).y;
		}
		if (m_vertices.at(i).z > maxZ){
			maxZ = m_vertices.at(i).z;
		}
		
		if (m_vertices.at(i).x < minX){
			minX = m_vertices.at(i).x;
		}
		if (m_vertices.at(i).y < minY){
			minY = m_vertices.at(i).y;
		}
		if (m_vertices.at(i).z < minZ){
			minZ = m_vertices.at(i).z;
		}
	}
	
	
	m_box.push_back(glm::vec4(minX, minY, maxZ, 1.0));
	m_box.push_back(glm::vec4(maxX, minY, maxZ, 1.0));

	m_box.push_back(glm::vec4(minX, minY, minZ, 1.0));
	m_box.push_back(glm::vec4(maxX, minY, minZ, 1.0));

	m_box.push_back(glm::vec4(minX, maxY, maxZ, 1.0));
	m_box.push_back(glm::vec4(maxX, maxY, maxZ, 1.0));

	m_box.push_back(glm::vec4(minX, maxY, minZ, 1.0));
	m_box.push_back(glm::vec4(maxX, maxY, minZ, 1.0));
	
}

void BoundingBox::boundingBox(Geometry* geometry)
{
	m_vertices = geometry->getVertices();

	float maxX = m_vertices.at(0).x;
	float minX = m_vertices.at(0).x;

	float maxY = m_vertices.at(0).y;
	float minY = m_vertices.at(0).y;

	float maxZ = m_vertices.at(0).z;
	float minZ = m_vertices.at(0).z;

	for (int i = 1; i < m_vertices.size(); i++){
		if (m_vertices.at(i).x > maxX){
			maxX = m_vertices.at(i).x;
		}
		if (m_vertices.at(i).y > maxY){
			maxY = m_vertices.at(i).y;
		}
		if (m_vertices.at(i).z > maxZ){
			maxZ = m_vertices.at(i).z;
		}

		if (m_vertices.at(i).x < minX){
			minX = m_vertices.at(i).x;
		}
		if (m_vertices.at(i).y < minY){
			minY = m_vertices.at(i).y;
		}
		if (m_vertices.at(i).z < minZ){
			minZ = m_vertices.at(i).z;
		}
	}


	m_box.push_back(glm::vec4(minX, minY, maxZ, 1.0));
	m_box.push_back(glm::vec4(maxX, minY, maxZ, 1.0));

	m_box.push_back(glm::vec4(minX, minY, minZ, 1.0));
	m_box.push_back(glm::vec4(maxX, minY, minZ, 1.0));

	m_box.push_back(glm::vec4(minX, maxY, maxZ, 1.0));
	m_box.push_back(glm::vec4(maxX, maxY, maxZ, 1.0));

	m_box.push_back(glm::vec4(minX, maxY, minZ, 1.0));
	m_box.push_back(glm::vec4(maxX, maxY, minZ, 1.0));

}

std::vector<glm::vec4> BoundingBox::getBox()
{
	return m_box;
}

void BoundingBox::setCollisionDetected(bool collision)
{
	m_collisionDetected = collision;

}

bool BoundingBox::getCollisionDetected()
{
	return m_collisionDetected;
}