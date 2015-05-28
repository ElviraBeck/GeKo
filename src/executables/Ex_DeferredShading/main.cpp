#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/Camera/Pilotview.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

bool showlightTexture = false;

InputHandler iH;
Pilotview cam("Pilotview");

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

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "Deferred Shading");
	glfwMakeContextCurrent(testWindow.getWindow());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	

  // You have to set a camera name
  cam.setName("PilotviewCam");
  cam.setPosition(glm::vec4(0.0, 0.5, 3.0, 1.0));
  cam.setNearFar(0.01f, 100.0f);

	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();

	//our shader
	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

  //load shader here
  VertexShader vsDsLighting(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsLighting.vert")));
  FragmentShader fsDsLighting(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsLighting.frag")));
  ShaderProgram shaderDsLightingShader(vsDsLighting, fsDsLighting);

  VertexShader vsDsCompositing(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsFinalCompositing.vert")));
  FragmentShader fsDsCompositing(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsFinalCompositing.frag")));
  ShaderProgram shaderDsCompositingShader(vsDsCompositing, fsDsCompositing);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);
  FBO fboDeferredShading(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);
  FBO fboCompositing(WINDOW_WIDTH, WINDOW_HEIGHT, 3, false, false);

	//our object
	Cube cube;

	Teapot teapot;

	Rect plane;
	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();

	//our textures
	Texture bricks((char*)RESOURCES_PATH "/bricks_diffuse.png");
  Texture bricks_normal((char*)RESOURCES_PATH "/bricks_normal.png");
  Texture bricks_height((char*)RESOURCES_PATH "/bricks_height.png");

	Texture chrome((char*)RESOURCES_PATH "/chrome.jpg");
  Texture cvLogo((char*)RESOURCES_PATH "/cv_logo.bmp");

	//Scene creation 
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

  //Add Camera to scenegraph
  testScene.getScenegraph()->addCamera(&cam);
  testScene.getScenegraph()->getCamera("PilotviewCam");
  testScene.getScenegraph()->setActiveCamera("PilotviewCam");
  
  Rect rect;

	Node cube1("cube1");
	cube1.addGeometry(&cube);
  cube1.addTexture(&bricks);
  cube1.addNormalMap(&bricks_normal);
  cube1.addHeightMap(&bricks_height);
  cube1.setModelMatrix(glm::translate(cube1.getModelMatrix(), glm::vec3(-1.0, 0.5, -0.5)));
	cube1.setModelMatrix(glm::scale(cube1.getModelMatrix(), glm::vec3(0.7, 0.7, 0.7)));

	Node cube2("cube2");
  cube2.addGeometry(&cube);
	cube2.addTexture(&bricks);
  cube2.addNormalMap(&bricks_normal);
	cube2.setModelMatrix(glm::translate(cube2.getModelMatrix(), glm::vec3(-1, 0.5, 0.5)));
  cube2.setModelMatrix(glm::scale(cube2.getModelMatrix(), glm::vec3(0.7, 0.7, 0.7)));
  
  Node cube3("cube3");
  cube3.addGeometry(&cube);
  cube3.addTexture(&bricks);
  cube3.setModelMatrix(glm::translate(cube3.getModelMatrix(), glm::vec3(0, 0.5, -0.5)));
  cube3.setModelMatrix(glm::scale(cube3.getModelMatrix(), glm::vec3(0.7, 0.7, 0.7)));

  Node cube4("cube4");
  cube4.addGeometry(&cube);
  cube4.addTexture(&bricks);
  cube4.addNormalMap(&bricks_normal);
  cube4.addHeightMap(&bricks_height,0.07,0.1,true);
  cube4.setModelMatrix(glm::translate(cube4.getModelMatrix(), glm::vec3(0, 0.5, 0.5)));
  cube4.setModelMatrix(glm::scale(cube4.getModelMatrix(), glm::vec3(0.7, 0.7, 0.7)));

	Node wallNode1("wall1");
	wallNode1.addGeometry(&plane);
  wallNode1.addTexture(&cvLogo);
	wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(0.0, 0.1, 0.2)));
	wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), 90.0f, glm::vec3(1.0, 0.0, 0.0)));
	wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(10.5, 10.5, 10.5)));


	Node teaNode("teaNode");
	teaNode.addGeometry(&teapot);
	teaNode.addTexture(&chrome);
	teaNode.setModelMatrix(glm::translate(teaNode.getModelMatrix(), glm::vec3(0.2, 0.4, 0.7)));
	teaNode.setModelMatrix(glm::scale(teaNode.getModelMatrix(), glm::vec3(0.5, 0.5, 0.5)));


	//Creating a scenegraph
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube1);
  testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube2);
  testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube3);
  testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube4);
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&teaNode);

	double startTime = glfwGetTime();
	//Renderloop

  //create Light spheres for DS
  Node lights = Node("Root");
  Sphere lightSphere = Sphere();

  for (int i = -4; i < 4; i++)
     for (int j = -4; j < 4;j++)
     {
       Node *newLight = new Node(std::string("Node_"+std::to_string(i)+std::to_string(j)));
       newLight->addGeometry(&lightSphere);
       newLight->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(i*1.5, 1.0f, j*1.5)));
       //newLight.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 1.0f)));
       newLight->setModelMatrix(glm::scale(newLight->getModelMatrix(), glm::vec3(2.0, 2.0, 2.0)));
       lights.addChildrenNode(newLight);
     }
    
  int outputFPS = 0;
    
	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{
		// You have to compute the delta time
		
    float deltaTime = glfwGetTime() - startTime;
    cam.setSensitivity(deltaTime);
		
    //if (!(outputFPS % 20))
      //std::cout << "FPS: " << static_cast<int>(1 / (glfwGetTime() - startTime)) << std::endl;

    std::cout << "FPS: " << static_cast<double>(glfwGetTime() - startTime) * 100 << std::endl;


    outputFPS++;
		startTime = glfwGetTime();

    //update Model Matrix
    lights.setModelMatrix(glm::rotate(lights.getModelMatrix(), 10.0f * deltaTime, glm::vec3(0.0, 1.0, 0.0)));


		fboGBuffer.bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderGBuffer.bind();
		shaderGBuffer.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderGBuffer.sendMat4("projectionMatrix", cam.getProjectionMatrix());
   
		testScene.render(shaderGBuffer);

		
    shaderGBuffer.unbind();
		fboGBuffer.unbind();

    //DEFERRED SHADING TEIL============================

    fboDeferredShading.bind();

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glClearColor(0, 0, 0, 0); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderDsLightingShader.bind();
    
    shaderDsLightingShader.sendMat4("viewMatrix", cam.getViewMatrix());
    shaderDsLightingShader.sendMat4("projectionMatrix", cam.getProjectionMatrix());
    
    shaderDsLightingShader.sendSampler2D("positionMap", fboGBuffer.getColorTexture(0),0);
    shaderDsLightingShader.sendSampler2D("normalMap", fboGBuffer.getColorTexture(1),1);

    shaderDsLightingShader.sendInt("windowWidth", testWindow.getWidth());
    shaderDsLightingShader.sendInt("windowHeight", testWindow.getHeight());

    shaderDsLightingShader.sendVec3("lightColor", glm::fvec3(0.7f,0.7f,0.4f));
        
    lights.render(shaderDsLightingShader);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    shaderDsLightingShader.unbind();
    fboDeferredShading.unbind();
    
    //COMPOSITING TEIL ===============================
    fboCompositing.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderDsCompositingShader.bind();

    shaderDsCompositingShader.sendSampler2D("colorMap", fboGBuffer.getColorTexture(2),0);
    shaderDsCompositingShader.sendSampler2D("lightMap", fboDeferredShading.getColorTexture(2),1);

    screenFillingQuad.renderGeometry();

    shaderDsCompositingShader.unbind();
    fboCompositing.unbind();

    //================================================
				
		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (glfwGetKey(testWindow.getWindow(), GLFW_KEY_F1))
      shaderSFQ.sendSampler2D("fboTexture", fboDeferredShading.getColorTexture(2));
    else
      shaderSFQ.sendSampler2D("fboTexture", fboCompositing.getColorTexture(2));

		screenFillingQuad.renderGeometry();
		shaderSFQ.unbind();
		
		
		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}