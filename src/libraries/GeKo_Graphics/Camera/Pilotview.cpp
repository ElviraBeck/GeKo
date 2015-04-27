#include "Pilotview.h"

Pilotview::Pilotview(std::string name)
{
	m_name = name;

	m_position = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);
	m_direction = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	m_theta = glm::pi<float>() / 2.0f;
	m_phi = glm::pi<float>() / 2.0f;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));
	m_projectionMatrix = glm::perspective(m_fov, m_width / (float)m_height, m_near, m_far);

	m_keySpeed = 50.0f;//2.0f;
	m_sensitivity = 0.01f;

	m_rotationAngle = 0;
}

Pilotview::~Pilotview()
{
}

void Pilotview::setPosition(glm::vec4 position){
	m_position = position;
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));
}

void Pilotview::moveForward(){
	m_position.x += m_keySpeed* m_sensitivity*m_direction.x;
	m_position.y += m_keySpeed* m_sensitivity*m_direction.y;
	m_position.z += m_keySpeed* m_sensitivity*m_direction.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveForward()" << std::endl;
}

void Pilotview::moveBackward(){
	m_position.x -= m_keySpeed* m_sensitivity*m_direction.x;
	m_position.y -= m_keySpeed* m_sensitivity*m_direction.y;
	m_position.z -= m_keySpeed* m_sensitivity*m_direction.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveBackward()" << std::endl;
}

void Pilotview::moveLeft(){
	glm::vec3 directionOrtho = glm::cross(glm::vec3(m_direction.x, m_direction.y, m_direction.z), glm::vec3(m_up.x, m_up.y, m_up.z));
	m_position.x -= m_keySpeed* m_sensitivity*directionOrtho.x;
	m_position.y -= m_keySpeed* m_sensitivity*directionOrtho.y;
	m_position.z -= m_keySpeed* m_sensitivity*directionOrtho.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));
	
	std::cout << "moveLeft()" << std::endl;
}
	
void Pilotview::moveRight(){
	glm::vec3 directionOrtho = glm::cross(glm::vec3(m_direction.x, m_direction.y, m_direction.z), glm::vec3(m_up.x, m_up.y, m_up.z));
	m_position.x += m_keySpeed* m_sensitivity*directionOrtho.x;
	m_position.y += m_keySpeed* m_sensitivity*directionOrtho.y;
	m_position.z += m_keySpeed* m_sensitivity*directionOrtho.z;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveRight()" << std::endl;
}

void Pilotview::moveUp(){
	m_position.y += m_keySpeed* m_sensitivity;
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveUp()" << std::endl;
}

void Pilotview::moveDown(){
	m_position.y -= m_keySpeed* m_sensitivity;
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "moveDown()" << std::endl;
}

void Pilotview::turnLeft(){
	m_rotationAngle = m_keySpeed* m_sensitivity;

	m_phi += m_keySpeed* m_sensitivity;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();

	m_direction.x = sin(m_theta) * cos(m_phi);
	m_direction.y = cos(m_theta);
	m_direction.z = -sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "turnLeft()" << std::endl;
}

void Pilotview::turnRight(){
	m_rotationAngle = -m_keySpeed* m_sensitivity;

	m_phi -= m_keySpeed* m_sensitivity;
	if (m_phi < 0) m_phi += 2 * glm::pi<float>();
	else if (m_phi > 2 * glm::pi<float>()) m_phi -= 2 * glm::pi<float>();

	m_direction.x = sin(m_theta) * cos(m_phi);
	m_direction.y = cos(m_theta);
	m_direction.z = -sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "turnRight()" << std::endl;
}

void Pilotview::turnUp(){
	m_theta -= m_keySpeed* m_sensitivity;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;
	
	m_direction.x = sin(m_theta) * cos(m_phi);
	m_direction.y = cos(m_theta);
	m_direction.z = -sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "turnUp()" << std::endl;
}

void Pilotview::turnDown(){
	m_theta += m_keySpeed* m_sensitivity;
	if (m_theta < 0.01f) m_theta = 0.01f;
	else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

	m_direction.x = sin(m_theta) * cos(m_phi);
	m_direction.y = cos(m_theta);
	m_direction.z = -sin(m_theta) * sin(m_phi);

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_position) + glm::vec3(m_direction), glm::vec3(m_up));

	std::cout << "turnDown()" << std::endl;
}