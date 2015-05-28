#include "GeKo_Graphics/Renderer/Renderer.h"
#include "GeKo_Graphics/Shader/Shader.h"
#include "GeKo_Graphics/Geometry/Teapot.h"
#include "GeKo_Graphics/Camera/Playerview.h"
#include "GeKo_Gameplay/Input/InputHandler.h"

/* This main is an example to test/implement the Playerview combined with key and mouse input.
The output is shown on the console and the window.

To use the Playerview camera combined with the input system, you need an InputHandler, the Trackball camera and the key and cursor position callback.
Furthermore you should compute delta time because of the frame rate and uniform variables because of the matrices of the camera.
The InputHandler sets all InputMaps and you need to enable one InputMap.
You have to set the start time and compute the delta time and set the sensitivity.
You also have to insert the key and the cursor position callback and set them before the renderloop.
The uniform matrices of the camera have to be send to the shader in the render loop.
*/

InputHandler iH;
Playerview cam("Playerview");

// As we do not use the Node class, we can't set the teapot into the middle of the window, therefore is the change of the direction improperly limited
// just needed in the player mode
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
	if (iH.getActiveInputMap()->getName() == "Playerview"){
		cam.turn(xpos, ypos);
	}
	else{}
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

int main()
{
	glfwInit();

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "PlayerviewMouseTest", NULL, NULL);
	glfwMakeContextCurrent(window);
		
	// You have to give the camera a name
	cam.setName("Playerview");
	cam.setPosition(glm::vec4(1.0, 0.0, 5.0, 1.0));
	cam.setNearFar(0.01f, 100.0f);
	
	// Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Playerview");

	// Callback
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	glewInit();

	// Shader
    VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.vert")));
    FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/colorShader3D.frag")));
	ShaderProgram shader(vs, fs);
    
	// Renderer
    OpenGL3Context context;
    Renderer renderer(context);
	
	// Object
	Teapot teapot;
	teapot.loadBufferData();

	// getting the start time
	double startTime = glfwGetTime();
	
    while (!glfwWindowShouldClose(window))
    {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// You have to compute the delta time
		cam.setSensitivity(glfwGetTime() - startTime);
		startTime = glfwGetTime();

		shader.bind();

		// You have to send the uniform matrices of the Pilotview camera to the shader
		shader.sendMat4("viewMatrix", cam.getViewMatrix());
		shader.sendMat4("projectionMatrix", cam.getProjectionMatrix());
	
		teapot.renderGeometry();
		shader.unbind();
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}