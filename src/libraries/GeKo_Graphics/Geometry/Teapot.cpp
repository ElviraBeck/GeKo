#include "Teapot.h"
#include "TeapotData.h"

Teapot::Teapot()
{
	m_points = _points;
	m_indices = _indices;
	setIndexTrue();
	setUVTrue();
	setNormalsTrue();

	for (int i = 0; i<_points; i++)
	{
		m_vertices.push_back(glm::vec4(_v[i * 3], _v[i * 3 + 1], _v[i * 3 + 2], 1.0));
		m_normals.push_back(glm::vec3(_n[i * 3], _n[i * 3 + 1], _n[i * 3 + 2]));
		m_uvs.push_back(glm::vec2(_t[i * 2], 1 - _t[i * 2 + 1]));
	}

	for (int i = 0; i<_indices; i++)
	{
		m_index.push_back(_index[i]);
	}
}

Teapot::~Teapot()
{
	m_vertices.clear();
	m_normals.clear();
	m_uvs.clear();
	m_index.clear();
}