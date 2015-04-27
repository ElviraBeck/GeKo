#include "Window.h"

GLFWwindow* m_window;
const char* m_name;
int m_width;
int m_height;
int m_xpos;
int m_ypos;

/*
open a window with a certain size in a certain position and set the context to him
*/
Window::Window(int xpos, int ypos, int width, int height, const char* name, glm::vec3 color)
{
	this->m_name = name;
	this->m_width = width;
	this->m_height = height;
	this->m_xpos = xpos;
	this->m_ypos = ypos;
	this->m_color = color;
	
	m_window = glfwCreateWindow(width, height, name, NULL, NULL);
	glfwSetWindowPos(m_window, xpos, ypos);
	glfwMakeContextCurrent(m_window);
	glClearColor(m_color.x, m_color.y, m_color.z, 1.0f);
}

Window::~Window()
{
}

void Window::close(){
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::swapAndPoll(){
	glfwSwapBuffers(this->getWindow());
	glfwPollEvents();
}

GLFWwindow* Window::getWindow(){
	return m_window;
}

void Window::setName(const char* name)
{
	m_name = name;
}

const char* Window::getName()
{
	return m_name;
}

void Window::setWidth(int width)
{
	m_width = width;
}

int Window::getWidth()
{
	return m_width;
}

void Window::setHeight(int height)
{
	m_height = height;
}

int Window::getHeight()
{
	return m_height;
}

void Window::setXpos(int xpos)
{
	m_xpos = xpos;
}

int Window::getXpos()
{
	return m_xpos;
}

void Window::setYpos(int ypos)
{
	m_ypos = ypos;
}

int Window::getYpos()
{
	return m_ypos;
}

void Window::setColor(glm::vec3 color)
{
	m_color = color;
}

glm::vec3 Window::getColor()
{
	return m_color;
}