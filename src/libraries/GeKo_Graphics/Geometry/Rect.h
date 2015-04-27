#pragma once
#include <GeKo_Graphics/Geometry/Geometry.h>
#include <iostream>
class Rect : public Geometry
{
public:

	Rect();
	Rect(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);
	~Rect();

	void create(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d, glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);
	void setPoints(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d);
	void setTcoords(glm::vec2 tca, glm::vec2 tcb, glm::vec2 tcc, glm::vec2 tcd);

	///A Method to load Buffer
	/**The vertices Data from m_vertices will be loaded into Buffers, so the Shader can use this information for the position of the object*/
	void loadBufferData();

	///A method to render the Object 
	/**In the while-Loop of the main-programm (Renderer or else) this method will be called to draw the array*/
	void renderGeometry();
};