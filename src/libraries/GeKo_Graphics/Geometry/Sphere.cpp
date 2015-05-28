#include "Sphere.h"
#include <glm/gtc/constants.hpp>
Sphere::Sphere()
{
	setNormalsTrue();
	setIndexTrue();
	setUVTrue();
	
  create(1.0f,20);
}

Sphere::Sphere(float radius, int resolution)
{
  create(radius, resolution);
}

Sphere::~Sphere()
{
	m_vertices.clear();
}

void Sphere::create(float radius, int resolution)
{
  // iniatialize the variable we are going to use
  float u, v;
  float phi, theta;
  float x, y, z;
  int offset = 0, i, j;

  // create points
  for (j = 0; j <= resolution; j++)  //theta
  for (i = 0; i <= resolution; i++) //phi
  {
    u = i / (float)resolution;		phi = 2 * glm::pi<float>() * u;
    v = j / (float)resolution;		theta = glm::pi<float>() * v;

    x = radius * sin(theta) * cos(phi);
    y = radius * cos(theta);
    z = -radius * sin(theta) * sin(phi);

    m_vertices.push_back(glm::vec4(x, y, z, 1.0f));
    m_normals.push_back(glm::vec3(x, y, z) / radius);
    m_uvs.push_back(glm::vec2(u, 1 - v));
  }
  m_points = m_vertices.size();

  // create index list
  for (j = 0; j < resolution; j++)
  {
    for (i = 0; i < resolution; i++)
    {
      // 1. Triangle
      m_index.push_back(offset + i);
      m_index.push_back(offset + i + resolution + 1);
      m_index.push_back(offset + i + 1);

      // 2. Triangle
      m_index.push_back(offset + i + resolution + 1);
      m_index.push_back(offset + i + resolution + 1 + 1);
      m_index.push_back(offset + i + 1);
    }
    offset += resolution + 1;
  }
  m_indices = m_index.size();

}

void Sphere::loadBufferData()
{
	m_vertexBuffer = new Buffer<glm::vec4>(m_vertices, STATIC_DRAW);
	m_normalBuffer = new Buffer<glm::vec3>(m_normals, STATIC_DRAW);
	m_uvBuffer = new Buffer<glm::vec2>(m_uvs, STATIC_DRAW);

	glGenVertexArrays(1, &m_vaoBuffer);
	glBindVertexArray(m_vaoBuffer);
	m_vertexBuffer->bind();
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	m_normalBuffer->bind();
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_uvBuffer->bind();
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Sphere::renderGeometry()
{
	glBindVertexArray(m_vaoBuffer);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexBuffer->size);
}