#include "GeKo_Graphics/Renderer/Renderer.h"
#include "GeKo_Graphics/Shader/Shader.h"
#include "GeKo_Graphics/Geometry/Teapot.h"
#include "GeKo_Graphics/Camera/Trackball.h"
#include "GeKo_Gameplay/Input/InputHandler.h"

/* This main is an example to test/implement the Trackball combined with key input.
The output is shown on the console and the window.

To use the Trackball camera combined with the input system, you need an InputHandler, the Trackball camera and the key callback.
Furthermore you should compute delta time because of the frame rate and uniform variables because of the matrices of the camera.
The InputHandler sets all InputMaps and you need to enable one InputMap.
You have to set the start time and compute the delta time and set the sensitivity.
You also have to insert the key callback and set it before the renderloop.
The uniform matrices of the camera have to be send to the shader in the render loop.
*/

InputHandler iH;
Trackball cam("Trackball");

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

int main()
{
	glfwInit();

	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "TrackballTest", NULL, NULL);
	glfwMakeContextCurrent(window);

	// You can set the Trackball camera to another position and give it a name
	cam.setPosition(glm::vec4(0.0,0.0,10.0,1.0));
	cam.setName("TrackballCam");
	cam.setNearFar(0.01f, 100.0f);

	// Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Trackball");

	// Callback
	glfwSetKeyCallback(window, key_callback);

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

		// You have to send the uniform matrices of the Trackball camera to the shader
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