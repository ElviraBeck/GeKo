#include "Terrain.h"
#include <stb_image.h>


Terrain::Terrain(std::string filename, float resolution, float interval)
{
	// set member values
	m_interval = interval;
	m_resolution = resolution;

	// initialize the height field
	loadHeightmap(filename);

	for (int z = 1; z < m_resolutionY - 1; z++){
		for (int x = 1; x < m_resolutionX - 1; x++){
			m_normals.push_back(calculateNormal(x, z));
			m_vertices.push_back(glm::vec4(x, getHeight(glm::vec2(x, z)), z, 1.0));
			//m_uvs.push_back(glm::vec2(x / (float)m_resolutionX, z / (float)m_resolutionY));
			m_uvs.push_back(glm::vec2((float)x / 5, (float)z / 5));
		}
	}

	int offset = 0;
	for (int z = 0; z < m_resolutionY - 3; z++){
		for (int x = 0; x < m_resolutionX - 3; x++){
			// 1. Triangle
			m_index.push_back(offset + x);
			m_index.push_back(offset + x + 1);
			m_index.push_back(offset + x + (m_resolutionY - 2));

			// 2. Triangle
			m_index.push_back(offset + x + (m_resolutionY - 2));
			m_index.push_back(offset + x + 1);
			m_index.push_back(offset + x + (m_resolutionY - 2) + 1);
		}
		offset += (m_resolutionY - 2);
	}

	m_indices = m_index.size();
	m_points = m_vertices.size();
	setNormalsTrue();
	setIndexTrue();
	setUVTrue();

}

Terrain::~Terrain(){

}

void Terrain::loadHeightmap(std::string fileName) {
	int bytesPerPixel;
	unsigned char *image = stbi_load(fileName.c_str(), &m_resolutionX, &m_resolutionY, &bytesPerPixel, 1);

	int count = 0;
	m_heightMap = new float*[m_resolutionX];
	for (int i = 0; i < m_resolutionX; ++i) {
		m_heightMap[i] = new float[m_resolutionY];
		for (int j = 0; j < m_resolutionY; ++j) {
			m_heightMap[i][j] = (float)image[count++] / 512.0f * 25;
		}
	}
}


glm::vec3 Terrain::calculateNormal(int x, int z)
{
	glm::vec3 normal;

	normal.x = getHeight(glm::vec2(x - 1, z)) - getHeight(glm::vec2(x + 1, z));
	normal.y = 2.0f;
	normal.z = getHeight(glm::vec2(x, z - 1)) - getHeight(glm::vec2(x, z + 1));

	return normal;
}

float Terrain::getHeight(glm::vec2 p) {
	if (p.x < 0 || p.y < 0 || p.x >= m_resolutionX - 1 || p.y >= m_resolutionY - 1) {
		return 0.0f;
	}

	float x0y1 = m_heightMap[(int)p.x][(int)p.y + 1];
	float x0y0 = m_heightMap[(int)p.x][(int)p.y];
	float x1y1 = m_heightMap[(int)p.x + 1][(int)p.y + 1];
	float x1y0 = m_heightMap[(int)p.x + 1][(int)p.y];
	float dx = p.x - (int)p.x;
	float dy = p.y - (int)p.y;

	float interpolateX0 = x0y0 * (1 - dx) + x1y0 * dx;
	float interpolateX1 = x0y1 * (1 - dx) + x1y1 * dx;

	return interpolateX0 * (1 - dy) + interpolateX1 * dy;
}

float Terrain::getResolutionX()
{
	return m_resolutionX;
}

float Terrain::getResolutionY()
{
	return m_resolutionY;
}
