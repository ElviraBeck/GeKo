#include <GeKo_Graphics/include.h>

/*
To use the FBO u just need to set an FBO, bind it and unbind it after the render pass
and then you can use the textures
*/

InputHandler iH;
Trackball cam("Trackball");

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();
	activeMap.at(key)();
}

int main()
{
	glfwInit();

	//our window
	Window window(550, 50, 800, 600, "FBO-Depth", glm::vec3(0.2, 0.0, 0.0));

	//Set Camera to another position
	cam.setPosition(glm::vec4(0.0, 0.0, 10.0, 1.0));
	cam.setName("TrackballCam");
	cam.setNearFar(1.0,100);

	//Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Trackball");

	//Callback
	glfwSetKeyCallback(window.getWindow(), key_callback);

	glewInit();

	//our shader
	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/minimalGBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/minimalGBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	//our fbos
	FBO fbo(800, 600, 3, true, false);

	//our object
	Cube cube;
	Teapot tea;
	Rect rect;
	rect.loadBufferData();

	//our textures
	Texture texCV((char*)RESOURCES_PATH "/Symbol/cv_logo.bmp");
	Texture texBrick((char*)RESOURCES_PATH "/Wall/brick.bmp");

	//Scene creation 
	Level level("level");
	Scene scene("scene");
	level.addScene(&scene);
	level.changeScene("scene");

	//Add Camera to scenegraph
	scene.getScenegraph()->addCamera(&cam);
	scene.getScenegraph()->getCamera("TrackballCam");
	scene.getScenegraph()->setActiveCamera("TrackballCam");

	//Create Nodes with geometry and texture
	Node teaNode("teaNode");
	teaNode.addGeometry(&tea);
	teaNode.addTexture(&texCV);
	teaNode.setModelMatrix(glm::translate(teaNode.getModelMatrix(), glm::vec3(0.0, 0.0, 0.0)));

	Node brickNode("brickNode");
	brickNode.addGeometry(&cube);
	brickNode.addTexture(&texBrick);
	brickNode.setModelMatrix(glm::translate(brickNode.getModelMatrix(), glm::vec3(3.0, 0.0, 0.0)));

	//Creating a scenegraph
	scene.getScenegraph()->getRootNode()->addChildrenNode(&teaNode);
	teaNode.addChildrenNode(&brickNode);

	//Gameloop
	while (!glfwWindowShouldClose(window.getWindow()))
	{
		//GBuffer Render Pass
		fbo.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderGBuffer.bind();
		shaderGBuffer.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderGBuffer.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		shaderGBuffer.sendInt("useTexture", 1);
		teaNode.setModelMatrix(glm::rotate(teaNode.getModelMatrix(), 0.02f, glm::vec3(0.0, 1.0, 0.0)));
		scene.render(shaderGBuffer);
		shaderGBuffer.unbind();
		fbo.unbind();

		//ScreenFillingQuad Render Pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderSFQ.bind();
		shaderSFQ.sendSampler2D("fboTexture", fbo.getDepthTexture());
		rect.renderGeometry();
		shaderSFQ.unbind();

		glfwSwapBuffers(window.getWindow());
		glfwPollEvents();
	}
	glfwDestroyWindow(window.getWindow());
	glfwTerminate();
	return 0;
}