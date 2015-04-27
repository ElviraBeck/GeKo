#include "GeKo_Graphics/ParticleSystem/Emitter.h" //TODO Delete this line
#include "GeKo_Graphics/ParticleSystem/Effect.h"  //TODO Delete this line
#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"

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

//include data?
enum FLOW { UNUSED = -1, CONSTANT = 0, ONCE = 1 } output;

int main()
{
	glfwInit();

	//our window
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "PS - Blume", NULL, NULL);
	glfwMakeContextCurrent(window);

	cam.setName("PilotviewCam");
	cam.setPosition(glm::vec4(0, 0, 2.0, 1.0));
	cam.setNearFar(0.01f, 10.0f);

	// Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	// Callback
	glfwSetKeyCallback(window, key_callback);

	glewInit();

	Emitter* flower = new Emitter(0, glm::vec3(0.0,-1.0,0.0), 0.0, 0.003, 1, 15.0, true);
	flower->setVelocity(0);
	//flower->usePhysicPointGravity(glm::vec4(0.5, 0.5, 0.0, 1.0), 1.8, 1, 0.8);
	//flower->setComputeShader("/ParticleSystem/ParticleSystemFlower.comp");
	Effect* efFlower = new Effect();
	efFlower->addEmitter(flower);
	ParticleSystem* psFlower = new ParticleSystem(glm::vec3(0, 0, 0), efFlower);
	psFlower->start();

	float startCamTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		psFlower->update(cam);
		psFlower->render(cam);

		cam.setSensitivity(glfwGetTime() - startCamTime);
		startCamTime = glfwGetTime();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
