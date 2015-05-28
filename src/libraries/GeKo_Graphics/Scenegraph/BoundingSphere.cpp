#include <GeKo_Graphics/Scenegraph/BoundingSphere.h>

BoundingSphere::BoundingSphere(Geometry* object, glm::mat4 modelMatrix){
	m_collisionDetected = false;
	createSphere(object, modelMatrix);		
}

BoundingSphere::BoundingSphere(double rad, glm::vec3 cent)
{
	radius = rad;
	center = cent;
	originalCenter = cent;
}

BoundingSphere::~BoundingSphere(){
   
}

void BoundingSphere::update(glm::mat4 modelMatrix)
{
	center = glm::vec3(modelMatrix * glm::vec4(originalCenter, 1.0));
}


void BoundingSphere::setCollisionDetected(bool collision)
{
	m_collisionDetected = collision;

}
bool BoundingSphere::getCollisionDetected()
{
	return m_collisionDetected;
}

void BoundingSphere::createSphere(Geometry* object, glm::mat4 modelMatrix)
{
	std::vector<glm::vec4> vs = object->getVertices();

	int i, n;
	float dx, dy, dz, le;
	glm::vec4 lev(1.0);

	n = vs.size();

	center = glm::vec3(0.0);

	for (int i = 0; i < n - 1; i++)
	{
		center.x = center.x + vs.at(i).x;
		center.y = center.y + vs.at(i).y;
		center.z = center.z + vs.at(i).z;

		center.x = center.x / n;
		center.y = center.y / n;
		center.z = center.z / n;
	}

	radius = 0;

	for (int i = 0; i < n - 1; i++)
	{
		dx = vs.at(i).x - center.x;
		dy = vs.at(i).y - center.y;
		dz = vs.at(i).z - center.z;

		lev.x = dx;
		lev.y = dy;
		lev.z = dz;
		lev = modelMatrix * lev;

		if (radius < glm::length(lev))
		{
			radius = glm::length(lev);
		}
	}

	originalCenter = center;
	center = glm::vec3(modelMatrix * glm::vec4(center, 1.0));
}