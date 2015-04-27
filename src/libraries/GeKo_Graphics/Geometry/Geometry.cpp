#include "Geometry.h"


Geometry::Geometry()
{
	m_wasLoaded = false;
	m_hasNormals = false;
	m_hasIndex = false;
	m_hasUV = false;
}
Geometry::Geometry(const StaticMesh &mesh){
  m_wasLoaded = false;
  m_hasNormals = true;
  m_hasIndex = true;
  m_hasUV = true;

  m_vertices = mesh.vertices;
  m_index = mesh.indicies;
  m_uvs = mesh.uvs;
  m_normals = mesh.normals;
}

Geometry::~Geometry()
{
}

void Geometry::loadBufferData()
{
	m_vertexBuffer = new Buffer<glm::vec4>(m_vertices, STATIC_DRAW);
	if (m_hasNormals){
		m_normalBuffer = new Buffer<glm::vec3>(m_normals, STATIC_DRAW);
	}
	if (m_hasUV){
		m_uvBuffer = new Buffer<glm::vec2>(m_uvs, STATIC_DRAW);
	}
	if (m_hasIndex)
	{
		m_indexBuffer = new BufferIndex<GLuint>(m_index, STATIC_DRAW_INDEX);
	}

	glGenVertexArrays(1, &m_vaoBuffer);
	glBindVertexArray(m_vaoBuffer);

	m_vertexBuffer->bind();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	if (m_hasNormals){
		m_normalBuffer->bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
	}

	if (m_hasUV){
		m_uvBuffer->bind();
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	if (m_hasIndex){
		m_indexBuffer->bind();
	}

	glBindVertexArray(0);
}

void Geometry::renderGeometry()
{
	glBindVertexArray(m_vaoBuffer);

	if (m_hasIndex)
	{
		glDrawElements(GL_TRIANGLES, m_index.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices.size());
	}

	glBindVertexArray(0);

}

void Geometry::computeTangents()
{
	m_tangents.resize(m_vertices.size());

	for (int i = 0; i <m_indices-4; i += 3)
	{
		// Edges of the triangle : postion delta
		int index_v0 = m_index[i];
		int index_v1 = m_index[i + 1];
		int index_v2 = m_index[i + 2];

		glm::vec4 deltaPos1 = m_vertices[index_v1] - m_vertices[index_v0];
		glm::vec4 deltaPos2 = m_vertices[index_v2] - m_vertices[index_v0];

		// UV delta
		glm::vec2 deltaUV1 = m_uvs[index_v1] - m_uvs[index_v0];
		glm::vec2 deltaUV2 = m_uvs[index_v2] - m_uvs[index_v0];

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec4 tmp = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 tangent = glm::vec3(tmp.x, tmp.y, tmp.z);


		// Set the same tangent for all three vertices of the triangle
		m_tangents[index_v0] = tangent;
		m_tangents[index_v1] = tangent;
		m_tangents[index_v2] = tangent;
	}
}

std::vector<glm::vec4> Geometry::getVertices()
{
	return m_vertices;
}

std::vector<glm::vec3> Geometry::getNormals()
{
	return m_normals;
}

std::vector<glm::vec2> Geometry::getUV()
{
	return m_uvs;
}

std::vector<glm::vec3> Geometry::getTangents()
{
	return m_tangents;
}

std::vector<GLuint> Geometry::getIndexList()
{
	return m_index;
}

void Geometry::setLoaded()
{
	m_wasLoaded = true;
}

void Geometry::resetLoaded()
{
	m_wasLoaded = false;
}

bool Geometry::isLoaded()
{
	return m_wasLoaded;
}

void Geometry::setIndexTrue()
{
	m_hasIndex = true;
}

void Geometry::setIndexFalse()
{
	m_hasIndex = false;
}

bool Geometry::hasIndex()
{
	return m_hasIndex;
}

void Geometry::setNormalsTrue()
{
	m_hasNormals = true;
}

void Geometry::setUVTrue()
{
	m_hasUV = true;
}

Geometry StaticMesh::toGeometry(){
  return Geometry(*this);
}
