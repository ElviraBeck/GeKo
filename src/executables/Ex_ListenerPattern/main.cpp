//===================================================================//
//==================Your Includes===================================//
//==================================================================//
#include <GL/glew.h>

#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>

#include <GeKo_Graphics/Camera/StrategyCamera.h>
#include <GeKo_Graphics/Camera/Pilotview.h>

#include <GeKo_Graphics/AIInclude.h>
#include "GeKo_Gameplay/AI_Decisiontree/DecisionTree.h"
#include <GeKo_Gameplay/Object/Geko.h>
#include <GeKo_Gameplay/Object/Ant.h>

#include <GeKo_Graphics/Geometry/AntMesh.h>
#include <GeKo_Graphics/Geometry/TreeMesh.h>
#include <GeKo_Graphics/Geometry/GekoMesh.h>
#include <GeKo_Graphics/Geometry/Plane.h>

#include <GeKo_Graphics/ParticleSystem/ParticleSystem.h>

#include <GeKo_Physics/CollisionTest.h>

#include <GeKo_Gameplay/Observer/ObjectObserver.h>
#include <GeKo_Gameplay/Observer/CollisionObserver.h>
#include <GeKo_Gameplay/Observer/GravityObserver.h>
#include <GeKo_Gameplay/Observer/SoundObserver.h>
#include <GeKo_Gameplay/Observer/QuestObserver.h>

#include <GeKo_Gameplay/Questsystem/ItemReward.h>
#include <GeKo_Gameplay/Questsystem/ExpReward.h>
#include <GeKo_Gameplay/Questsystem/Goal_Kill.h>
#include <GeKo_Gameplay/Questsystem/Goal_Eaten.h>

//#include <GeKo_Graphics/GUI/GUI.h>
//#include <GeKo_Graphics/GUI/GUIComponents.hpp>
//#include <GeKo_Graphics/GUI/PlayerGUI.h>

//===================================================================//
//==================Things you need globally==========================//
//==================================================================//
Geko geko("Geko", glm::vec3(-10.0, 1.0,30.0));
static StrategyCamera cam("PlayerViewCam");
//Renderer *renderer;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int HUD_HEIGHT = 100;
const int HUD_WIDTH = 350;
const int QUEST_WIDTH = 325;
const int QUEST_HEIGHT = 300;
bool showlightTexture = false;

GUI *hud;
GuiElement::NestedWindow *questWindow;
GuiElement::NestedWindow *inventoryWindow;

//===================================================================//
//==================Callbacks for the Input==========================//
//==================================================================//

static InputHandler iH;

// As we do not use the Node class, we can't set the teapot into the middle of the window, therefore is the change of the direction improperly limited
// just needed in the player mode
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
	if (iH.getActiveInputMap()->getType() == MapType::CAMPLAYERVIEW){
		cam.turn(xpos, ypos);
	}
	if (iH.getActiveInputMap()->getType() == MapType::CAMSTRATEGY){
		if (glfwGetMouseButton(window, 0) == GLFW_PRESS){
			cam.turn(xpos, ypos);
		}
		else{
			cam.updateCursor(window);
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	iH.getActiveInputMap()->checkKeys(key, *window);
	geko.addKeyInput(key);
}

//===================================================================//
//==================Main Method=====================================//
//==================================================================//
int main()
{

	//===================================================================//
	//==================Things you need to start with====================//
	//==================================================================//
	glfwInit();

	Window testWindow(500, 50, 800, 600, "testWindow");
	glfwMakeContextCurrent(testWindow.getWindow());

	// Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	cam.setKeySpeed(2.0);
	cam.setNearFar(0.01, 100);

	glewInit();

	OpenGL3Context context;
	//renderer = new Renderer(context);
	Renderer renderer(context);

	//===================================================================//
	//==================Shaders for your program========================//
	//==================================================================//
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/Vertex-Shaders/TextureShader3D.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/Fragment-Shaders/TextureShader3D.frag")));
	ShaderProgram shader(vs, fs);

	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);



	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);

	ParticleSystem* particle = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/ComicCloudEffect.xml");
	particle->m_type = ParticleType::FIGHT;
	ParticleSystem* particle2 = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/SwarmOfFliesEffect.xml");
	particle2->m_type = ParticleType::SWARMOFFLIES;
	ParticleSystem* particleFire = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Fire.xml");
	particleFire->m_type = ParticleType::FIRE;

	Node particleNode("ParticleNode");
	particleNode.addParticleSystem(particle2);
	particleNode.setCamera(&cam);
	particleNode.setParticleActive(true);

	Node particleNodeFire("ParticleNodeFire");
	particleNodeFire.addParticleSystem(particleFire);
	particleNodeFire.setCamera(&cam);
	particleNodeFire.setParticleActive(true);

	//===================================================================//
	//==================A Graph for the AI-Unit=========================//
	//==================================================================//

	glm::vec3 posFood(10.0, 0.0, -5.0);
	glm::vec3 posSpawn(3.0, 0.0, 3.0);
	Graph<AStarNode, AStarAlgorithm> testGraph;

	//===================================================================//
	//==================A Decision-Tree for the AI-Unit==================//
	//==================================================================//

	DecisionTree defaultTree;
	defaultTree.setAntTreeAggressiv();


	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: AI==============================//

	Teapot teaAnt;

	Texture texPlayer((char*)RESOURCES_PATH "/Mesh/Snake.jpg");
	SoundFileHandler sfh = SoundFileHandler(1000);

	AStarNode defaultASNode();
	Ant ant_Flick;
	ant_Flick.setAntAfraid();

	AntMesh ant;

	Node aiNode("Flick");
	aiNode.addGeometry(&ant);
	aiNode.addTexture(&texPlayer);
	aiNode.addTranslation(ant_Flick.getPosition().x, ant_Flick.getPosition().y, ant_Flick.getPosition().z);
	aiNode.setObject(&ant_Flick);
	ant_Flick.setSoundHandler(&sfh);

	ant_Flick.setSourceName(MOVESOUND_AI, "AIFootsteps", RESOURCES_PATH "/Sound/Footsteps.wav");
	sfh.disableLooping("AIFootsteps");
	ant_Flick.setSourceName(DEATHSOUND_AI, "AIDeath", RESOURCES_PATH "/Sound/death.wav");
	sfh.disableLooping("AIDeath");
	ant_Flick.setSourceName(EATSOUND_AI, "AIEssen", RESOURCES_PATH "/Sound/Munching.wav");
	sfh.disableLooping("AIEssen");
	ant_Flick.setSourceName(DEATHSOUND_FLIES_AI, "Flies", RESOURCES_PATH "/Sound/Fliege_kurz.wav");
	sfh.setGain("Flies", 7.0);

	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();



	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Player===========================//
	Teapot teaPlayer;
	Texture texCV((char*)RESOURCES_PATH "/cv_logo.bmp");

	GekoMesh gekoMesh;
	geko.setExp(0.0);
	geko.setLevelThreshold(100.0);
	geko.setLevel(0);

	Node playerNode("Player");

	playerNode.addGeometry(&gekoMesh);
	playerNode.setObject(&geko);
	playerNode.addTexture(&texCV);

	sfh.generateSource(posFood, RESOURCES_PATH "/Sound/Rascheln.wav");
	geko.setSoundHandler(&sfh);
	geko.setSourceName(MOVESOUND, "SpielerFootsteps", RESOURCES_PATH "/Sound/Rascheln.wav");
	//geko.setSourceName(BACKGROUNDMUSIC, "Hintergrund", RESOURCES_PATH "/Sound/jingle2.wav");
	geko.setSourceName(FIGHTSOUND, "Kampfsound", RESOURCES_PATH "/Sound/punch.wav");
	geko.setSourceName(EATSOUND, "Essen", RESOURCES_PATH "/Sound/Munching.wav");
	geko.setSourceName(QUESTSOUND, "Quest", RESOURCES_PATH "/Sound/jingle.wav");
	geko.setSourceName(ITEMSOUND, "Item", RESOURCES_PATH "/Sound/itempickup.wav");
	geko.setSourceName(FIRESOUND, "Fire", RESOURCES_PATH "/Sound/Feuer_kurz.wav");

	sfh.disableLooping("Essen");
	sfh.disableLooping("Quest");
	sfh.disableLooping("Item");
	//sfh.generateSource("Feuer",posFood, RESOURCES_PATH "/Sound/Feuer kurz.wav");

	playerNode.setCamera(&cam);

	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Tree===========================//

	StaticObject treeStatic;
	treeStatic.setTree();

	TreeMesh tree;
	Node treeNode("Tree");
	treeNode.addGeometry(&tree);
	treeNode.setObject(&treeStatic);
	treeNode.addTranslation(posFood);
	treeNode.getBoundingSphere()->radius = 3.0;

	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Plane===========================//

	StaticObject terrainObject;
	terrainObject.setClassType(ClassType::TERRAIN);

	Plane terrain;
	Texture terrainTex((char*)RESOURCES_PATH "/Grass.jpg");

	Node terrainNode("Plane");
	terrainNode.addGeometry(&terrain);
	terrainNode.addTexture(&terrainTex);
	terrainNode.setObject(&terrainObject);
	terrainNode.addTranslation(0.0, -0.75, 0.0);
	terrainNode.addRotation(90.0f, glm::vec3(1.0, 0.0, 0.0));
	terrainNode.addScale(20.0, 20.0, 20.0);

	//===================================================================//
	//==================Setting up the Level and Scene==================//
	//==================================================================//

	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");
	testLevel.getFightSystem()->setParticle(particle);

	//==================Add Camera to Scene============================//
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->setActiveCamera("PlayerViewCam");

	//==================Set Input-Maps and activate one================//
	iH.setAllInputMaps(*(testScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap(MapType::OBJECT);
	iH.getActiveInputMap()->update(geko);

	//==================Add Objects to the Scene=======================//
	//==================Update the Bounding-Sphere 1st time============//
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&aiNode);

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&playerNode);

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&terrainNode);

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&treeNode);

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&particleNode);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&particleNodeFire);
	testScene.getScenegraph()->addParticleSystem(particle);
	testScene.getScenegraph()->addParticleSystem(particle2);
	testScene.getScenegraph()->addParticleSystem(particleFire);

	// ==============================================================
	// == Questsystem ====================================================
	// ==============================================================
	//QuestHandler questhandler;


	/*Quest questCollect(1);


	questCollect.setActive(true);
	Goal_Collect goalCollect(1);
	Goal_Collect goalCollect3(3);
	questCollect.addGoal(&goalCollect);
	questCollect.addGoal(&goalCollect3);

	ExpReward expReward(1);
	expReward.setExp(100);
	questCollect.addReward(&expReward);

	Item cookie2(100);
	cookie2.setName("Cookie100");
	cookie2.setTypeId(ItemType::COOKIE);
	ItemReward itemReward(2);
	itemReward.addItem(&cookie2);

	questCollect.addReward(&itemReward);

	Quest questCollect2(2);
	questCollect2.setActive(true);
	Goal_Collect goalCollect2(2);
	questCollect2.addGoal(&goalCollect2);
	questCollect2.addReward(&expReward);
	questCollect2.addReward(&itemReward);

	goalCollect.setGoalCount(50);
	goalCollect2.setGoalCount(50);
	goalCollect3.setGoalCount(50);

	goalCollect.setItemType(ItemType::BRANCH);
	goalCollect2.setItemType(ItemType::BRANCH);
	goalCollect3.setItemType(ItemType::COOKIE);

	testLevel.getQuestHandler()->addQuest(&questCollect);
	testLevel.getQuestHandler()->addQuest(&questCollect2);*/

	Quest questKillAnt(1);
	Quest questEatAnt(2);
	Quest questCollectCookie(3);
	Quest questCollectBranch(4);

	Goal_Kill killAnt(1);
	Goal_Eaten eatAnt(2);
	Goal_Collect collectCookie(3);
	Goal_Collect collectBranch(4);

	questKillAnt.addGoal(&killAnt);
	questEatAnt.addGoal(&eatAnt);
	questCollectCookie.addGoal(&collectCookie);
	questCollectBranch.addGoal(&collectBranch);

	killAnt.setGoalCount(1);
	eatAnt.setGoalCount(1);
	collectCookie.setGoalCount(1);
	collectBranch.setGoalCount(1);

	collectBranch.setItemType(ItemType::BRANCH);
	collectCookie.setItemType(ItemType::COOKIE);

	ExpReward expReward(1);
	expReward.setExp(100);

	questKillAnt.addReward(&expReward);
	questEatAnt.addReward(&expReward);
	questCollectCookie.addReward(&expReward);
	questCollectBranch.addReward(&expReward);

	QuestGraph questGraph;
	QuestGraphNode nodeStart;
	nodeStart.setQuest(&questKillAnt);
	questGraph.addNode(&nodeStart);
	questKillAnt.setActive(true);

	QuestGraphNode nodeSecond;
	nodeSecond.setQuest(&questEatAnt);
	nodeSecond.setParent(&nodeStart);
	questGraph.addNode(&nodeSecond);

	QuestGraphNode nodeThird;
	nodeThird.setQuest(&questCollectCookie);
	nodeThird.setParent(&nodeSecond);
	questGraph.addNode(&nodeThird);

	QuestGraphNode nodeFourth;
	nodeFourth.setQuest(&questCollectBranch);
	nodeFourth.setParent(&nodeThird);
	questGraph.addNode(&nodeFourth);

	testLevel.getQuestHandler()->addQuest(&questKillAnt);
	testLevel.getQuestHandler()->addQuest(&questEatAnt);
	testLevel.getQuestHandler()->addQuest(&questCollectBranch);
	testLevel.getQuestHandler()->addQuest(&questCollectCookie);

	testLevel.getQuestHandler()->setGraph(&questGraph);


	//===================================================================//
	//==================Setting up the Collision=========================//
	//==================================================================//
	CollisionTest collision;
	collision.collectNodes(testScene.getScenegraph()->getRootNode());


	//===================================================================//
	//==================Setting up the Observers========================//
	//==================================================================//
	ObjectObserver aiObserver(&testLevel);
	SoundObserver soundAIObserver(&testLevel);
	ant_Flick.addObserver(&aiObserver);
	ant_Flick.addObserver(&soundAIObserver);

	ObjectObserver playerObserver(&testLevel);
	SoundObserver soundPlayerObserver(&testLevel);
	geko.addObserver(&playerObserver);
	geko.addObserver(&soundPlayerObserver);

	CollisionObserver colObserver(&testLevel);
	collision.addObserver(&colObserver);
	collision.addObserver(&soundPlayerObserver);

	GravityObserver gravityObserver(&testLevel);
	collision.addObserver(&gravityObserver);

	QuestObserver questObserver(&testLevel);
	/*questCollect.addObserver(&questObserver);
	questCollect2.addObserver(&questObserver);
	questCollect.addObserver(&soundPlayerObserver);
	questCollect2.addObserver(&soundPlayerObserver);

	goalCollect.addObserver(&questObserver);
	goalCollect2.addObserver(&questObserver);
	goalCollect3.addObserver(&questObserver);*/
	questKillAnt.addObserver(&questObserver);
	questEatAnt.addObserver(&questObserver);
	questCollectCookie.addObserver(&questObserver);
	questCollectBranch.addObserver(&questObserver);

	killAnt.addObserver(&questObserver);
	eatAnt.addObserver(&questObserver);
	collectCookie.addObserver(&questObserver);
	collectBranch.addObserver(&questObserver);

	testLevel.getFightSystem()->addObserver(&questObserver);



	//===================================================================//
	//==================Setting up the Gravity===========================//
	//==================================================================//
	Gravity gravity;
	playerNode.addGravity(&gravity);
	aiNode.addGravity(&gravity);

	//===================================================================//
	//==================The Render-Loop==================================//
	//==================================================================//
	float lastTime = glfwGetTime();

	sfh.playSource("Feuer");
	sfh.playSource("Hintergrund");
	sfh.setGain("Hintergrund", 0.5f);

	//===================================================================//
	//==================The Particle-System==============================//
	//==================================================================//
	//Effect* smBla = new Effect();
	//smBla->loadEffect((char*)RESOURCES_PATH "/XML/ComicCloudEffect.xml");
	//ParticleSystem* particle = new ParticleSystem(glm::vec3(0, 0, -10), smBla);
	//	ParticleSystem* particle = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/ComicCloudEffect.xml");

	//===================================================================//
	//==================The GUI=========================================//
	//==================================================================//


	//========================================================================================================
	//SETUP GUI
	//Texture bricks((char*)RESOURCES_PATH "/bricks_diffuse.png");
	//hud = new GUI("testGUI", HUD_WIDTH, HUD_HEIGHT);
	//hud->setPosition((WINDOW_WIDTH / 2) - (HUD_WIDTH / 2), WINDOW_HEIGHT - HUD_HEIGHT);
	//hud->setCollapsable(false);
	//hud->setTitleBarVisible(false);
	//hud->setBackgroundAlpha(0.5f);
	//hud->setResizable(false);
	//hud->setUseScrollbar(false);
	//hud->setMoveable(false);

	//int hp = playerNode.getPlayer()->getHealth();
	//int hpMax = 10;
	//int exp = playerNode.getPlayer()->getExp();
	//int expMax = playerNode.getPlayer()->getLevelThreshold();

	//GuiElement::ProgressBar *hpBar = new GuiElement::ProgressBar(&hp, hpMax, 300, glm::fvec4(1.0f, 0.0f, 0.0f, 1.0f));
	//hud->addElement(hpBar);
	//hud->addElement(new GuiElement::SameLine());
	//hud->addElement(new GuiElement::Text("HP"));

	//GuiElement::ProgressBar *expBar = new GuiElement::ProgressBar(&exp, expMax, 300, glm::fvec4(1.0f, 0.9960784f, 0.9529411f, 1.0f));
	//hud->addElement(expBar);
	//hud->addElement(new GuiElement::SameLine());
	//hud->addElement(new GuiElement::Text("EXP"));

	//hud->addElement(new GuiElement::Spacing());
	//hud->addElement(new GuiElement::Separator());
	//hud->addElement(new GuiElement::Spacing());
	//hud->addElement(new GuiElement::Text("LVL"));

	//int level = playerNode.getPlayer()->getLvl();
	//hud->addElement(new GuiElement::SameLine());
	//GuiElement::IntBox *lvlBox = new GuiElement::IntBox(&level, glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f), glm::fvec4(0.7f, 0.7f, 0.7f, 1.0f));
	//hud->addElement(lvlBox);

	//hud->addElement(new GuiElement::SameLine());
	//GuiElement::PushButton *inventoryButton = new GuiElement::PushButton("Inventory");
	//hud->addElement(inventoryButton);
	//hud->addElement(new GuiElement::SameLine());
	//GuiElement::PushButton *questButton = new GuiElement::PushButton("Quests");
	//hud->addElement(questButton);

	//questWindow = new GuiElement::NestedWindow();
	//questWindow->hide();
	//GuiElement::Header *quest1 = new GuiElement::Header("Test the Quest");
	//quest1->addElement(new GuiElement::Text("Quest description here bla bla bla"));
	//GuiElement::Header *quest2 = new GuiElement::Header("Testwindow Questwindow");
	//quest2->addElement(new GuiElement::Text("Quest description here too bla bla bla"));
	//questWindow->addElement(quest1);
	//questWindow->addElement(quest2);
	//questWindow->setName("Quests");
	//questWindow->setCollapsable(false);
	//questWindow->setPosition(WINDOW_WIDTH - QUEST_WIDTH, (WINDOW_HEIGHT / 2) - (QUEST_HEIGHT / 2));
	//questWindow->setSize(QUEST_WIDTH, QUEST_HEIGHT);
	//hud->addNestedWindow(questWindow);

	//inventoryWindow = new GuiElement::NestedWindow();
	//inventoryWindow->hide();

	//inventoryWindow->setName("Inventory");
	//inventoryWindow->setCollapsable(false);
	//inventoryWindow->setResizable(false);
	//inventoryWindow->setPosition(WINDOW_WIDTH - QUEST_WIDTH, (WINDOW_HEIGHT / 2) - (QUEST_HEIGHT / 2));
	//inventoryWindow->setSize(QUEST_WIDTH, QUEST_HEIGHT);
	//hud->addNestedWindow(inventoryWindow);

	//std::map<std::string, Texture*> *inventoryItems = new std::map<std::string, Texture*>();
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem1"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem2"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem3"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem4"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem5"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem6"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem7"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem8"), &bricks));
	//inventoryItems->insert(std::pair<std::string, Texture*>(std::string("bricksItem9"), &bricks));
	//GuiElement::Inventory *inventory = new GuiElement::Inventory(inventoryItems, 6);
	//inventoryWindow->addElement(inventory);
	
//	PlayerGUI playerGUI(HUD_WIDTH, HUD_HEIGHT, WINDOW_HEIGHT, WINDOW_WIDTH, QUEST_HEIGHT, QUEST_WIDTH, playerNode.getPlayer());


	float testFloat = float(0.0f);
	float testFloat2 = float(0.0f);

	/*testLevel.addGUI(hud);
	testLevel.getGUI("testGUI")->m_windowName;*/

	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		fboGBuffer.bind();
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderGBuffer.bind();
		shaderGBuffer.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderGBuffer.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		
		testScene.render(shaderGBuffer);


		ant_Flick.update();
		geko.update();
		geko.deleteKeyInput();
		geko.setDeltaTime(currentTime);
		collision.update();

		//TEST
		//particle->update(cam);
		//particle->render(cam);

		shaderGBuffer.unbind();
	
		fboGBuffer.unbind();

		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

