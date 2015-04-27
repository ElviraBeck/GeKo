#include "GL/glew.h"
#include "GeKo_Graphics/Camera/Trackball.h"
#include "GeKo_Gameplay/Input/InputHandler.h"

/* This main is an example to test/implement the Input (of keys).
The output is shown on the console only.

To use the input system, you need one InputHandler, at least one camera and the key callback.
The InputHandler sets all InputMaps and you need to enable one InputMap.
In this example you just have to use the camera because the InputHandler needs always to know which camera is active
You also have to insert the key callback and set it before the renderloop
*/

InputHandler iH;
// alibiCam, because here we have just output on the console
Trackball alibiCam("Trackball");

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

int main(void)
{
	glfwInit();

	GLFWwindow* window;
	window = glfwCreateWindow(640, 480, "InputTest", NULL, NULL);
	glfwMakeContextCurrent(window);

	iH.setAllInputMaps(alibiCam);
	// Setting an InputMap active so that action can be performed
	iH.changeActiveInputMap("Trackball");

	//Callback
	glfwSetKeyCallback(window, key_callback);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}