//#include <TinyXML/src/TinyXML/tinyxml2.h>
#include <vector>
#include <GL/glew.h>
#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>

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

	//our window
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "ParticleSystemXML", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	//CAM
	cam.setKeySpeed(4.0);
	iH.setAllInputMaps(cam);
	glfwSetKeyCallback(window, key_callback);
	cam.setFOV(50);
	cam.setNearFar(1, 100);

	glewInit();

	//Textures
	Texture* fireTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/Fire2_M.png");
	Texture* fireTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire1_M.png");
	Texture* fireTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire3_M.png");
	Texture* fireTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame02_L.png");
	//Texture* fireFlickering1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_1.png");
	//Texture* fireFlickering2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_2.png");
	//Texture* fireFlickering3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_3.png");
	//Texture* fireFlickering4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_4.png");
	Texture* fireSparkleTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireSparkle1.png");
	Texture* fireSparkleTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireSparkle2.png");

	//Texture* texFireworkBlue = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireworkParticleBlue.png");
	Texture* texFireworkRed = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/firework_red.png");
	//Texture* texFireworkGold = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireworkParticleGold.png");
	Texture* texFireworkTail = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/firework_tail.png");

	Texture* smokeWhiteTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite01.png");
	Texture* smokeWhiteTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite02.png");
	Texture* smokeBlack1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack01.png");
	Texture* smokeBlack2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack02.png");

	//////////////////////Emitter//////////////////////
	
	//Emitter explosion sparkle
	Emitter* explosionSparkle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.25, 0.01, 80, 1.25, true);
	explosionSparkle->setVelocity(5);
	//explosionSparkle->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 0.3), 3.0f);
	explosionSparkle->usePhysicPointGravity(glm::vec3(0.0, 0.0, 0.0), 0.6, 10.0, 2, 4.0f, true);
	explosionSparkle->addTexture(fireSparkleTex1, 1.0);
	explosionSparkle->defineLook(true, 0.01, 0.0, 0.1);
	
	//Emitter explosion fire
	//TODO

	//Emitter fire smoke
	Emitter* fire = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 2, 5.0, true);
	fire->setVelocity(5);
	fire->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 5.0), 0.5f);
	fire->addTexture(fireTex1, 1.0);
	fire->addTexture(fireTex2, 0.7);
	fire->addTexture(smokeBlack2, 0.1);
	//fire->addTexture(smokeBlack2, 0.1);
	//fire->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> sizeF{ 0.05f, 0.5f, 0.75f, 1.0f };
	std::vector<float> timeF{ 0.0f, 0.4f, 0.75f, 1.0f };
	fire->defineLook(true, sizeF, timeF, 0.5, 4.0, 3.0, true, 0.3);
	fire->switchToGeometryShader();

	//Emitter fire flickering
	Emitter* fireFlickering = new Emitter(0, glm::vec3(0.0, 0.1, 0.0), 0.0, 0.2, 1, 2.0, true);
	fireFlickering->setVelocity(5);
	fireFlickering->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.5), 0.3f);
	/*fireFlickering->addTexture(fireFlickering1, 1.0);
	fireFlickering->addTexture(fireFlickering2, 0.7);
	fireFlickering->addTexture(fireFlickering3, 0.5);
	fireFlickering->addTexture(fireFlickering4, 0.3);*/
	fireFlickering->defineLook(true, 0.1, 0.5, 1.0, 0.5, true, 0.3);
	fireFlickering->switchToGeometryShader();

	Emitter* fireSparkle = new Emitter(0, glm::vec3(0.0, 0.1, 0.0), 0.0, 0.05, 3, 2.5, true);
	fireSparkle->setVelocity(5);
	fireSparkle->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.8), 0.5f);
	fireSparkle->addTexture(fireSparkleTex1, 1.0);
	fireSparkle->defineLook(true, 0.005, 0.5, 0.5);
	//testEff->addEmitter(fireSparkle);
	
	
	//Emitter firework explosion
	Emitter* fireworkExplosion = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.1, 0.01, 50, 1.8, true);
	fireworkExplosion->setVelocity(6);
	//fireworkExplosion->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 0.6), 3.0f);
	fireworkExplosion->usePhysicPointGravity(glm::vec3(0.0, -2.0, 0.0), 0.9, 5.0, 2, 2.0f, true);
	fireworkExplosion->addTexture(texFireworkRed, 1.0);
	fireworkExplosion->defineLook(true, 0.01, 0.0, 0.1);
	
	//Emitter firework tail
	Emitter* fireworkTail = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 2.0, 0.01, 20, 0.5, true);
	fireworkTail->setVelocity(5);
	//fireworkTail->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 2.9), 0.2f);
	fireworkTail->usePhysicPointGravity(glm::vec3(0.0, -4.0, 0.0), 30.6, 10.0, 2, 0.2f, true);
	fireworkTail->addTexture(texFireworkTail, 1.0);
	fireworkTail->defineLook(true, 0.001, 0.0, 0.1);


	//FINAL EMITTER WHITE SMOKE
	Emitter* smokeWhite = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.4, 1, 8.0, true);
	smokeWhite->setVelocity(2);
	smokeWhite->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeWhite->addTexture(smokeWhiteTex1, 1.0);
	smokeWhite->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> smokeWhiteSize{ 0.05f, 0.5f, 0.75f, 1.2f };
	std::vector<float> smokeWhiteTime{ 0.0f, 0.4f, 0.75f, 1.0f };
	smokeWhite->defineLook(true, smokeWhiteSize, smokeWhiteTime, 1.0, 4.0, 4.0, false, 0.3);
	smokeWhite->switchToGeometryShader();

	//////////////////////Effect//////////////////////
	Effect* efExplosion = new Effect();
	efExplosion->addEmitter(explosionSparkle);
	
	Effect* efFire = new Effect();
	efFire->addEmitter(fire);
	//efFire->addEmitter(fireFlickering);
	efFire->addEmitter(fireSparkle);
	
	Effect* efFireworkTail = new Effect();
	efFireworkTail->addEmitter(fireworkTail);

	Effect* efFireworkExplosion = new Effect();
	efFireworkExplosion->addEmitter(fireworkExplosion);

	Effect* smWhi = new Effect();
	smWhi->addEmitter(smokeWhite);

	//////////////////////ParticleSystem//////////////////////
	ParticleSystem* psExplosion = new ParticleSystem(glm::vec3(0, -1, 0), efExplosion);
	ParticleSystem* psFire = new ParticleSystem(glm::vec3(-2, 0, 3), efFire);
	ParticleSystem* psFireworkTail = new ParticleSystem(glm::vec3(0, 0, 5), efFireworkTail);
	ParticleSystem* psFireworkExplosion = new ParticleSystem(glm::vec3(0, 2, 5), efFireworkExplosion);
	ParticleSystem* psSmokeWhite = new ParticleSystem(glm::vec3(2, 0, 3), smWhi);


	//////////////////////Node//////////////////////
	Node nodeExplosion("nodeExplosion");
	nodeExplosion.setCamera(&cam);
	nodeExplosion.addParticleSystem(psExplosion);
	
	Node fireNode("fireNode");
	fireNode.setCamera(&cam);
	fireNode.addParticleSystem(psFire);
	
	Node nodeFireworkTail("fireworkTailNode");
	nodeFireworkTail.setCamera(&cam);
	nodeFireworkTail.addParticleSystem(psFireworkTail);

	Node nodeFireworkExplosion("fireworkExplosionNode");
	nodeFireworkExplosion.setCamera(&cam);
	nodeFireworkExplosion.addParticleSystem(psFireworkExplosion);
	
	Node whiteSmokeNode("whiteSmokeNode");
	whiteSmokeNode.setCamera(&cam);
	whiteSmokeNode.addParticleSystem(psSmokeWhite);

	//set nodes active
	nodeExplosion.setParticleActive(true);
	fireNode.setParticleActive(true);
	nodeFireworkTail.setParticleActive(true);
	nodeFireworkExplosion.setParticleActive(true);
	whiteSmokeNode.setParticleActive(true);


	// Shader
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.frag")));
	ShaderProgram shader(vs, fs);

	// Renderer
	OpenGL3Context context;
	Renderer renderer(context);

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

	//add nodes to the scenegraph
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeExplosion);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&fireNode);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFireworkTail);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFireworkExplosion);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&whiteSmokeNode);

	//start the ParticleSystems
	psFire->start();
	psSmokeWhite->start();
	//psExplosion->start();
	psFireworkTail->start();
	//psFireworkExplosion->start();

	double startTime = glfwGetTime();
	
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(window))
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		cam.setSensitivity(glfwGetTime() - startTime);
		startTime = glfwGetTime();

		glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();
		shader.sendMat4("viewMatrix", cam.getViewMatrix());
		shader.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		testScene.render(shader);
		shader.unbind();

		if (glfwGetTime() > 2.8 && glfwGetTime() < 2.9) {
			psFireworkExplosion->start();
			psFireworkTail->stop();
		}

		glm::vec3 pos = psFireworkTail->getPosition();
		psFireworkTail->setPosition(glm::vec3(pos.x, pos.y + (glfwGetTime() - startTime), pos.z));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}