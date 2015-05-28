#include "Rect.h"

Rect::Rect()
{
	setNormalsTrue();
	setIndexTrue();
	setUVTrue();
	
	create(glm::vec4(1.0, -1.0, 0, 1.0), glm::vec4(-1.0, -1.0, 0, 1.0), glm::vec4(1.0, 1.0, 0, 1.0), glm::vec4(-1.0, 1.0, 0, 1.0),
	glm::vec2(1.0, 0.0), glm::vec2(0.0, 0.0), glm::vec2(1.0, 1.0), glm::vec2(0.0, 1.0));

}

Rect::Rect(glm::vec4 dright, glm::vec4 dleft, glm::vec4 uright, glm::vec4 uleft, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	create(dright, dleft, uright, uleft, tca, tcb, tcc, tcd);
}

Rect::~Rect()
{
	m_vertices.clear();
}

void Rect::create(glm::vec4 dright, glm::vec4 dleft, glm::vec4 uright, glm::vec4 uleft, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	m_vertices.push_back(glm::vec4(dright));
	m_vertices.push_back(glm::vec4(dleft));
	m_vertices.push_back(glm::vec4(uright));
	m_vertices.push_back(glm::vec4(uleft));

	m_points = 4;

	glm::vec3 n = glm::normalize(glm::cross(glm::vec3(dright - dleft), glm::vec3(uleft - dleft)));

	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);

	m_uvs.push_back(tca);
	m_uvs.push_back(tcb);
	m_uvs.push_back(tcc);
	m_uvs.push_back(tcd);

	m_index.push_back(0);
	m_index.push_back(1);
	m_index.push_back(2);

	m_index.push_back(2);
	m_index.push_back(3);
	m_index.push_back(1);

	m_indices = 6;
}

void Rect::setPoints(glm::vec4 dright, glm::vec4 dleft, glm::vec4 uright, glm::vec4 uleft)
{	
	m_vertices.clear();
	m_vertices.push_back(glm::vec4(dright));
	m_vertices.push_back(glm::vec4(dleft));
	m_vertices.push_back(glm::vec4(uright));
	m_vertices.push_back(glm::vec4(uleft));

	glm::vec3 n = glm::normalize(glm::cross(glm::vec3(dright - dleft), glm::vec3(uleft - dleft)));

	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
	m_normals.push_back(n);
}

void Rect::setTcoords(glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd)
{
	m_uvs.clear();
	m_uvs.push_back(tca);
	m_uvs.push_back(tcb);
	m_uvs.push_back(tcc);
	m_uvs.push_back(tcd);
}

void Rect::loadBufferData()
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

void Rect::renderGeometry()
{
	glBindVertexArray(m_vaoBuffer);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexBuffer->size);
}