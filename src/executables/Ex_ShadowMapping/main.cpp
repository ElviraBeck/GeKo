#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include <GeKo_Graphics/Camera/Camera.h>


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//define camera (trackball)
InputHandler iH;

Pilotview cam("Pilotview");
Pilotview cam_shadow("Pilotview");

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

	Window testWindow(50, 50, WINDOW_WIDTH, WINDOW_HEIGHT, "ShadowMapping");
	glfwMakeContextCurrent(testWindow.getWindow());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	cam_shadow.setNearFar(1.f, 30.f);

  cam.setName("PilotviewCam");
  cam.setPosition(glm::vec4(1.0, 0.0, 5.0, 1.0));
  cam.setNearFar(0.01f, 100.0f);

	// Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	// Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();
	
	//load, compile and link Shader
	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsSM(loadShaderSource(SHADERS_PATH + std::string("/ShadowMapping/ShadowMap.vert")));
	FragmentShader fsSM(loadShaderSource(SHADERS_PATH + std::string("/ShadowMapping/ShadowMap.frag")));
	ShaderProgram shadowmapShader(vsSM, fsSM);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	//our renderer
	OpenGL3Context context;
	Renderer renderer(context);

	//OpenGL parameters
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);         
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//our object
	Cube cube;
	Rect plane;
	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();
	
	//our textures
	Texture bricks((char*)RESOURCES_PATH "/brick.bmp");
	Texture marble((char*)RESOURCES_PATH "/seamless_marble.jpg");
	Texture chrome((char*)RESOURCES_PATH "/chrome.jpg");

	//Scene creation 
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add cameras to scenegraph
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->addCamera(&cam_shadow);

	Node wallNode1("wall1");
	wallNode1.addGeometry(&plane);
	wallNode1.addTexture(&marble);
	wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(0.0, 0.1, 0.2)));
	wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), -90.0f, glm::vec3(1.0, 0.0, 0.0)));
	wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(1.5, 1.5, 1.5)));

	Node wallNode2("wall2");
	wallNode2.addGeometry(&plane);
	wallNode2.addTexture(&marble);
	wallNode2.setModelMatrix(glm::translate(wallNode2.getModelMatrix(), glm::vec3(0.0, 1.0, -0.2)));
	wallNode2.setModelMatrix(glm::scale(wallNode2.getModelMatrix(), glm::vec3(1.5, 1.5, 1.5)));

	Node cube1("cube1");
	cube1.addGeometry(&cube);
	cube1.addTexture(&bricks);
	cube1.setModelMatrix(glm::translate(cube1.getModelMatrix(), glm::vec3(-0.3, 0.25, 0.2)));
	cube1.setModelMatrix(glm::scale(cube1.getModelMatrix(), glm::vec3(0.3, 0.3, 0.3)));
  
	//Creating a scenegraph
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube1);

	//define light
	ConeLight slight(glm::vec4(1.0, 1.5, 2.5, 1.0), glm::vec4(0.2, 0.2, 0.2, 1.0), true, glm::vec3(-1.0, -1.0, -1.0), 90.0f, 5.0f, glm::radians(35.0f));

	//Set the position of the camera inside the light source
	glm::vec4 lightPosition = slight.m_position;
	cam_shadow.setPosition(lightPosition);

	//Set the lookAt point of the camera along the direction of the spot light
	glm::vec3 lookAt = glm::vec3(lightPosition) + (slight.m_direction);
	cam_shadow.setLookAt(lookAt);

	//Set the camera's field of view to the spot light's angle in degrees
	cam_shadow.setFOV(slight.m_angle);

	FBO sm_fbo(WINDOW_WIDTH, WINDOW_HEIGHT, 1, true, false);
	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);

	glm::vec3 lightAmbient(black);

	double startTime = glfwGetTime();

	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{	
		cam.setSensitivity(glfwGetTime() - startTime);
		startTime = glfwGetTime();

		///////////////////////////////////////////////////////////////////
		//Draw scene from the lights point of view to create a shadow map//
		///////////////////////////////////////////////////////////////////

		{
			testScene.getScenegraph()->setActiveCamera("shadow");

			//Bind Shadow Map FBO
			sm_fbo.bind();

			//Clear Shadow Map
			glClear(GL_DEPTH_BUFFER_BIT);

			//Set the shader for the light pass. This shader is highly optimized because the scene depth is the only thing that matters here!
			shadowmapShader.bind();		
			shadowmapShader.sendMat4("viewMatrix", cam_shadow.getViewMatrix());
			shadowmapShader.sendMat4("projectionMatrix", cam_shadow.getProjectionMatrix());

			//Render the scene
			testScene.render(shadowmapShader);

			//Restore the default framebuffer
			shadowmapShader.unbind();
			sm_fbo.unbind();
		}

		//////////////
		//Draw Scene//
		//////////////
		fboGBuffer.bind();

		//Clear Default Framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update Camera
		testScene.getScenegraph()->setActiveCamera("cam");

		shaderGBuffer.bind();

		shaderGBuffer.sendInt("useShadowMap", 1);

		shaderGBuffer.sendMat4("viewMatrix", cam.getViewMatrix());
		shaderGBuffer.sendMat4("projectionMatrix", cam.getProjectionMatrix());

		shaderGBuffer.sendVec4("light.pos", slight.m_position);
		shaderGBuffer.sendVec3("light.col", glm::vec3(slight.m_color));

		shaderGBuffer.sendVec3("light.spot_direction", slight.m_direction);
		shaderGBuffer.sendFloat("light.spot_exponent", slight.m_exponent);
		shaderGBuffer.sendFloat("light.spot_cutoff", slight.m_radius);

		shaderGBuffer.sendVec3("lightAmbient", lightAmbient);

		shaderGBuffer.sendInt("usePCFlinear", 0);

		//Shadow mapping
		glm::mat4 lightPerspective, lightView, lightMVPBias;
		lightPerspective = cam_shadow.getProjectionMatrix();
		//lightPerspective = glm::perspective(slight.m_angle, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.f, slight.m_radius);
		lightView = cam_shadow.getViewMatrix();
		//lightView = glm::lookAt(glm::vec3(slight.m_position), glm::vec3(0), glm::vec3(0, -1, 0));

		glm::mat4 sm_lightViewport(
			0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0,
			0.5, 0.5, 0.5, 1.0
			);

		//Build "shadow matrix"
    lightMVPBias = sm_lightViewport * lightPerspective * lightView;
		shaderGBuffer.sendMat4("lightVPBias", lightMVPBias);

		shaderGBuffer.sendVec3("mat.diffuse", darkgrey);
		shaderGBuffer.sendVec3("mat.specular", grey);
		shaderGBuffer.sendFloat("mat.shininess", 100.0f);
		shaderGBuffer.sendFloat("mat.alpha", 1.0f);

		shaderGBuffer.sendInt("useTexture", 1);

		//Bind and Pass shadow map. Only use SHADOW_TEXTURE_UNIT when Normal Mapping is applied.
		shaderGBuffer.sendSampler2D("depthTexture", sm_fbo.getDepthTexture(), 1);

		testScene.render(shaderGBuffer);
		//screenFillingQuad.renderGeometry();

		shaderGBuffer.unbind();
		fboGBuffer.unbind();


		//ScreenFillingQuad Render Pass
		shaderSFQ.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderSFQ.sendSampler2D("fboTexture", fboGBuffer.getColorTexture(2), 2);

		screenFillingQuad.renderGeometry();

		shaderSFQ.unbind();

		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
	}
	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}

