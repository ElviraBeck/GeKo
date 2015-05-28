#include "GeKo_Graphics/Renderer/Renderer.h"
#include "GeKo_Graphics/Shader/Shader.h"
#include "GeKo_Graphics/Light/PointLight.h"
#include "GeKo_Graphics/Scenegraph/Scene.h"
#include "GeKo_Graphics/Scenegraph/Node.h"
#include "GeKo_Graphics/Geometry/Cube.h"
#include "GeKo_Graphics/Material/Texture.h"

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

	//our window
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "NormalMapping", NULL, NULL);
	glfwMakeContextCurrent(window);

	//Set Camera to another position
	cam.setPosition(glm::vec4(0.0, 0.0, 2.0, 1.0));
	cam.setName("PilotviewCam");

	//Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(window, key_callback);

	glewInit();

	//our NormalMapping shader
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/NormalMapping/NormalMapping.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/NormalMapping/NormalMapping.frag")));
	ShaderProgram shaderBump(vs, fs);
	
	//our Textures
	Texture normalmap((char*)RESOURCES_PATH "/NormalMapping/face_normal.png");
	Texture diffusemap((char*)RESOURCES_PATH "/NormalMapping/face_diffuse.png");
	//Texture normalmapStone((char*)RESOURCES_PATH "/Stone/normalmap.jpg");
	//Texture diffusemapStone((char*)RESOURCES_PATH "/Stone/diffusemap.jpg");
	
	//our object
	Cube cube;

	//our Light
	PointLight plight(glm::vec4(0.0f, 1.0, 10.0f, 1.0f), glm::vec4(0.6, 0.6, 0.6, 1.0), true, 0);

	//our Node
	Scene* scene;
	Node cubeNode("cubeNode");
	cubeNode.addGeometry(&cube);
	cubeNode.addTexture(&diffusemap);
	cubeNode.addNormalMap(&normalmap);
	cubeNode.setModelMatrix(glm::translate(cubeNode.getModelMatrix(), glm::vec3(0.0, 0.0, 0.0)));

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);
		
	//for a rotating cube
	double startCamTime = glfwGetTime();
	float startRotation = glfwGetTime();

	glClearColor(1.0, 1.0, 1.0, 0.0);

    //Renderloop
	while (!glfwWindowShouldClose(window))
	{
		//essentials
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//increase rotation
		float deltaTime = glfwGetTime() - startRotation;
		startRotation = glfwGetTime();
		cubeNode.setModelMatrix(glm::rotate(cubeNode.getModelMatrix(), 10.0f * deltaTime, glm::vec3(0.0, 1.0, 0.0)));

		//Camera update
		cam.setSensitivity(glfwGetTime() - startCamTime);
		startCamTime = cam.getSensitivity();

		//shader updates
		shaderBump.bind();
		shaderBump.sendMat4("modelMatrix", cubeNode.getModelMatrix());
		shaderBump.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderBump.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		shaderBump.sendLightData("pos", "col", "dir", "exp", "ang", "rad", plight);
		shaderBump.sendSampler2D("colortexture", cubeNode.getTexture()->getTexture(), 0);
		shaderBump.sendSampler2D("normaltexture", cubeNode.getNormalMap()->getTexture(), 1);
		cubeNode.render();
		shaderBump.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}
