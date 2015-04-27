#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"
#include "GeKo_Graphics/MaterialInclude.h"
#include "GeKo_Graphics/Renderer/Renderer.h"
#include "GeKo_Graphics/Shader/Shader.h"
#include "GeKo_Graphics/Geometry/Teapot.h"
#include "GeKo_Graphics/Camera/Pilotview.h"
#include "GeKo_Gameplay/Input/InputHandler.h"

InputHandler iH;
Pilotview cam("Pilotview");

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	// The active InputMap is fetched
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();

	// You go over the active InputMap, if it's the key that is pressed, a method is called and the mapped action is executed else the key is ignored
	for (std::map<int, std::function<void()>>::iterator it = activeMap.begin(); it != activeMap.end(); it++){
		if (it->first == key)
			//iH.getActiveInputMap()->checkMultipleMappedKeys(key, *window);
		if (it == activeMap.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}
}

int main()
{
	glfwInit();

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "ParticleSystemScene", NULL, NULL);
	glfwMakeContextCurrent(window);

	//CAM
	cam.setKeySpeed(4.0);
	iH.setAllInputMaps(cam);
	glfwSetKeyCallback(window, key_callback);
	cam.setFOV(50);
	cam.setNearFar(1, 100);

	glewInit();

	//TEXTURES
	Texture* smokeWhiteTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite01.png");
	Texture* smokeWhiteTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite02.png");
	Texture* smokeBlackTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack01.png");
	Texture* smokeBlackTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack02.png");
	Texture* snowTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/snowflake.png"); //TODO better Resolution

	//FINAL EMITTER SNOW
	Emitter* snow = new Emitter(0, glm::vec3(0.0, 10.0, -5.0), 0.0, 0.166, 100, 30.0, true);
	snow->setVelocity(0);
	snow->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 1.0), 0.5f);
	snow->setAreaEmitting(false, true, 10.0, 10000);
	snow->addTexture(snowTex, 0.0);
	snow->defineLook(true, 0.04, 2.0);
	snow->setMovable(true);

	//FINAL EMITTER WHITE SMOKE
	Emitter* smokeWhite = new Emitter(0, glm::vec3(0.0, 0.0, 5.0), 0.0, 0.4, 1, 8.0, true);
	smokeWhite->setVelocity(2);
	smokeWhite->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeWhite->addTexture(smokeWhiteTex1, 1.0);
	smokeWhite->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> smokeWhiteSize{ 0.05f, 0.5f, 0.75f, 1.2f };
	std::vector<float> smokeWhiteTime{ 0.0f, 0.4f, 0.75f, 1.0f };
	smokeWhite->defineLook(true, smokeWhiteSize, smokeWhiteTime, 1.0, 4.0, 4.0, false, 0.3);
	smokeWhite->switchToGeometryShader();

	//FINAL EMITTER BLACK SMOKE
	Emitter* smokeBlack = new Emitter(0, glm::vec3(0.0, 0.0, -10.0), 0.0, 0.4, 1, 8.0, true);
	smokeBlack->setVelocity(2);
	smokeBlack->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeBlack->addTexture(smokeBlackTex1, 1.0);
	smokeBlack->addTexture(smokeBlackTex2, 0.08);
	std::vector<float> smokeBlackSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> smokeBlackTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	smokeBlack->defineLook(true, smokeBlackSize, smokeBlackTime, 1.0, 4.0, 4.0, false, 0.3);
	smokeBlack->switchToGeometryShader();

	//PARTICLE SYSTEM
	Effect* sn = new Effect();
	sn->addEmitter(snow);
	ParticleSystem* psSnow = new ParticleSystem(glm::vec3(0, 2, -5), sn);
	Node snowNode("snowNode");
	snowNode.setParticleActive(true);
	snowNode.setCamera(&cam);
	snowNode.addParticleSystem(psSnow);
	
	Effect* smWhi = new Effect();
	smWhi->addEmitter(smokeWhite);
	ParticleSystem* psSmokeWhite = new ParticleSystem(glm::vec3(0, 0, 3), smWhi);
	Node whiteSmokeNode("whiteSmokeNode");
	whiteSmokeNode.setParticleActive(true);
	whiteSmokeNode.setCamera(&cam);
	whiteSmokeNode.addParticleSystem(psSmokeWhite);
	
	Effect* smBla = new Effect();
	smBla->addEmitter(smokeBlack);
	ParticleSystem* psSmokeBlack = new ParticleSystem(glm::vec3(0, 0, -3), smBla);
	Node blackSmokeNode("blackSmokeNode");
	blackSmokeNode.setParticleActive(true);
	blackSmokeNode.setCamera(&cam);
	blackSmokeNode.addParticleSystem(psSmokeBlack);
	
	// Shader
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.frag")));
	ShaderProgram shader(vs, fs);

	// Renderer
	OpenGL3Context context;
	Renderer renderer(context);

	// Object
	Teapot teapot;
	teapot.loadBufferData();
	Node teapotNode("teapotNode");
	teapotNode.addGeometry(&teapot);
	teapotNode.setCamera(&cam);
	teapotNode.setModelMatrix(glm::translate(teapotNode.getModelMatrix(), glm::vec3(0.0, 0.0, -7.0)));
	
	//need scene here mainly because of input
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add Camera to Scene
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->setActiveCamera("Pilotview");

	//Set Input-Maps and activate one
	iH.setAllInputMaps(*(testScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap("Pilotview");

	//Add Objects to the Scene
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&teapotNode);	
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&blackSmokeNode);	
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&whiteSmokeNode);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&snowNode);

	//start the ParticleSystems
	psSmokeBlack->start();
	psSmokeWhite->start();
	psSnow->start();

	// getting the start time
	double startTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		cam.setSensitivity(glfwGetTime() - startTime);
		startTime = glfwGetTime();

		glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();
		shader.sendMat4("modelMatrix", teapotNode.getModelMatrix());
		shader.sendMat4("viewMatrix", cam.getViewMatrix());
		shader.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		testScene.render(shader);
		shader.unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}