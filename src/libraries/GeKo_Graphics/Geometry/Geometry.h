#pragma once
#include <glm/glm.hpp>
#include "GeKo_Graphics/StaticMesh.h"
#include<vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GeKo_Graphics/Buffer.h>
#include <GeKo_Graphics/BufferIndex.h>
#include <Geko_Resource/Handle/Handle.hpp>

/*This class should be used as a interface class. It provides two Methods to load Data and to render the Vertice Data. 
  Every Geometry Object which will inherit this class should use this methods. A vector for the vertices is provided as well.*/
using Normal = glm::vec3;
using Vertex = glm::vec4;
using Uv = glm::vec2;
using Index = GLuint;

class Geometry;
struct StaticMesh{
  std::vector<Vertex> vertices;
  std::vector<Normal> normals;
  std::vector<Uv> uvs;
  std::vector<Index> indicies;
  Geometry toGeometry();
};
class Geometry
{
public:
	///Standard constructor, sets the m_wasLoaded boolean to false
	/**/
	Geometry();
  Geometry(const StaticMesh &mesh);
  ~Geometry();

	///A Method to load Buffer
	/**The vertices Data from m_vertices will be loaded into Buffers, so the Shader can use this information for the position of the object*/
	 void loadBufferData();

	///A method to render the Object 
	/**In the while-Loop of the main-programm (Renderer or else) this method will be called to draw the array*/
    void renderGeometry();

	///A method to compute the tangents for each triangle
	/**Here we compute the tangens of each triangle to use them in a normalMapping Shader. This isn't a performance optimized solution but it works.*/
	void computeTangents();

	///Returns m_vertices of the Geometry Object
	/**/
	std::vector<glm::vec4> getVertices();

	///Returns m_normals of the Geometry Object
	/**/
	std::vector<glm::vec3> getNormals();

	///Returns m_uvs of the Geometry Object
	/**/
	std::vector<glm::vec2> getUV();

	///Returns m_tangents of the Geometry Object
	/**/
	std::vector<glm::vec3> getTangents();

	///Returns m_index of the Geometry Object
	/**/
	std::vector<GLuint> getIndexList();
	
	///Sets m_wasLoaded to true
	/**/
	void setLoaded();

	///Sets m_wasLoaded to false
	/**/
	void resetLoaded();

	///Returns the boolean of m_wasLoaded
	/**/
	bool isLoaded();
	
	///If the object uses an index-List, this method has to be used 
	/**Set m_hasIndex to true*/
	void setIndexTrue();
	
	///If the object does not an index-List, this method has to be used 
	/**Set m_hasIndex to false*/
	void setIndexFalse();

	///Returns m_hasIndex
	/**/
	bool hasIndex();

	///If the Geometry has Normals, m_hasNormals will be set to true
	/**/
	void setNormalsTrue();

	///If the Geometry has UVs, m_hasUVs will be set to true
	/**/
	void setUVTrue();

protected:
	int m_points;
	std::vector<glm::vec4> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_tangents;

	std::vector<GLuint> m_index;
	int m_indices;

	GLuint m_vaoBuffer;
	Buffer<glm::vec4>* m_vertexBuffer;
	Buffer<glm::vec3>* m_normalBuffer;
	Buffer<glm::vec2>* m_uvBuffer;
	Buffer<glm::vec3>* m_tangentBuffer;
	BufferIndex<GLuint>* m_indexBuffer;

private:
	bool m_wasLoaded;
	bool m_hasIndex;
	bool m_hasNormals;
	bool m_hasUV;
};
