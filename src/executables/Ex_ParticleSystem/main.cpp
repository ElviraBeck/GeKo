#include "GeKo_Graphics/ParticleSystem/Emitter.h" //TODO Delete this line
#include "GeKo_Graphics/ParticleSystem/Effect.h"  //TODO Delete this line
#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"

#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/GUI/GUI.h"

//GUI
GUI *gui;
GuiElement::Checkbox *useSnowButton;
GuiElement::Checkbox *useStrongSnowButton;
GuiElement::Checkbox *useRainButton;
GuiElement::Checkbox *useFruitFliesButton;
GuiElement::Checkbox *useMaximumParticle;
int particleCount = 0;

//CAM
InputHandler iH;
Pilotview cam("Pilotview");
Pilotview screen("Screen");

//RENDERER FOR GUI
OpenGL3Context context;
Renderer *renderer;

//include data?
enum FLOW { UNUSED = -1, CONSTANT = 0, ONCE = 1 } output;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	// The active InputMap is fetched
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();

	// You go over the active InputMap, if it's the key that is pressed, the mapped action is executed else the key is ignored
	for (std::map<int, std::function<void()>>::iterator it = activeMap.begin(); it != activeMap.end(); it++){
		if (it->first == key)
			activeMap.at(key)();
		if (it == activeMap.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}
}

//TODO ausklappen
void initGUI()
{
	gui = new GUI("Particlesystem Settings - Hide [F10]", 300, 250);

	GuiElement::Header *moveableParticleHeader = new GuiElement::Header("Weather");

	useSnowButton = new GuiElement::Checkbox("Snow");
	moveableParticleHeader->addElement(useSnowButton);
	moveableParticleHeader->addElement(new GuiElement::Spacing);
	
	useStrongSnowButton = new GuiElement::Checkbox("Strong Snow");
	moveableParticleHeader->addElement(useStrongSnowButton);
	moveableParticleHeader->addElement(new GuiElement::Spacing);

	useRainButton = new GuiElement::Checkbox("Rain");
	moveableParticleHeader->addElement(useRainButton);
	moveableParticleHeader->addElement(new GuiElement::Spacing);

	GuiElement::Header *screenParticleHeader = new GuiElement::Header("Screen Emitter");

	useFruitFliesButton = new GuiElement::Checkbox("Fruitflies");
	screenParticleHeader->addElement(useFruitFliesButton);
	screenParticleHeader->addElement(new GuiElement::Spacing);

	GuiElement::Header *maxParticleHeader = new GuiElement::Header("Maximum Particlenumber");
	useMaximumParticle = new GuiElement::Checkbox("300 000 Particles without Texture");
	maxParticleHeader->addElement(useMaximumParticle);
	maxParticleHeader->addElement(new GuiElement::Spacing);

	gui->addElement(moveableParticleHeader);
	gui->addElement(screenParticleHeader);
	gui->addElement(maxParticleHeader);

	renderer = new Renderer(context);
	//renderer->addGui(gui);
}

int main()
{
	//////////////////////////////////////////BASICS///////////////////////////////////////////

	srand(time(NULL));

	//glfwInit();

	//WINDOW
	Window window(50, 50, 800, 600, "ParticleSystem");
	glfwMakeContextCurrent(window.getWindow());

	//CAM
	cam.setPosition(glm::vec4(-15.0, 0.0, 7.0, 1.0));
	cam.setNearFar(0.1f, 100.0f);
	cam.setLookAt(glm::vec3(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z - 1.0));
	cam.setKeySpeed(8.0);
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(window.getWindow(), key_callback);

	//More then just 60 fps, vsync off
	glfwSwapInterval(0);

	glewInit();

	//SHADER
	VertexShader vsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.vert")));
	FragmentShader fsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.frag")));
	ShaderProgram shaderSkybox(vsSkybox, fsSkybox);

	VertexShader vsObject(loadShaderSource(SHADERS_PATH + std::string("/TextureShader3D/TextureShader3D.vert")));
	FragmentShader fsObject(loadShaderSource(SHADERS_PATH + std::string("/TextureShader3D/TextureShader3D.frag")));
	ShaderProgram shaderObject(vsObject, fsObject);

	/////////////////////////////////////////TEXTURES//////////////////////////////////////////

	//COMIC CLOUD
	Texture* comicBalloonTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon01.png"); 
	Texture* comicBalloonTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon02.png"); 
	Texture* comicBalloonTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon03.png"); 
	Texture* comicBalloonTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon04.png"); 
	Texture* comicCloudTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud01.png"); 
	Texture* comicCloudTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud02.png");
	Texture* comicCloudTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud03.png");
	Texture* comicCloudTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud04.png");
	Texture* comicExclamationMarkTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark01.png");
	Texture* comicExclamationMarkTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark02.png");
	Texture* comicExclamationMarkTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark03.png");
	Texture* comicExclamationMarkTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark04.png");
	Texture* comicLightningTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning01.png");
	Texture* comicLightningTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning02.png");
	Texture* comicLightningTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning03.png");
	Texture* comicLightningTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning04.png");
	Texture* comicSpiralTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral01.png");
	Texture* comicSpiralTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral02.png");
	Texture* comicSpiralTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral03.png");
	Texture* comicSpiralTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral04.png");
	Texture* comicSpiralTex5 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral05.png");
	Texture* comicSpiralTex6 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral06.png");
	Texture* comicStarTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/star01.png");
	Texture* comicStarTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/star02.png");

	//FIRE
	Texture* fireTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame01_L.png");
	Texture* fireTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame02_L.png");
	Texture* fireTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame03_L.png");

	//PARTICLE
	Texture* particleBlackTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/virus.png");
	Texture* particleWhiteTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/particle.png");
	Texture* snowTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/snowflake.png"); //TODO better Resolution
	Texture* glowwormTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/glowworm.png");
	Texture* energyTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/energy.png");
	Texture* fireflyTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/firefly.png");
	Texture* fireSparkTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/sparkOrange.png");
	Texture* fireSparkTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/sparkRed.png");

	//RAIN
	Texture* rainTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/rain/rain.png"); //TODO darker

	//SMOKE
	Texture* drawSmokeTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke01_L.png");
	Texture* drawSmokeTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke02_L.png");
	Texture* drawSmokeTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke03_L.png");
	Texture* drawSmokeTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke04_L.png");
	Texture* smokeWhiteTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite01.png");
	Texture* smokeWhiteTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite02.png");
	Texture* smokeBlackTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack01.png");
	Texture* smokeBlackTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack02.png");

	//STAR
	Texture* fireWorkYellowTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starYellow.png"); //TODO Work?
	Texture* fireWorkOrangeTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starPink.png"); //TODO Work?
	Texture* fireWorkRedTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starRed.png"); //TODO Work?
	Texture* fireWorkBlueTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starBlue.png"); //TODO Work?
	Texture* fireWorkGreenTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starGreen.png"); //TODO Work?

	///////////////////////////////////////FINAL EMITTER///////////////////////////////////////

	//FINAL EMITTER SNOW
	Emitter* snow = new Emitter(0, glm::vec3(0.0, 5.0, 0.0), 0.0, 0.166, 100, 30.0, true);
	snow->setVelocity(0);
	snow->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 1.0), 0.5f);
	snow->setAreaEmitting(false,true, 10.0, 10000);
	snow->addTexture(snowTex, 0.0);
	snow->defineLook(true, 0.04, 2.0);

	//FINAL EMITTER STRONG SNOW
	Emitter* snowStrong = new Emitter(0, glm::vec3(-3.5, 4.0, 0.0), 0.0, 0.166, 100, 15.0, true);
	snowStrong->setVelocity(2);
	snowStrong->usePhysicDirectionGravity(glm::vec4(0.6, -1.5, 0.0, 1.0), 2.6f);
	snowStrong->setAreaEmitting(false, true, 8.0, 10000);
	snowStrong->addTexture(snowTex, 0.0);
	snowStrong->defineLook(true, 0.03, 1.0, 3.0);

	//FINAL EMITTER WHITE SMOKE
	Emitter* smokeWhite = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.4, 1, 8.0, true);
	smokeWhite->setVelocity(2);
	smokeWhite->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeWhite->addTexture(smokeWhiteTex1, 1.0);
	smokeWhite->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> smokeWhiteSize{ 0.05f, 0.5f, 0.75f, 1.2f };
	std::vector<float> smokeWhiteTime{ 0.0f, 0.4f, 0.75f, 1.0f };
	smokeWhite->defineLook(true, smokeWhiteSize, smokeWhiteTime, 1.0, 4.0, 3.0, false, 0.3);
	smokeWhite->switchToGeometryShader();

	//FINAL EMITTER BLACK SMOKE
	Emitter* smokeBlack = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.6, 1, 8.0, true);
	smokeBlack->setVelocity(2);
	smokeBlack->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeBlack->addTexture(smokeBlackTex1, 1.0);
	smokeBlack->addTexture(smokeBlackTex2, 0.2);
	std::vector<float> smokeBlackSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> smokeBlackTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	smokeBlack->defineLook(true, smokeBlackSize, smokeBlackTime, 1.0, 5.0, 3.0, false, 0.3);
	smokeBlack->switchToGeometryShader();

	//FINAL EMITTER CLOUD SMOKE
	Emitter* smokeCloud = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.3, 1, 10.0, true);
	smokeCloud->setVelocity(3);
	smokeCloud->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.1), 0.15f);
	smokeCloud->addTexture(smokeWhiteTex1, 1.0);
	smokeCloud->addTexture(smokeWhiteTex2, 0.08);
	std::vector<float> smokeCloudSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> smokeCloudTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	smokeCloud->defineLook(true, smokeBlackSize, smokeBlackTime, 1.0, 2.0, 4.0, false, 0.3);
	smokeCloud->switchToGeometryShader();

	//FINAL EMITTER RAIN
	Emitter* rain = new Emitter(0, glm::vec3(0.0, 3.0, 0.0), 0.0, 0.02, 50, 5.0, true);
	rain->setVelocity(0);
	rain->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 1.0), 5.0f);
	rain->setAreaEmitting(false, true, 8.0, 10000);
	rain->addTexture(rainTex, 0.0);
	rain->defineLook(true, 0.03, 1.0, 0.0);
	particleCount += (50 * 5.0 / 0.02);

	//FINAL EMITTER FONTAINE
	Emitter* fontaine = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.05, 5, 2.0, true);
	fontaine->setVelocity(0);
	fontaine->usePhysicTrajectory(glm::vec4(0.0, 0.0, 0.0, 1.0), 1.0);
	//fontaine->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.7f);
	fontaine->defineLook(false, 0.3, 0.0, 0.5, 0.0, true, 1.0);

	//FINAL EMITTER CIRCLE
	Emitter* circle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 50, 4.0, true);
	circle->setVelocity(4);
	circle->addTexture(fireSparkTex1, 1.0);
	circle->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.3f);
	circle->addTexture(fireSparkTex2 ,0.1);
	circle->defineLook(true, 0.03, 0.0, 2.0, 1.0, true, 0.0);
	circle->switchToGeometryShader();

	//FINAL EMITTER QUAD
	Emitter* quad = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0, 100, 1.0, true);
	quad->setVelocity(0);
	quad->setAreaEmitting(true, true, 0.5, 1);
	quad->defineLook(false, 0.1, 0.0, 0.0, 0.0, false, 0.0);

	//FINAL EMITTER FRUITFLIES
	Emitter* fruitFlies = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.166, 2, 10.0, true);
	fruitFlies->setVelocity(0);
	fruitFlies->usePhysicSwarmCircleMotion(true, true, true, 3.0);
	fruitFlies->setAreaEmitting(true, true, 0.5, 100);
	fruitFlies->addTexture(particleBlackTex, 0.0);
	fruitFlies->defineLook(true, 0.012, 3.0, 3.0, 0.0, true, 1.0);
	fruitFlies->switchToGeometryShader();

	//FINAL SCREEN EMITTER FRUITFLIES
	Emitter* screenFruitFlies = new Emitter(0, glm::vec3(-0.2, 0.2, 7.0), 0.0, 0.1, 2, 10.0, true);
	screenFruitFlies->setVelocity(0);
	screenFruitFlies->usePhysicSwarmCircleMotion(true, true, true, 3.0);
	screenFruitFlies->setAreaEmitting(true, false, 2.5, 1000);
	screenFruitFlies->addTexture(particleBlackTex, 0.0);
	screenFruitFlies->defineLook(true, 0.02, 1.0, 3.0, 0.0, true, 1.0);
	screenFruitFlies->switchToGeometryShader();

	//FINAL EMITTER GLOWWORM
	Emitter* glowworm = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 1, 10.0, true);
	glowworm->setVelocity(0);
	glowworm->usePhysicSwarmCircleMotion(true, true, true, 3.0);
	glowworm->setAreaEmitting(true, false, 0.4, 1000);
	glowworm->addTexture(glowwormTex, 0.0);
	glowworm->defineLook(true, 0.1, 1.0, 1.5);

	//FINAL EMITTER ENERGYBALL
	Emitter* energyBall = new Emitter(0, glm::vec3(0.0, 0.0, 1.0), 0.0, 0.01, 5, 15.0, true);
	energyBall->setVelocity(5);
	//energyBall->usePhysicPointGravity(glm::vec4(2.0, 2.0, 2.0, 3.0), 8.0, 2, 1.8);
	energyBall->addTexture(energyTex, 1.0);
	energyBall->defineLook(true, 0.04);

	//FINAL EMITTER COMIC CLOUD
	Emitter* cloud01 = new Emitter(0, glm::vec3(0.0, 0.5, 0.0), 0.0, 1.6, 1, 10.0, true);
	cloud01->setVelocity(4);
	//cloud01->usePhysicPointGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.0, 0, 0.72);
	cloud01->addTexture(comicCloudTex1, 0.0);
	std::vector<float> cloudSize1{ 0.8f, 1.6f };
	std::vector<float> cloudTime1{ 0.0f, 1.0f };
	cloud01->defineLook(true, cloudSize1, cloudTime1, 0.4, 4.0, 0.0, false, 0.3);
	cloud01->switchToGeometryShader();

	Emitter* cloud02 = new Emitter(0, glm::vec3(0.0, 0.5, 0.0), 0.0, 2.3, 1, 10.0, true);
	cloud02->setVelocity(4);
	//cloud02->usePhysicPointGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.0, 0, 0.6);
	cloud02->addTexture(comicCloudTex3, 0.0);
	std::vector<float> cloudSize2{ 0.8f, 1.6f };
	std::vector<float> cloudTime2{ 0.0f, 1.0f };
	cloud02->defineLook(true, cloudSize2, cloudTime2, 0.4, 4.0, 0.0, false, 0.3);
	cloud02->switchToGeometryShader();

	Emitter* cloud03 = new Emitter(0, glm::vec3(-1.0, 1.0, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud03->setVelocity(0);
	cloud03->addTexture(comicBalloonTex4, 1.0);
	cloud03->defineLook(true, 0.8, 2.0);
	cloud03->switchToGeometryShader();

	Emitter* cloud04 = new Emitter(0, glm::vec3(1.2, 2.0, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud04->setVelocity(0);
	cloud04->addTexture(comicStarTex2, 1.0);
	cloud04->defineLook(true, 0.3, 2.0, 0.0, 0.0, false, 0.6);
	cloud04->switchToGeometryShader();

	Emitter* cloud05 = new Emitter(0, glm::vec3(-1.0, -1.0, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud05->setVelocity(0);
	cloud05->addTexture(comicStarTex2, 1.0);
	cloud05->defineLook(true, 0.5, 2.0, 0.0, 0.0, true, 0.4);
	cloud05->switchToGeometryShader();

	Emitter* cloud06 = new Emitter(0, glm::vec3(1.7, 0.2, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud06->setVelocity(0);
	cloud06->addTexture(comicSpiralTex5, 1.0);
	cloud06->defineLook(true, 0.3, 2.0);
	cloud06->switchToGeometryShader();

	Emitter* cloud07 = new Emitter(0, glm::vec3(-2.0, 0.7, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud07->setVelocity(0);
	cloud07->addTexture(comicSpiralTex3, 1.0);
	cloud07->defineLook(true, 0.3, 2.0);
	cloud07->switchToGeometryShader();

	Emitter* cloud08 = new Emitter(0, glm::vec3(-1.9, 2.1, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud08->setVelocity(0);
	cloud08->addTexture(comicLightningTex1, 1.0);
	cloud08->defineLook(true, 0.6, 2.0);
	cloud08->switchToGeometryShader();

	Emitter* cloud09 = new Emitter(0, glm::vec3(2.0, 1.4, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud09->setVelocity(0);
	cloud09->addTexture(comicLightningTex4, 1.0);
	cloud09->defineLook(true, 0.5, 2.0);
	cloud09->switchToGeometryShader();

	Emitter* cloud10 = new Emitter(0, glm::vec3(2.7, 0.8, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud10->setVelocity(0);
	cloud10->addTexture(comicExclamationMarkTex3, 1.0);
	cloud10->defineLook(true, 0.3, 2.0);
	cloud10->switchToGeometryShader();

	Emitter* cloud11 = new Emitter(0, glm::vec3(-0.5, 2.4, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud11->setVelocity(0);
	cloud11->addTexture(comicExclamationMarkTex2, 1.0);
	cloud11->defineLook(true, 0.5, 2.0);
	cloud11->switchToGeometryShader();

	////////////////////////////////NOT FINAL EMITTER////////////////////////////////

	//FINAL EMITTER MAXIMUM; SET POSITION
	Emitter* maximumParticle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 5000, 12.0, true);
	maximumParticle->setVelocity(3);
	maximumParticle->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.2), 0.5);
	maximumParticle->defineLook(false, 0.01);
	int particleCountMax = 5000 * 12 / 0.2;

	//particleMax with Texture

	//!EMITTER FIRE
	Emitter* fireMiddle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0, 3.0, 4.0, true);
	fireMiddle->setVelocity(3);
	fireMiddle->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.5), 0.2);
	fireMiddle->addTexture(fireTex1, 1.0);
	//fireMiddle->addTexture(fireTex2, 0.5);
	fireMiddle->addTexture(fireTex3, 0.0);
	fireMiddle->defineLook(true, 0.5, 1.0, 2.0);
	fireMiddle->switchToGeometryShader();

	Emitter* fireSmoke = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.4, 1, 8.0, true);
	fireSmoke->setVelocity(2);
	fireSmoke->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	fireSmoke->addTexture(smokeBlackTex1, 1.0);
	fireSmoke->addTexture(smokeBlackTex2, 0.08);
	std::vector<float> fireSmokeSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> fireSmokeTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	fireSmoke->defineLook(true, fireSmokeSize, fireSmokeTime, 1.0, 4.0, 1.0, false, 0.3);
	fireSmoke->switchToGeometryShader();

	Emitter* firefly = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 1.3, 1, 9.0, true);
	firefly->setVelocity(3);
	firefly->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -1.5), 0.13f);
	firefly->addTexture(fireflyTex, 1.0);
	firefly->defineLook(true, 0.04, 2.0, 2.0, 0.0, false, 0.3);

	Emitter* fireSparkOrange = new Emitter(0, glm::vec3(0.1, 0.0, 1.0), 0.0, 0.1, 1, 0.8, true);
	fireSparkOrange->setVelocity(3);
	fireSparkOrange->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -1.0), 1.5f);
	fireSparkOrange->addTexture(fireSparkTex1, 1.0);
	fireSparkOrange->defineLook(true, 0.02, 0.4, 0.3, 0.0, false, 0.3);

	Emitter* fireSparkRed = new Emitter(0, glm::vec3(0.1, 0.0, 1.0), 0.0, 0.1, 1, 1.0, true);
	fireSparkRed->setVelocity(3);
	fireSparkRed->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -1.5), 1.5f);
	fireSparkRed->addTexture(fireSparkTex2, 1.0);
	fireSparkRed->defineLook(true, 0.02, 0.4, 0.3, 0.0, false, 0.3);

	//!EMITTER FIREWORK
	Emitter* firework = new Emitter(0, glm::vec3(0.6, 0.6, 0.0), 0.0, 0.5, 5, 1.0, true);
	firework->setVelocity(4);
	firework->usePhysicDirectionGravity(glm::vec4(0.0,-1.0,0.0,0.1), 1.0f);
	//firework->addTexture(*fireWorkYellowTex, 0.0);
	//firework->defineLook(true, 0.8, 0.1, 0.2);
	Emitter* firework2 = new Emitter(0, glm::vec3(-0.6, -0.6, 0.0), 0.0, 0.5, 5, 1.0, true);
	firework2->setVelocity(4);
	firework2->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 0.1), 1.0f);
	//firework2->addTexture(*fireWorkOrangeTex, 0.0);
	//firework2->defineLook(true, 0.8, 0.1, 0.2);

	//////////////////////////////////////Effect//////////////////////////////////////////////////
	//Effect* ef


	//////////////////////////////////////OUR SCENE///////////////////////////////////////////////

	//SKYBOX
	Cube cube;
	const char *textureNames[6] = { 
		/*(char*)RESOURCES_PATH "/PereaBeach1/posx.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/negx.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/posy.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/negy.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/posz.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/negz.jpg"*/
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png" };
	Skybox skybox(textureNames);
	Node skyboxNode("skybox");
	skyboxNode.addGeometry(&cube);

	//OBJECTS
	Teapot teapot;
	teapot.loadBufferData();
	Sphere sphere;
	sphere.loadBufferData();
	Rect plane;
	plane.loadBufferData();

	//TEXTURES
	Texture chrome((char*)RESOURCES_PATH "/Metal/chrome.jpg");
	Texture marble((char*)RESOURCES_PATH "/Wall/seamless_marble.png"); //TODO JPG
	Texture cvLogo((char*)RESOURCES_PATH "/Symbol/cv_logo.bmp");
	Texture bricks((char*)RESOURCES_PATH "/Wall/brick.bmp");

	//CREATING NODES
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	Node cube1("cube1");
	cube1.addGeometry(&cube);
	cube1.addTexture(&chrome);
	cube1.setModelMatrix(glm::translate(cube1.getModelMatrix(), glm::vec3(-0.3, 0.25, 0.4)));
	cube1.setModelMatrix(glm::scale(cube1.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));

	Node cube2("cube2");
	cube2.addGeometry(&cube);
	cube2.addTexture(&chrome);
	cube2.setModelMatrix(glm::translate(cube2.getModelMatrix(), glm::vec3(0.7, 0.25, 0.4)));
	cube2.setModelMatrix(glm::scale(cube2.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));

	Node wallNode1("wall1");
	wallNode1.addGeometry(&plane);
	wallNode1.addTexture(&marble);
	wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(0.0, -1.0, 0.0)));
	wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), 90.0f, glm::vec3(1.0, 0.0, 0.0)));
	wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(25.0, 2.0, 1.0)));

	Node wallNode2("wall2");
	wallNode2.addGeometry(&plane);
	wallNode2.addTexture(&bricks);
	wallNode2.setModelMatrix(glm::translate(wallNode2.getModelMatrix(), glm::vec3(0.0, 0.0, 0.0)));
	wallNode2.setModelMatrix(glm::scale(wallNode2.getModelMatrix(), glm::vec3(25.0, 2.5, 1.0)));

	Node teaNode("teaNode");
	teaNode.addGeometry(&teapot);
	teaNode.addTexture(&cvLogo);
	teaNode.setModelMatrix(glm::translate(teaNode.getModelMatrix(), glm::vec3(0.2, 0.3, 1.0)));
	teaNode.setModelMatrix(glm::scale(teaNode.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));

	//CREATING A SCENEGRAPH
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&teaNode);

	//ADD CAMERA TO SCENEGRAPH
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->getCamera("Pilotview");
	testScene.getScenegraph()->setActiveCamera("Pilotview");

	//GUI
	//initGUI();

	double startCamTime = glfwGetTime();

	int outputFrames = 0;
	float dTime;

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		//CAM
		dTime = glfwGetTime() - startCamTime;
		cam.setSensitivity(dTime);
		startCamTime = glfwGetTime();

		//Maximum Particle
		if (!useMaximumParticle->isActive()){

			//SKYBOX
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderSkybox.bind();
			glDisable(GL_DEPTH_TEST);
			shaderSkybox.sendMat4("viewMatrix", cam.getViewMatrix());
			shaderSkybox.sendMat4("projectionMatrix", cam.getProjectionMatrix());
			shaderSkybox.sendSkyboxTexture("testTexture", skybox.getSkyboxTexture());
			//skyboxNode.render();
			shaderSkybox.unbind();

			glEnable(GL_DEPTH_TEST);
			shaderObject.bind();
			shaderObject.sendMat4("viewMatrix", cam.getViewMatrix());
			shaderObject.sendMat4("projectionMatrix", cam.getProjectionMatrix());
			shaderObject.sendInt("useTexture", 1);
			teaNode.setModelMatrix(glm::rotate(teaNode.getModelMatrix(), 3.0f, glm::vec3(0.0, 1.0, 0.0)));
			cube1.setModelMatrix(glm::rotate(cube1.getModelMatrix(), 3.0f, glm::vec3(0.0, 1.0, 0.0)));
			cube2.setModelMatrix(glm::rotate(cube2.getModelMatrix(), 3.0f, glm::vec3(0.0, 1.0, 0.0)));
			//testScene.render(shaderObject);
			shaderObject.unbind();

			///////////////////////////////////////FINAL EMITTER///////////////////////////////////////
			glDisable(GL_DEPTH_TEST);

			//smokeWhite->update();
			//smokeWhite->render(cam);

			//smokeBlack->update();
			//smokeBlack->render(cam);

			//fontaine->update(cam);
			//fontaine->render(cam);

			//circle->update();
			//circle->render(cam);

			//quad->update();
			//quad->render(cam);

			//glowworm->update();
			//glowworm->render(cam);

			//fruitFlies->update();
			//fruitFlies->render(cam);

			//energyBall->update();
			//energyBall->render(cam);

			//cloud01->update();
			//cloud01->render(cam);
			//cloud02->update();
			//cloud02->render(cam);
			//cloud03->update();
			//cloud03->render(cam);
			//cloud04->update();
			//cloud04->render(cam);
			//cloud05->update();
			//cloud05->render(cam);
			//cloud06->update();
			//cloud06->render(cam);
			//cloud07->update();
			//cloud07->render(cam);
			//cloud08->update();
			//cloud08->render(cam);
			//cloud09->update();
			//cloud09->render(cam);
			//cloud10->update();
			//cloud10->render(cam);
			//cloud11->update();
			//cloud11->render(cam);
		}
		////////////////////////////////FINAL SCREEN EMITTER//////////////////////////////////////////////////
		else{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//maximumParticle->update();
			//maximumParticle->render(cam);
			particleCount = particleCountMax;
		}

		if (useFruitFliesButton->isActive()){
			//screenFruitFlies->update();
			//screenFruitFlies->render(screen);
		}
		else{
			screenFruitFlies->startTime();
		}

			////////////////////////////////FINAL GUI EMITTER///////////////////////////////////////////
		if (useSnowButton->isActive()){
			//snow->update(glm::vec3(cam.getPosition()));
			//snow->render(cam);
		}
		else{
			snow->startTime();
		}

		if (useStrongSnowButton->isActive()){
			//snowStrong->update(glm::vec3(cam.getPosition()));
			//snowStrong->render(cam);
		}
		else{
			snowStrong->startTime();
		}

		if (useRainButton->isActive()){
			//rain->update(glm::vec3(cam.getPosition()));
			//rain->render(cam);
		}{
			rain->startTime();
		}

		////////////////////////////////WAITING FOR TEXTURES EMITTER////////////////////////////////

		//todo: flamme
		//fireMiddle->update();
		//fireMiddle->render(cam);
		//fireSmoke->update();
		//fireSmoke->render(cam);
		//firefly->update();
		//firefly->render(cam);
		//fireSparkOrange->update();
		//fireSparkOrange->render(cam);
		//fireSparkRed->update();
		//fireSparkRed->render(cam);

		// todo all
		//firework->update();
		//firework->render(cam);

		//smokeCloud
		//smokeCloud->update();
		//smokeCloud->render(cam);

		////////////////////////////////BASICS///////////////////////////////////////////////////////

		//renderer->renderGUI(*gui, window);

		//WINDOW
		window.swapAndPoll();

		//FPS
		if (!(outputFrames % 30)){
			outputFrames = 1;
			//std::cout << "FPS: " << static_cast<int> (1 / dTime) << "; Up to " << particleCount << " particles"<<std::endl;
		}
		outputFrames++;
	}
	glfwDestroyWindow(window.getWindow());
	glfwTerminate();

	return 0;
}