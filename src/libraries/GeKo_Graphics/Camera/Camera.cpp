#include "Camera.h"

Camera::Camera(std::string name){
	m_name = name;

	m_position = glm::vec4(0.0f, 0.0f, 5.0f, 1.0f);
	m_center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

	m_fov = 60.0f;
	m_near = 0.01f;
	m_far = 10.0f;

	m_width = 800;
	m_height = 600;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
	m_projectionMatrix = glm::perspective(m_fov, m_width / (float)m_height, m_near, m_far);

	m_rotationAngle = 0;
}

Camera::~Camera(){
}

void Camera::update(GLFWwindow* window)
{
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}

void Camera::setFOV(float fov){
	m_fov = fov;
	m_projectionMatrix = glm::perspective(m_fov, (float)m_width / (float)m_height, m_near, m_far);
}

void Camera::setLookAt(glm::vec3 lookAt){
	m_center = glm::vec4(lookAt, 1.0);
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), glm::vec3(m_center), glm::vec3(m_up));
}


glm::mat4 Camera::getViewMatrix(){
	return m_viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix(){
	return m_projectionMatrix;
}

void Camera::setNearFar(float near, float far){
	m_near = near;
	m_far = far;
	m_projectionMatrix = glm::perspective(m_fov, (float)m_width / (float)m_height, m_near, m_far);
}

float Camera::getNear(){
	return m_near;
}

float Camera::getFar(){
	return m_far;
}

void Camera::setWidthHeight(int width, int height){
	m_width = width;
	m_height = height;
}

void Camera::setPosition(glm::vec4 pos){
	m_position = pos;
}

glm::vec4 Camera::getPosition(){
	return m_position;
}

std::string Camera::getName(){
	return m_name;
}

void Camera::setName(std::string n){
	m_name = n;
}

float Camera::getSensitivity(){
	return m_sensitivity;
}

void Camera::setSensitivity(float sensitivity){
	m_sensitivity = sensitivity;
}

float Camera::getKeySpeed(){
	return m_keySpeed;
}

void Camera::setKeySpeed(float speed){
	m_keySpeed = speed;
}

float Camera::getMouseSpeed(){
	return m_mouseSpeed;
}

void Camera::setMouseSpeed(float speed){
	m_mouseSpeed = speed;
}

glm::vec4 Camera::getCenter(){
	return m_center;
}

void Camera::setCenter(glm::vec4 c){
	m_center = c;
}

float Camera::getPhi(){
	return m_phi;
}

float Camera::getTheta(){
	return m_theta;
}

double Camera::getRotationAngle()
{
	return m_rotationAngle;
}

void Camera::moveForward(){}
void Camera::moveBackward(){}
void Camera::moveLeft(){}
void Camera::moveRight(){}
void Camera::moveUp(){}
void Camera::moveDown(){}
void Camera::turnLeft(){}
void Camera::turnRight(){}
void Camera::turnUp(){}
void Camera::turnDown(){}
//void Camera::turn(GLFWwindow* window){}
void Camera::turn(double xpos, double ypos){}
void Camera::increaseRadius(){}
void Camera::decreaseRadius(){}
void Camera::updateCursor(GLFWwindow* window){}