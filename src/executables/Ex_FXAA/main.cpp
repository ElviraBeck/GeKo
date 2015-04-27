#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/Camera/Pilotview.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

bool USE_FXAA = true;

InputHandler iH;
Pilotview cam("Pilotview");

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();

	if (key == GLFW_KEY_F1)
		USE_FXAA = true;
	
	if (key == GLFW_KEY_F2)
		USE_FXAA = false;

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

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "FXAA");
	glfwMakeContextCurrent(testWindow.getWindow());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// You have to set a camera name
	cam.setName("PilotviewCam");
	cam.setPosition(glm::vec4(0.0, 0.3, 1.0, 1.0));
	cam.setNearFar(0.001f, 100.0f);

	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();

	//our shader
	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsFXAA(loadShaderSource(SHADERS_PATH + std::string("/FXAA/FXAA.vert")));
	FragmentShader fsFXAA(loadShaderSource(SHADERS_PATH + std::string("/FXAA/FXAA.frag")));
	ShaderProgram shaderFXAA(vsFXAA, fsFXAA);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);
	FBO fboFXAA(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);

	//our object
	Teapot teapot;
	teapot.loadBufferData();

	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();

	//our textures
	Texture chrome((char*)RESOURCES_PATH "/chrome.jpg");

	//Scene creation 
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add Camera to scenegraph
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->getCamera("PilotviewCam");
	testScene.getScenegraph()->setActiveCamera("PilotviewCam");


	Node teapotNode("teapot");
	teapotNode.addGeometry(&teapot);
	teapotNode.addTexture(&chrome);
	teapotNode.setModelMatrix(glm::scale(teapotNode.getModelMatrix(), glm::vec3(0.2, 0.2, 0.2)));



	//Creating a scenegraph
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&teapotNode);

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

		fboFXAA.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderFXAA.bind();

		shaderFXAA.sendSampler2D("colorTexture", fboGBuffer.getColorTexture(2), 2);
		shaderFXAA.sendInt("fboWidth", WINDOW_WIDTH);
		shaderFXAA.sendInt("fboHeight", WINDOW_HEIGHT);

		screenFillingQuad.renderGeometry();
		
		shaderFXAA.unbind();
		fboFXAA.unbind();
				
		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (USE_FXAA)
			shaderSFQ.sendSampler2D("fboTexture", fboFXAA.getColorTexture(2));
		else
			shaderSFQ.sendSampler2D("fboTexture", fboGBuffer.getColorTexture(2));

		screenFillingQuad.renderGeometry();
		shaderSFQ.unbind();
		
		
		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}