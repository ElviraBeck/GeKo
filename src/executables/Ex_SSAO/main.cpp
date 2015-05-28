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

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "SSAO");
  
	glfwMakeContextCurrent(testWindow.getWindow());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
  cam.setRadius(2.0);
  cam.setPosition(glm::vec4(0.0, 1.0, 1.0, 1.0));
  cam.setName("TrackballCam");
  cam.setNearFar(0.1f, 1000.0f);
  cam.moveDown();

	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();

	//our shader
	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsSSAO(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssao.vert")));
	FragmentShader fsSSAO(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssao.frag")));
  ShaderProgram shaderSSAO(vsSSAO, fsSSAO);

  VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
  FragmentShader fsBLUR(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssaoBlur.frag")));
  ShaderProgram shaderBLUR(vsSfq, fsBLUR);

	FragmentShader fsFinal(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssaoFinal.frag")));
  ShaderProgram shaderFinal(vsSfq, fsFinal);

  FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
  ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);
  FBO fboSSAO(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);
  FBO fboSSAOblurred(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);
  FBO fboXYblurred(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);
  FBO fboXYFinal(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);

	//our object
	Teapot teapot;
	teapot.loadBufferData();

  Sphere sphere;
  sphere.loadBufferData();

  Rect plane;
	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();

	//our textures
	Texture chrome((char*)RESOURCES_PATH "/chrome.jpg");
  Texture cvLogo((char*)RESOURCES_PATH "/cv_logo.bmp");
  Texture bricks((char*)RESOURCES_PATH "/brick.bmp");

	//Scene creation 
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

  //Add Camera to scenegraph
  testScene.getScenegraph()->addCamera(&cam);
  testScene.getScenegraph()->getCamera("TrackballCam");
  testScene.getScenegraph()->setActiveCamera("TrackballCam");



  for (int i = -5; i < 5; i++)
    for (int j = -5; j < 5; j++)
    {
      Node *teapotNode = new Node("teapot"+std::to_string(i)+std::to_string(j));
      teapotNode->addGeometry(&teapot);
      teapotNode->addTexture(&bricks);
      teapotNode->setModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0.2)));
      teapotNode->setModelMatrix(glm::translate(teapotNode->getModelMatrix(), glm::vec3(i*2, 1.1, j*2)));
	    testScene.getScenegraph()->getRootNode()->addChildrenNode(teapotNode);
    }

    Node wallNode1("wall1");
    wallNode1.addGeometry(&plane);
    wallNode1.addTexture(&cvLogo);
    wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(-0.2, 0.0, -0.5)));
    wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), -90.0f, glm::vec3(1.0, 0.0, 0.0)));
    wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(2.5,2.5,2.5)));

    testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);

	double startTime = glfwGetTime();
	//Renderloop


	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{
    // You have to compute the delta time

    float deltaTime = glfwGetTime() - startTime;
    cam.setSensitivity(deltaTime);

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

		fboSSAO.bind();
    glClearColor(1, 1, 1, 1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderSSAO.bind();

    shaderSSAO.sendSampler2D("positionMap", fboGBuffer.getColorTexture(0), 0);
    shaderSSAO.sendSampler2D("normalMap", fboGBuffer.getColorTexture(1), 1);
    shaderSSAO.sendMat4("sceneProjectionMatrix", testScene.getScenegraph()->getActiveCamera()->getProjectionMatrix());
    shaderSSAO.sendFloat("radius", 0.1f);
    shaderSSAO.sendFloat("quality", 30.0f);
    
		screenFillingQuad.renderGeometry();
		
    shaderSSAO.unbind();
		fboSSAO.unbind();


    //Blur #1 SSAO calculation
    fboSSAOblurred.bind();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderBLUR.bind();
    shaderBLUR.sendSampler2D("colortexture", fboSSAO.getColorTexture(2), 0);
    shaderBLUR.sendInt("secondPass", 0);
    screenFillingQuad.renderGeometry();

    shaderBLUR.unbind();
    fboSSAOblurred.unbind();
    
    //Blur #2 SSAO calculation
    fboXYblurred.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderBLUR.bind();
    shaderBLUR.sendSampler2D("colortexture", fboSSAOblurred.getColorTexture(2), 0);
    shaderBLUR.sendFloat("secondPass",1);
    screenFillingQuad.renderGeometry();

    shaderBLUR.unbind();
    fboXYblurred.unbind();

    glClearColor(0, 0, 0, 0);

    //COLOR COMPOSITING
    fboXYFinal.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderFinal.bind();
    shaderFinal.sendSampler2D("colorMap", fboGBuffer.getColorTexture(2), 0);
    shaderFinal.sendSampler2D("ssaoMap", fboXYblurred.getColorTexture(2), 1);
    screenFillingQuad.renderGeometry();
    shaderFinal.unbind();
    fboXYFinal.unbind();


		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (glfwGetKey(testWindow.getWindow(),GLFW_KEY_F1))
      shaderSFQ.sendSampler2D("fboTexture", fboGBuffer.getColorTexture(2));
    else
      shaderSFQ.sendSampler2D("fboTexture", fboXYFinal.getColorTexture(2));

		screenFillingQuad.renderGeometry();
		shaderSFQ.unbind();
		
		
		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}