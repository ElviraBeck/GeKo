#pragma once
#include "GeKo_Graphics/include.h"
#include "GeKo_Graphics/Defs.h"
#include "GeKo_Graphics/Renderer/Renderer.h"
#include "GeKo_Graphics/Window.h"
#include "GeKo_Graphics/Light/PointLight.h"
#include "GeKo_Graphics/Light/ConeLight.h"
#include "GeKo_Graphics/Light/DirectionLight.h"
#include "GeKo_Graphics/Shader/Shader.h"
#include "GeKo_Graphics/Buffer.h"
#include "GeKo_Graphics/Shader/FBO.h"
#include "GeKo_Graphics/Geometry/Rect.h"
#include "GeKo_Graphics/Geometry/Teapot.h"
#include "GeKo_Graphics/Geometry/Cube.h"

/*
if you want to use lights, you just must create them with PointLight, DirectionLight or ConeLight class
and send all needed information to the shader.
You can also set them to the shader.
*/
int main()
{
    glfwInit();

    //our window
    Window window(550, 50, 800, 600, "Light Example");

    glewInit();

	//our shader
	VertexShader vsColor(loadShaderSource(SHADERS_PATH + std::string("/PhongPointLight/PhongPointLight.vert"))); //Specular Bug!!
	FragmentShader fsColor(loadShaderSource(SHADERS_PATH + std::string("/PhongPointLight/PhongPointLight.frag")));
	ShaderProgram shaderPhong(vsColor, fsColor);

	VertexShader vsColorPhong(loadShaderSource(SHADERS_PATH + std::string("/PhongGeneral/PhongGeneral.vert"))); //Specular Bug!!
	FragmentShader fsColorPhong(loadShaderSource(SHADERS_PATH + std::string("/PhongGeneral/PhongGeneral.frag")));
	ShaderProgram shaderPhongGeneral(vsColorPhong, fsColorPhong);
    
	//our renderer
    OpenGL3Context context;
    Renderer renderer(context);

	//our object
	Rect* rect = new Rect(glm::vec4(0.5, -0.5, 0, 1.0), glm::vec4(-0.5, -0.5, 0, 1.0), glm::vec4(0.5, 0.5, 0, 1.0), glm::vec4(-0.5, 0.5, 0, 1.0),
		glm::vec2(0.75, 0.25), glm::vec2(0.25, 0.25), glm::vec2(0.75, 0.75), glm::vec2(0.25, 0.75));
	rect->loadBufferData();

	//our light
	PointLight plight(glm::vec4(0.0, 0.0, 0.1, 1.0), glm::vec4(1.0, 1.0, 1.0, 1.0), true, 0); //TODO radius & mehrere Lichter uebergeben

    //Gameloop
    while (!glfwWindowShouldClose(window.getWindow()))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderPhongGeneral.bind();
		
		shaderPhongGeneral.sendLightData("l_pos", "l_col", "l_dir", "l_exp", "l_ang", "l_rad", plight);
		glm::mat4 m(1.0);
		shaderPhongGeneral.sendMat4("modelMatrix", m);
		shaderPhongGeneral.sendMat4("viewMatrix", m);
		shaderPhongGeneral.sendMat4("projectionMatrix", m);
		shaderPhongGeneral.sendFloat("shininess", 2.0f);

		shaderPhongGeneral.sendVec4("l_ambient", glm::vec4(0.1, 0.1, 0.1, 1.0));
		shaderPhongGeneral.sendVec4("l_diffuse", glm::vec4(1.0, 0.0, 0.0, 1.0));
		shaderPhongGeneral.sendVec4("l_specular", glm::vec4(1.0, 1.0, 1.0, 1.0));

		rect->renderGeometry();
		shaderPhongGeneral.unbind();

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    window.close();
    return 0;
}