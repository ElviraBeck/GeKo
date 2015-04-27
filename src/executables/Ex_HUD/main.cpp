#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/Camera/Pilotview.h"
#include "GeKo_Graphics/GUI/GUI.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int HUD_HEIGHT = 100;
const int HUD_WIDTH = 350;
const int QUEST_WIDTH = 325;
const int QUEST_HEIGHT = 300;
bool showlightTexture = false;

InputHandler iH;
Pilotview cam("Pilotview");

GUI *hud;
GuiElement::NestedWindow *questWindow;
GuiElement::NestedWindow *inventoryWindow;

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

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "HUD");
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

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);

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


  //========================================================================================================
  //SETUP GUI
  
  hud = new GUI("",HUD_WIDTH, HUD_HEIGHT);
  hud->setPosition((WINDOW_WIDTH / 2) - (HUD_WIDTH / 2), WINDOW_HEIGHT - HUD_HEIGHT);
  hud->setCollapsable(false);
  hud->setTitleBarVisible(false);
  hud->setBackgroundAlpha(0.5f);
  hud->setResizable(false);
  hud->setUseScrollbar(false);
  hud->setMoveable(false);

  int hp = 20;
  int hpMax = 20;
  int exp = 10;
  int expMax = 30;

  GuiElement::ProgressBar *hpBar = new GuiElement::ProgressBar(&hp, &hpMax, 300, glm::fvec4(1.0f, 0.0f, 0.0f, 1.0f));
  hud->addElement(hpBar);
  hud->addElement(new GuiElement::SameLine());
  hud->addElement(new GuiElement::Text("HP"));
  
  GuiElement::ProgressBar *expBar = new GuiElement::ProgressBar(&exp, &expMax, 300, glm::fvec4(1.0f, 0.9960784f, 0.9529411f, 1.0f));
  hud->addElement(expBar);
  hud->addElement(new GuiElement::SameLine());
  hud->addElement(new GuiElement::Text("EXP"));
  
  hud->addElement(new GuiElement::Spacing());
  hud->addElement(new GuiElement::Separator());
  hud->addElement(new GuiElement::Spacing());
  hud->addElement(new GuiElement::Text("LVL"));

  int level = 1;
  hud->addElement(new GuiElement::SameLine());
  GuiElement::IntBox *lvlBox = new GuiElement::IntBox(&level, glm::fvec4(1.0f,1.0f,1.0f,1.0f), glm::fvec4(0.7f,0.7f,0.7f,1.0f));
  hud->addElement(lvlBox);

  hud->addElement(new GuiElement::SameLine());
  GuiElement::PushButton *inventoryButton = new GuiElement::PushButton("Inventory");
  hud->addElement(inventoryButton);
  hud->addElement(new GuiElement::SameLine());
  GuiElement::PushButton *questButton = new GuiElement::PushButton("Quests");
  hud->addElement(questButton);

  questWindow = new GuiElement::NestedWindow();
  questWindow->hide();
  GuiElement::Header *quest1 = new GuiElement::Header("Test the Quest");
  quest1->addElement(new GuiElement::Text("Quest description here bla bla bla"));
  GuiElement::Header *quest2 = new GuiElement::Header("Testwindow Questwindow");
  quest2->addElement(new GuiElement::Text("Quest description here too bla bla bla"));
  questWindow->addElement(quest1);
  questWindow->addElement(quest2);
  questWindow->setName("Quests");
  questWindow->setCollapsable(false);
  questWindow->setPosition(WINDOW_WIDTH-QUEST_WIDTH,(WINDOW_HEIGHT/2)-(QUEST_HEIGHT/2));
  questWindow->setSize(QUEST_WIDTH,QUEST_HEIGHT);
  hud->addNestedWindow(questWindow);

  inventoryWindow = new GuiElement::NestedWindow();
  inventoryWindow->hide();
    
  inventoryWindow->setName("Inventory");
  inventoryWindow->setCollapsable(false);
  inventoryWindow->setResizable(false);
  inventoryWindow->setPosition(WINDOW_WIDTH - QUEST_WIDTH, (WINDOW_HEIGHT / 2) - (QUEST_HEIGHT / 2));
  inventoryWindow->setSize(QUEST_WIDTH, QUEST_HEIGHT);
  hud->addNestedWindow(inventoryWindow);
  
  std::map<std::string, Texture*> *inventoryItems = new std::map<std::string, Texture*> ();
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem1"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem2"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem3"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem4"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem5"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem6"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem7"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem8"),&bricks));
  inventoryItems->insert(std::pair<std::string,Texture*>(std::string("bricksItem9"),&bricks));
  GuiElement::Inventory *inventory = new GuiElement::Inventory(inventoryItems,6);
  inventoryWindow->addElement(inventory);

  //========================================================================================================

	double startTime = glfwGetTime();
	//Renderloop
 
  int outputFPS = 0;
    
	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{
		// You have to compute the delta time
		
    float deltaTime = glfwGetTime() - startTime;
    cam.setSensitivity(deltaTime);
		
    if (!(outputFPS % 40))
    {
      hp--;
      exp++;
      std::cout << "Selected Item: " << inventory->getSelectedItem() << std::endl;
    }
   
    if (hp < 0)
      hp = 20;
    

    if (exp > 30)
    {
      exp = 0;
      level++;
    }

    outputFPS++;
		startTime = glfwGetTime();

    

		fboGBuffer.bind();
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderGBuffer.bind();
		shaderGBuffer.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderGBuffer.sendMat4("projectionMatrix", cam.getProjectionMatrix());
   
		testScene.render(shaderGBuffer);

		
    shaderGBuffer.unbind();
		fboGBuffer.unbind();
				
		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderSFQ.sendSampler2D("fboTexture", fboGBuffer.getColorTexture(2));
    
		screenFillingQuad.renderGeometry();
		shaderSFQ.unbind();
		
    if (questButton->isPushed())
     {
        questWindow->toggleVisibility();
    }

    if (inventoryButton->isPushed())
    {
      inventoryWindow->toggleVisibility();
    }

    renderer.renderGUI(*hud, testWindow);

		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}
