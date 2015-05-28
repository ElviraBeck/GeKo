#include <GeKo_Graphics/Geometry/Plane.h>

Plane::Plane()
{
	setIndexTrue();
	setNormalsTrue();
	setUVTrue();

	m_vertices.push_back(glm::vec4(-1, 1, 0, 1.0f));
	m_vertices.push_back(glm::vec4(-1, -1, 0, 1.0f));
	m_vertices.push_back(glm::vec4(1, -1, 0, 1.0f));
	m_vertices.push_back(glm::vec4(1, 1, 0, 1.0f));

	m_points = 4;

	glm::vec3 n = glm::normalize(glm::cross(glm::vec3(-1, -1, 0) - glm::vec3(-1, 1, 0), glm::vec3(1, -1, 0) - glm::vec3(-1, 1, 0)));

	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);

	m_uvs.push_back(glm::vec2(0, 1));
	m_uvs.push_back(glm::vec2(0, 0));
	m_uvs.push_back(glm::vec2(1,0));
	m_uvs.push_back(glm::vec2(1, 1));

	m_indices = 6;

	m_index.push_back(0);
	m_index.push_back(1);
	m_index.push_back(2);

	m_index.push_back(0);
	m_index.push_back(2);
	m_index.push_back(3);

}

Plane::~Plane()
{
	m_vertices.clear();
	m_normals.clear();
	m_uvs.clear();
	m_index.clear();
}