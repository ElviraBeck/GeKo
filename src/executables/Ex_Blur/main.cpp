#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/Camera/Pilotview.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

InputHandler iH;
Trackball cam("Trackball");

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();

	for (std::map<int, std::function<void()>>::iterator it = activeMap.begin(); it != activeMap.end(); it++){
		if (it->first == key)
			activeMap.at(key)();
		if (it == activeMap.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}
}

int main()
{
	glfwInit();

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "Blur");
	glfwMakeContextCurrent(testWindow.getWindow());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	

	cam.setRadius(2.0);
	cam.setPosition(glm::vec4(0.0, 1.0, 1.0, 1.0));
	cam.setName("TrackballCam");
	cam.setNearFar(0.1f, 50.0f);
	cam.moveDown();

	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Trackball");

	//Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();

	//our shader
	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsBlur(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsBlur(loadShaderSource(SHADERS_PATH + std::string("/Blur/blur1D.frag")));
	ShaderProgram shaderBlur(vsBlur, fsBlur);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);
	FBO fboBlur(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);

	//our object
	Cube cube;
	Teapot teapot;

	Rect plane;
	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();

	//our textures
	Texture bricks((char*)RESOURCES_PATH "/brick.bmp");
	Texture marble((char*)RESOURCES_PATH "/seamless_marble.jpg");
	Texture chrome((char*)RESOURCES_PATH "/chrome.jpg");

	//Scene creation 
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add Camera to scenegraph
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->getCamera("TrackballCam");
	testScene.getScenegraph()->setActiveCamera("TrackballCam");

	Node cube1("cube1");
	cube1.addGeometry(&cube);
	cube1.addTexture(&bricks);
	cube1.setModelMatrix(glm::translate(cube1.getModelMatrix(), glm::vec3(-0.3, 0.25, 0.2)));
	cube1.setModelMatrix(glm::scale(cube1.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));

	Node cube2("cube2");
	cube2.addGeometry(&cube);
	cube2.addTexture(&bricks);
	cube2.setModelMatrix(glm::translate(cube2.getModelMatrix(), glm::vec3(0.7, 0.25, 0.3)));
	cube2.setModelMatrix(glm::scale(cube2.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));


	Node wallNode1("wall1");
	wallNode1.addGeometry(&plane);
	wallNode1.addTexture(&marble);
	wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(0.0, 0.1, 0.2)));
	wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), 90.0f, glm::vec3(1.0, 0.0, 0.0)));
	wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(1.5, 1.5, 1.5)));

	Node wallNode2("wall2");
	wallNode2.addGeometry(&plane);
	wallNode2.addTexture(&marble);
	wallNode2.setModelMatrix(glm::translate(wallNode2.getModelMatrix(), glm::vec3(0.0, 1.0, -0.2)));
	wallNode2.setModelMatrix(glm::scale(wallNode2.getModelMatrix(), glm::vec3(1.5, 1.5, 1.5)));


	Node teaNode("teaNode");
	teaNode.addGeometry(&teapot);
	teaNode.addTexture(&chrome);
	teaNode.setModelMatrix(glm::translate(teaNode.getModelMatrix(), glm::vec3(0.2, 0.3, 0.7)));
	teaNode.setModelMatrix(glm::scale(teaNode.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));


	//Creating a scenegraph
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&teaNode);

	double startTime = glfwGetTime();
	//Renderloop
	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{
		// You have to compute the delta time
		cam.setSensitivity(glfwGetTime() - startTime);
		
		startTime = glfwGetTime();

		fboGBuffer.bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderGBuffer.bind();
		shaderGBuffer.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderGBuffer.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		shaderGBuffer.sendInt("useTexture", 1);
		testScene.render(shaderGBuffer);
		shaderGBuffer.unbind();
		fboGBuffer.unbind();	

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderBlur.bind();
		fboBlur.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderBlur.sendSampler2D("image", fboGBuffer.getColorTexture(2), 2);
		screenFillingQuad.renderGeometry();
		fboBlur.unbind();

		shaderBlur.unbind();
				
		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderSFQ.sendSampler2D("fboTexture", fboBlur.getColorTexture(2), 2);
		screenFillingQuad.renderGeometry();
		shaderSFQ.unbind();
		
		
		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}