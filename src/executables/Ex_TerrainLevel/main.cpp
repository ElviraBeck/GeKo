//===================================================================//
//==================Your Includes===================================//
//==================================================================//
#include <GL/glew.h>

#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/Camera/StrategyCamera.h>
#include <GeKo_Graphics/Camera/Pilotview.h>
#include <GeKo_Graphics/ShaderInclude.h>

#include <GeKo_Graphics/AIInclude.h>
#include "GeKo_Gameplay/AI_Decisiontree/DecisionTree.h"
#include <GeKo_Gameplay/Object/Geko.h>
#include <GeKo_Gameplay/Object/AI.h>

#include <GeKo_Graphics/Geometry/AntMesh.h>
#include <GeKo_Graphics/Geometry/TreeMesh.h>
#include <GeKo_Graphics/Geometry/GekoMesh.h>
#include <GeKo_Graphics/Geometry/Plane.h>
#include <GeKo_Graphics/Geometry/ForestData.h>

#include <GeKo_Graphics/Geometry/Terrain.h>

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

#include <GeKo_Gameplay/Object/AntHome.h>

#include <GeKo_Graphics/GUI/GUI.h>
#include <GeKo_Graphics/GUI/GUIComponents.hpp>
#include <GeKo_Graphics/GUI/PlayerGUI.h>

//===================================================================//
//==================Things you need globally==========================//
//==================================================================//
Player geko("Geko", glm::vec3(-10.0, 1.0, 30.0));
static StrategyCamera cam("PlayerViewCam");
static InputHandler iH;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int HUD_HEIGHT = 100;
const int HUD_WIDTH = 350;
const int QUEST_WIDTH = 325;
const int QUEST_HEIGHT = 300;

GUI *hud;

//===================================================================//
//==================Callbacks for the Input==========================//
//==================================================================//

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
	// The active InputMap is fetched
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();
	// You go over the active InputMap, if it's the key that is pressed, a method is called and the mapped action is executed else the key is ignored
	for (std::map<int, std::function<void()>>::iterator it = activeMap.begin(); it != activeMap.end(); it++){
		if (it->first == key)
			activeMap.at(key)();
		if (it == activeMap.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}
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

	Window testWindow(500, 50, 800, 600, "Demo");
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

	VertexShader vsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.vert")));
	FragmentShader fsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.frag")));
	ShaderProgram shaderSkybox(vsSkybox, fsSkybox);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);

	SoundFileHandler sfh = SoundFileHandler(1000);

	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();

	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Terrain===========================//

	StaticObject terrainObject;
	terrainObject.setClassType(ClassType::TERRAIN);

	Texture terrainTex((char*)RESOURCES_PATH "/Grass2.jpg");

	Terrain terrain2((char*)RESOURCES_PATH "/heightmap.jpg", 0.0f, 0.0f);
	Node terrainNode2("Terrain");
	terrainNode2.addGeometry(&terrain2);
	terrainNode2.addTexture(&terrainTex);
	terrainNode2.setObject(&terrainObject);


	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Player===========================//

	Texture texCV((char*)RESOURCES_PATH "/cv_logo.bmp");

	GekoMesh gekoMesh;
	geko.setExp(0.0);
	geko.setLevelThreshold(100.0);
	geko.setLevel(0);

	Node playerNode("Player");

	playerNode.addGeometry(&gekoMesh);
	playerNode.setObject(&geko);
	playerNode.addTexture(&texCV);

	sfh.generateSource(glm::vec3(geko.getPosition()), RESOURCES_PATH "/Sound/Rascheln.wav");
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
	geko.setPosition(glm::vec4(terrain2.getResolutionX() / 2.0f, 10.0f, terrain2.getResolutionY() / 2.0f, 1.0));
	//sfh.generateSource("Feuer",posFood, RESOURCES_PATH "/Sound/Feuer kurz.wav");
	playerNode.setCamera(&cam);

	//===================================================================//
	//==================Setting up the Level and Scene==================//
	//==================================================================//

	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");
	testLevel.setTerrain(&terrain2);

	//==================Add Camera to Scene============================//
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->setActiveCamera("PlayerViewCam");

	//==================Set Input-Maps and activate one================//
	iH.setAllInputMaps(*(testScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap(MapType::OBJECT);
	iH.getActiveInputMap()->update(geko);

	//==================Add Objects to the Scene=======================//
	//==================Update the Bounding-Sphere 1st time============//
	/*Node translateNode("Translate");
	translateNode.addTranslation(glm::vec3(terrain2.getResolutionX() / 2.0f, 0.0, terrain2.getResolutionY() / 2.0f));*/

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&terrainNode2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&playerNode);
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&translateNode);
	//testScene.getScenegraph()->getRootNode()->getChildrenNode("Translate")->addChildrenNode(&playerNode);



	//===================================================================//
	//==================Setting up the Observers========================//
	//==================================================================//

	ObjectObserver playerObserver(&testLevel);
	SoundObserver soundPlayerObserver(&testLevel);
	geko.addObserver(&playerObserver);
	geko.addObserver(&soundPlayerObserver);


	// ==============================================================
	// == Object (Forest) ==========================================
	// ==============================================================
	TreeMesh tree;

	glm::vec3 tmp;
	std::stringstream name;
	for (int i = 0; i<TreeData::forest1.size(); i++)
	{
		name << "Forest1Tree" << i;
		std::string stringname = name.str();
		StaticObject *treeStatic = new StaticObject();
		treeStatic->setTree(50 / TreeData::forest1.size());
		Node *treeNode = new Node(stringname);
		treeNode->addGeometry(&tree);
		treeNode->setObject(treeStatic);
		tmp.x = TreeData::forest1[i].x;
		tmp.z = TreeData::forest1[i].z;
		tmp.y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		treeNode->addTranslation(tmp);
		treeNode->getBoundingSphere()->radius = 2.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}
	for (int i = 0; i<TreeData::forest2.size(); i++)
	{
		name << "Forest2Tree" << i;
		std::string stringname = name.str();
		StaticObject *treeStatic = new StaticObject();
		treeStatic->setTree(50 / TreeData::forest2.size());
		Node *treeNode = new Node(stringname);
		treeNode->addGeometry(&tree);
		treeNode->setObject(treeStatic);

		tmp.x = TreeData::forest2[i].x;
		tmp.z = TreeData::forest2[i].z;
		tmp.y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		treeNode->addTranslation(tmp);
		treeNode->getBoundingSphere()->radius = 2.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}

	// ==============================================================
	// == Object (Anthome) ==========================================
	// ==============================================================
	glm::vec3 posFood(10.0, 0.0, -5.0);
	glm::vec3 posFood2((terrain2.getResolutionX() / 2.0f) + 10.0, 0.0, (terrain2.getResolutionY() / 2.0f) - 5.0);
	glm::vec3 posSpawn(terrain2.getResolutionX() / 2.0f, 10.0, terrain2.getResolutionY() / 2.0f);
	glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);
	AntMesh antMesh;

	DecisionTree *aggressivedecisionTree = new DecisionTree();
	aggressivedecisionTree->setAntTreeAggressiv();

	DecisionTree *afraidDecisionTree = new DecisionTree();
	afraidDecisionTree->setAntTreeAfraid();

	Graph<AStarNode, AStarAlgorithm>* antAggressiveGraph = new Graph<AStarNode, AStarAlgorithm>();
	antAggressiveGraph->setExampleAntAggressiv(posSpawn, posFood2, posDefaultPlayer);

	Graph<AStarNode, AStarAlgorithm>* antAfraidGraph = new Graph<AStarNode, AStarAlgorithm>();
	antAfraidGraph->setExampleAntAfraid2(posSpawn, posDefaultPlayer);

	AntHome antHome(posSpawn, antMesh, &playerObserver, &texCV, &texCV, aggressivedecisionTree, antAggressiveGraph, afraidDecisionTree, antAfraidGraph);
	//antHome.generateGuards(5, &aiObserver, testScene.getScenegraph()->getRootNode());
	antHome.generateWorkers(5, testScene.getScenegraph()->getRootNode());
	/*antHome.addAntsToSceneGraph(testScene.getScenegraph()->getRootNode());*/





	//===================================================================//
	//==================Setting up the Collision=========================//
	//==================================================================//
	CollisionTest collision;
	collision.collectNodes(testScene.getScenegraph()->getRootNode());

	CollisionObserver colObserver(&testLevel);
	collision.addObserver(&colObserver);
	collision.addObserver(&soundPlayerObserver);

	GravityObserver gravityObserver(&testLevel);
	collision.addObserver(&gravityObserver);



	//===================================================================//
	//==================Setting up the Gravity===========================//
	//==================================================================//
	Gravity gravity;
	playerNode.addGravity(&gravity);

	//===================================================================//
	//==================Setting up the Gravity===========================//
	//==================================================================//
	PlayerGUI playerGUI(HUD_WIDTH, HUD_HEIGHT, WINDOW_HEIGHT, WINDOW_WIDTH, QUEST_HEIGHT, QUEST_WIDTH, playerNode.getPlayer());

	//===================================================================//
	//==================The Render-Loop==================================//
	//==================================================================//
	float lastTime = glfwGetTime();

	sfh.playSource("Hintergrund");
	sfh.setGain("Hintergrund", 0.5f);


	//TODO adjust the Rotation,to match the Terrain
	glm::vec4 tmpPos;
	glm::vec3 normalFromTerrain;
	glm::vec3 rotateAxis;
	glm::vec4 viewDirFromPlayer;
	glm::vec3 up(0.0, 1.0, 0.0);
	float lengthFromNormal;
	float lengthFromUp;
	float phi;



	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{


		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;


		//===================================================================//
		//==================Update your Objects per Frame here =============//
		//==================================================================//
		collision.update();


		//===================================================================//
		//==================Input and update for the Player==================//
		//==================================================================//

		geko.update();
		geko.setDeltaTime(currentTime);

		tmpPos = testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition();
		viewDirFromPlayer = testScene.getScenegraph()->searchNode("Player")->getPlayer()->getViewDirection();
		//ToDo calculate Normal funktioniert evtl falsch
		normalFromTerrain = terrain2.calculateNormal(tmpPos.x, tmpPos.z);
		rotateAxis = glm::cross(glm::vec3(viewDirFromPlayer), normalFromTerrain );
	//	lengthFromNormal = glm::length(normalFromTerrain);
	//	lengthFromUp = glm::length(up);
		normalFromTerrain = glm::normalize(normalFromTerrain);
		up = glm::normalize(up);
		phi = glm::dot(up, normalFromTerrain);
		phi = glm::atan(phi) * (180 / glm::pi<float>());
	//	phi = glm::acos(glm::dot(normalFromTerrain, up) / (lengthFromNormal * lengthFromUp));
		//ToDo Rotation überschreibt die frühere Rotation
		testScene.getScenegraph()->searchNode("Player")->addRotation(phi, rotateAxis);

		antHome.updateAnts();

		//===================================================================//
		//==================Render your Objects==============================//
		//==================================================================//
		//renderer.renderScene(testScene, testWindow);


		fboGBuffer.bind();
		glClearColor(0.5, 0.5, 0.5, 0);
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

		renderer.renderGUI(*playerGUI.getHUD(), testWindow);
		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();


	}



	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();


	return 0;

}

