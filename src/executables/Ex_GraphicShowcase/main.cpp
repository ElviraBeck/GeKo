#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/Camera/Pilotview.h"
#include "GeKo_Graphics/GUI/GUI.h"
#include "GeKo_Graphics/Geometry/Mesh.h"


const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

InputHandler iH;
Pilotview cam("Pilotview");


glm::fvec3 *dsLightColor;
float *ssaoQuality;
float *ssaoRadius;
float *reflectionStrength;
float *blurStrength;
float *radialBlurStrength;
float *focusDepth;
float *bloomStrength;
int *useShadowMode;
GUI *gui;

GuiElement::Checkbox *useShadowMappingButton;
GuiElement::Checkbox *useLinearButton;
GuiElement::Checkbox *usePCFButton;
GuiElement::Checkbox *usePCFLinearButton;
GuiElement::Checkbox *useDeferredShadingButton;
GuiElement::Checkbox *deferredShadingRotationButton;
GuiElement::Checkbox *useSSAOButton;
GuiElement::Checkbox *useReflectionsButton;
GuiElement::Checkbox *useBloomButton;
GuiElement::Checkbox *useFXAAButton;
GuiElement::Checkbox *useDepthOfFieldButton;
GuiElement::Checkbox *useBlurButton;
GuiElement::Checkbox *useRadialBlurButton;

//our renderer
OpenGL3Context context;
Renderer *renderer;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();

	for (std::map<int, std::function<void()>>::iterator it = activeMap.begin(); it != activeMap.end(); it++){
		if (it->first == key)
			activeMap.at(key)();
		if (it == activeMap.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}

  if (glfwGetKey(window, GLFW_KEY_F7))
    if (gui->visible()) gui->hide(); else gui->show();
}


void initGUI()
{
  //INIT GUI
  gui = new GUI("Settings - Hide [F7]", 400, 400);

  GuiElement::Header *shadowMappingHeader = new GuiElement::Header("Shadow Mapping");
  useShadowMappingButton = new GuiElement::Checkbox("Use Shadow Mapping");
  shadowMappingHeader->addElement(useShadowMappingButton);
  shadowMappingHeader->addElement(new GuiElement::Spacing);

  useLinearButton = new GuiElement::Checkbox("Use linear filtering");
  shadowMappingHeader->addElement(useLinearButton);

  usePCFButton = new GuiElement::Checkbox("Use PCF filtering");
  shadowMappingHeader->addElement(usePCFButton);

  usePCFLinearButton = new GuiElement::Checkbox("Use PCF linear filtering");
  shadowMappingHeader->addElement(usePCFLinearButton);

  gui->addElement(shadowMappingHeader);
  //===============================================================================================

  

  GuiElement::Header *deferredShadingHeader = new GuiElement::Header("Deferred Shading");
  useDeferredShadingButton = new GuiElement::Checkbox("Use Deferred Shading");
  deferredShadingHeader->addElement(useDeferredShadingButton);
  deferredShadingHeader->addElement(new GuiElement::Spacing);


  GuiElement::ColorEditRGB *deferredShadingLightColorEdit = new GuiElement::ColorEditRGB("Lightcolor", dsLightColor);
  deferredShadingHeader->addElement(deferredShadingLightColorEdit);
  deferredShadingHeader->addElement(new GuiElement::Spacing);

  deferredShadingRotationButton = new GuiElement::Checkbox("Rotate Lights");
  deferredShadingHeader->addElement(deferredShadingRotationButton);
  deferredShadingHeader->addElement(new GuiElement::Spacing);

  gui->addElement(deferredShadingHeader);

  //===============================================================================================


  GuiElement::Header *ssaoHeader = new GuiElement::Header("Screen Space Ambient Occlusion");
  useSSAOButton = new GuiElement::Checkbox("Use SSAO");
  ssaoHeader->addElement(useSSAOButton);
  ssaoHeader->addElement(new GuiElement::Spacing);
  ssaoHeader->addElement(new GuiElement::SliderFloat("Quality", ssaoQuality, 0.0f, 100.0f));
  ssaoHeader->addElement(new GuiElement::Spacing);
  ssaoHeader->addElement(new GuiElement::SliderFloat("Radius", ssaoRadius, 0.0f, 0.5f));
  ssaoHeader->addElement(new GuiElement::Spacing);

  gui->addElement(ssaoHeader);

  //===============================================================================================
  GuiElement::Header *reflectionsHeader = new GuiElement::Header("Screen Space Reflections");
  useReflectionsButton = new GuiElement::Checkbox("Use Screen Space Reflections");
  reflectionsHeader->addElement(useReflectionsButton);
  reflectionsHeader->addElement(new GuiElement::Spacing);
  reflectionsHeader->addElement(new GuiElement::SliderFloat("Reflection Strength", reflectionStrength, 0.0f, 0.4f));
  reflectionsHeader->addElement(new GuiElement::Spacing);

  gui->addElement(reflectionsHeader);
  //===============================================================================================

  //===============================================================================================
  GuiElement::Header *postProcessingHeader = new GuiElement::Header("Postprocessing");

  //BLOOM
  useBloomButton = new GuiElement::Checkbox("Use Bloom");
  postProcessingHeader->addElement(useBloomButton);
  postProcessingHeader->addElement(new GuiElement::Spacing);
  postProcessingHeader->addElement(new GuiElement::SliderFloat("Bloom Strength", bloomStrength, 0.0f, 2.0f));
  postProcessingHeader->addElement(new GuiElement::Spacing);

  //FXAA
  useFXAAButton = new GuiElement::Checkbox("Use FXAA");
  postProcessingHeader->addElement(useFXAAButton);
  postProcessingHeader->addElement(new GuiElement::Spacing);


  gui->addElement(postProcessingHeader);
  //===============================================================================================
  //===============================================================================================
  GuiElement::Header *deptOfFieldHeader = new GuiElement::Header("Depth of Field");
  useDepthOfFieldButton = new GuiElement::Checkbox("Use Depth of Field");
  deptOfFieldHeader->addElement(useDepthOfFieldButton);
  deptOfFieldHeader->addElement(new GuiElement::Spacing);
  deptOfFieldHeader->addElement(new GuiElement::SliderFloat("Depthfield", focusDepth, 0.0f, 1.0f));
  deptOfFieldHeader->addElement(new GuiElement::Spacing);

  gui->addElement(deptOfFieldHeader);
  //===============================================================================================
  //===============================================================================================
  GuiElement::Header *blurHeader = new GuiElement::Header("Blur");
  useBlurButton = new GuiElement::Checkbox("Use Blur");
  blurHeader->addElement(useBlurButton);
  blurHeader->addElement(new GuiElement::Spacing);
  blurHeader->addElement(new GuiElement::SliderFloat("Blurstrength", blurStrength, 0.0f, 5.0f));
  blurHeader->addElement(new GuiElement::Spacing);

  gui->addElement(blurHeader);
  //===============================================================================================
  //===============================================================================================
  GuiElement::Header *radialBlurHeader = new GuiElement::Header("RadialBlur");

  useRadialBlurButton = new GuiElement::Checkbox("Use RadialBlur");
  radialBlurHeader->addElement(useRadialBlurButton);
  radialBlurHeader->addElement(new GuiElement::Spacing);
  radialBlurHeader->addElement(new GuiElement::SliderFloat("Blurstrength", radialBlurStrength, 0.0f, 5.0f));
  radialBlurHeader->addElement(new GuiElement::Spacing);

  gui->addElement(radialBlurHeader);
  //===============================================================================================

  renderer->addGui(gui);
}

int main()
{
  
	glfwInit();

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "Graphic Showcase");
	glfwMakeContextCurrent(testWindow.getWindow());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
  // You have to set a camera name
  cam.setName("PilotviewCam");
  cam.setPosition(glm::vec4(0.0, 0.5, 3.0, 1.0));
  cam.setNearFar(0.01f, 100.0f);

  // Set all InputMaps and set one InputMap active
  iH.setAllInputMaps(cam);
  iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();

  renderer = new Renderer(context);

  //our object
	Cube cube;
	Teapot teapot;
	Sphere sphere;
	
  Rect plane;
  plane.setTcoords(
    glm::vec2(0, 0),
    glm::vec2(4, 0),
    glm::vec2(0, 4),
    glm::vec2(4, 4)
  );

	//our textures

	Texture planks((char*)RESOURCES_PATH "/Vintage Plank_DIFFUSE.jpg");
	Texture planks_normal((char*)RESOURCES_PATH "/Vintage Plank_NORMAL.jpg");
	Texture planks_height((char*)RESOURCES_PATH "/Vintage Plank_DISP.jpg");

	Texture santao((char*)RESOURCES_PATH "/Santao Rosewood_DIFFUSE.jpg");
	Texture santao_normal((char*)RESOURCES_PATH "/Santao Rosewood_NORMAL.jpg");
	Texture santao_height((char*)RESOURCES_PATH "/Santao Rosewood_DISP.jpg");

	Texture stones((char*)RESOURCES_PATH "/18_DIFFUSE.jpg");
	Texture stones_normal((char*)RESOURCES_PATH "/18_NORMAL.jpg");
	Texture stones_height((char*)RESOURCES_PATH "/18_DISP.jpg");

	Texture tiles((char*)RESOURCES_PATH "/05_DIFFUSE.jpg");
	Texture tiles_normal((char*)RESOURCES_PATH "/05_NORMAL.jpg");
	Texture tiles_height((char*)RESOURCES_PATH "/05_DISP.jpg");

	Texture bricks((char*)RESOURCES_PATH "/01_DIFFUSE.jpg");
	Texture bricks_normal((char*)RESOURCES_PATH "/01_NORMAL.jpg");
	Texture bricks_height((char*)RESOURCES_PATH "/01_DISP.jpg");

	Texture marble((char*)RESOURCES_PATH "/seamless_marble.png");
	Texture chrome((char*)RESOURCES_PATH "/chrome.jpg");
	Texture cv((char*)RESOURCES_PATH "/cv_logo.bmp");

	//Scene creation 
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add Camera to scenegraph
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->getCamera("PilotviewCam");
    testScene.getScenegraph()->setActiveCamera("PilotviewCam");

	Node sphere1("sphere1");
	sphere1.addGeometry(&sphere);
	sphere1.addTexture(&tiles);
	sphere1.setModelMatrix(glm::translate(sphere1.getModelMatrix(), glm::vec3(-0.7, 0.30, -0.7)));
	sphere1.setModelMatrix(glm::scale(sphere1.getModelMatrix(), glm::vec3(0.2, 0.2, 0.2)));

	Node sphere2("sphere2");
	sphere2.addGeometry(&sphere);
	sphere2.addTexture(&planks);
	sphere2.addNormalMap(&planks_normal);
	sphere2.addHeightMap(&planks_height, 0.07, 0.1, false);
	sphere2.setModelMatrix(glm::translate(sphere2.getModelMatrix(), glm::vec3(1.3, 0.30, 0.7)));
	sphere2.setModelMatrix(glm::scale(sphere2.getModelMatrix(), glm::vec3(0.2, 0.2, 0.2)));

	Node sphere3("sphere3");
	sphere3.addGeometry(&sphere);
	sphere3.addTexture(&marble);
	sphere3.setModelMatrix(glm::translate(sphere3.getModelMatrix(), glm::vec3(-1.2, 0.30, 1.8)));
	sphere3.setModelMatrix(glm::scale(sphere3.getModelMatrix(), glm::vec3(0.2, 0.2, 0.2)));

	Node wallNode1("wall1");
	wallNode1.addGeometry(&plane);
	wallNode1.addTexture(&santao);
	wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(0.0, 0.1, 0.2)));
	wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), -90.0f, glm::vec3(1.0, 0.0, 0.0)));
	wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(2.5, 2.5, 2.5)));

	Node teaNode("teaNode");
	teaNode.addGeometry(&teapot);
	teaNode.addTexture(&cv);
	teaNode.setModelMatrix(glm::translate(teaNode.getModelMatrix(), glm::vec3(0.0, 0.3, 0.6)));
	teaNode.setModelMatrix(glm::scale(teaNode.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));

	//Creating a scenegraph
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&sphere1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&sphere2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&sphere3);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&teaNode);
	
  Node lights = Node("Root");
  Sphere lightSphere = Sphere();
  ConeLight slight(glm::vec4(2.0, 2.5, 3.5, 1.0), glm::vec4(0.7, 0.7, 0.7, 1.0), true, glm::vec3(-1.0, -1.0, -1.0), 90.0f, 1.0f, 10.0f);
  
  dsLightColor = new glm::fvec3(1.0f, 1.0f, 1.0f);

  Node *newLight1 = new Node(std::string("Node_1"));
  newLight1->addGeometry(&lightSphere);
  newLight1->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1,1,1)));
  newLight1->setModelMatrix(glm::scale(newLight1->getModelMatrix(), glm::vec3(3.0, 3.0, 3.0)));
  lights.addChildrenNode(newLight1);
  
  Node *newLight2 = new Node(std::string("Node_2"));
  newLight2->addGeometry(&lightSphere);
  newLight2->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1,1,1)));
  newLight2->setModelMatrix(glm::scale(newLight2->getModelMatrix(), glm::vec3(3.0, 3.0, 3.0)));
  lights.addChildrenNode(newLight2);

  Node *newLight3 = new Node(std::string("Node_3"));
  newLight3->addGeometry(&lightSphere);
  newLight3->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(1, 1, -1)));
  newLight3->setModelMatrix(glm::scale(newLight3->getModelMatrix(), glm::vec3(3.0, 3.0, 3.0)));
  lights.addChildrenNode(newLight3);

  Node *newLight4 = new Node(std::string("Node_4"));
  newLight4->addGeometry(&lightSphere);
  newLight4->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-1, 1, -1)));
  newLight4->setModelMatrix(glm::scale(newLight4->getModelMatrix(), glm::vec3(3.0, 3.0, 3.0)));
  lights.addChildrenNode(newLight4);

  ssaoQuality = new float(2.0f);
  ssaoRadius = new float(0.1f);

  reflectionStrength = new float(0.2f);
  blurStrength = new float(1.0f);
  radialBlurStrength = new float(0.10f);
  focusDepth = new float(0.04);
  bloomStrength = new float(0.20f);
  useShadowMode = new int(0);

  initGUI();

 	
	double startTime = glfwGetTime();
	//Renderloop
  int outputFPS = 0;
	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{

			if (useLinearButton->isActive() == true)
			{
				*useShadowMode = 1;
			}
			else if (usePCFButton->isActive() == true)
			{
				*useShadowMode = 2;
			}
			else if (usePCFLinearButton->isActive() == true)
			{
				*useShadowMode = 3;
			}
			else
				*useShadowMode = 0;

	renderer->useShadowMapping(useShadowMappingButton->isActive(), useShadowMode, &slight);
    renderer->useDeferredShading(useDeferredShadingButton->isActive(), &lights, dsLightColor);
    renderer->useSSAO(useSSAOButton->isActive(), ssaoQuality, ssaoRadius);
    renderer->useReflections(useReflectionsButton->isActive(), reflectionStrength);
	renderer->useBloom(useBloomButton->isActive(), bloomStrength);
    renderer->useAntiAliasing(useFXAAButton->isActive());
	renderer->useBlur(useBlurButton->isActive(), blurStrength);
	renderer->useRadialBlur(useRadialBlurButton->isActive(), radialBlurStrength);
	renderer->useDoF(useDepthOfFieldButton->isActive(), focusDepth);

	teaNode.setModelMatrix(glm::rotate(teaNode.getModelMatrix(), 0.5f, glm::vec3(0.0, 1.0, 0.0)));

	sphere1.setModelMatrix(glm::translate(sphere1.getModelMatrix(), glm::vec3(0.7, -0.30, 0.7)));
	sphere1.setModelMatrix(glm::rotate(sphere1.getModelMatrix(), 0.5f, glm::vec3(0.0, 1.0, 0.0)));
	sphere1.setModelMatrix(glm::translate(sphere1.getModelMatrix(), glm::vec3(-0.7, 0.30, -0.7)));

	sphere2.setModelMatrix(glm::translate(sphere2.getModelMatrix(), glm::vec3(-1.3, -0.30, -0.7)));
	sphere2.setModelMatrix(glm::rotate(sphere2.getModelMatrix(), 0.5f, glm::vec3(0.0, 1.0, 0.0)));
	sphere2.setModelMatrix(glm::translate(sphere2.getModelMatrix(), glm::vec3(1.3, 0.30, 0.7)));

	sphere3.setModelMatrix(glm::translate(sphere3.getModelMatrix(), glm::vec3(1.2, -0.30, -1.8)));
	sphere3.setModelMatrix(glm::rotate(sphere3.getModelMatrix(), 0.5f, glm::vec3(0.0, 1.0, 0.0)));
	sphere3.setModelMatrix(glm::translate(sphere3.getModelMatrix(), glm::vec3(-1.2, 0.30, 1.8)));

    // You have to compute the delta time
    float dTime = glfwGetTime() - startTime;

    cam.setSensitivity(dTime/50);

    if (deferredShadingRotationButton->isActive())
      lights.setModelMatrix(glm::rotate(lights.getModelMatrix(), 10.0f * dTime, glm::vec3(0.0, 1.0, 0.0)));

    if (!(outputFPS % 20))
      std::cout << "FPS: " << static_cast<int> (1 / dTime) << std::endl;

    outputFPS++;

    startTime = glfwGetTime();
    renderer->renderScene(testScene, testWindow);

	glfwSwapBuffers(testWindow.getWindow());
	glfwPollEvents();

	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}
