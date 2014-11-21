#pragma once
#include <GeKo_Graphics/Defs.h>
#include <GeKo_Graphics/Shader/Shader.h>

class Light
{
public:
	Light();
	Light(glm::vec4 position, glm::vec3 color, bool isActive);
	~Light();
	void sendUniform();

	glm::vec3 m_color;
	glm::vec4 m_position;
	bool m_isActive;
};