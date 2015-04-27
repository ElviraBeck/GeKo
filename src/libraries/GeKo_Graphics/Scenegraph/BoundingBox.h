#pragma once
#include <vector>
#include <glm/ext.hpp>
#include <GeKo_Graphics/Scenegraph/Node.h>

/** The Bounding Box class provides a cube-like box, which contains an Object completely*/
class BoundingBox {

public:
	///Each Bounding Box needs an Object to wrap
	/**/
	BoundingBox();
	BoundingBox(Node* object);
	BoundingBox(Geometry* object, glm::mat4 modelMatrix);
	~BoundingBox();

	///Returns all points of the Box
	/**/
	void boundingBox(Node* object);

	void boundingBox(Geometry* geometry);

	///Returns the Points in a vector
	/**/
	std::vector<glm::vec4> getBox();

	///Will be used to set the boolean for a collision
	/**/
	void setCollisionDetected(bool collision);
	///return m_collisionDetected, true if a collision happened, false if not
	/**/
	bool getCollisionDetected();

private:
    std::vector<glm::vec4> m_vertices;
	std::vector<glm::vec4> m_box;

	bool m_collisionDetected;
};