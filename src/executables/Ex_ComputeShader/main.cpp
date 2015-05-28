#include "GeKo_Graphics/Renderer/Renderer.h"
#include "GeKo_Graphics/Shader/Shader.h"
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

enum
{
	PARTICLE_GROUP_SIZE = 128,		//"in" variable in the ComputeShader
	PARTICLE_GROUP_COUNT = 1000,	//number of Particles in a Compute Group
	PARTICLE_COUNT = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT),
};

int main() {
	glfwInit();

	//our window
	GLFWwindow* window;
	window = glfwCreateWindow(800, 600, "ComputeShader", NULL, NULL);
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
	
	/*THIS IS WHERE THE MAGIC HAPPENS*/

	ComputeShader cs(loadShaderSource(SHADERS_PATH + std::string("/ComputeShader/simpleComputeShader2.comp")));
	ShaderProgram compute(cs);

	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ComputeShader/ExComputeShader.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ComputeShader/ExComputeShader.frag")));
	ShaderProgram render(vs, fs);

	GLuint render_vao;
	glGenVertexArrays(1, &render_vao);
	glBindVertexArray(render_vao);

	GLuint position_buffer;
	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(glm::vec4), NULL, GL_DYNAMIC_COPY);
	// Map the position buffer and fill it with random vectors
	glm::vec4 * positions = (glm::vec4 *)
		glMapBufferRange(GL_ARRAY_BUFFER,
		0,
		PARTICLE_COUNT * sizeof(glm::vec4),
		GL_MAP_WRITE_BIT |
		GL_MAP_INVALIDATE_BUFFER_BIT);
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i].x = ((rand() % 100) / 100.0f) - 0.5;
		positions[i].y = ((rand() % 100) / 100.0f) - 0.5;
		positions[i].z = ((rand() % 100) / 100.0f) - 0.5;
		positions[i].w = 1;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	//Textur
	GLuint tbo;
	glGenTextures(1, &tbo);

	glBindTexture(GL_TEXTURE_BUFFER, tbo);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, position_buffer);

	/*END*/

	//glClearColor(1.0, 1.0, 1.0, 1.0); //BUUUG
	float startCamTime = glfwGetTime();
	
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(window))
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		cam.setSensitivity(glfwGetTime() - startCamTime);
		startCamTime = cam.getSensitivity();

		compute.bind();
		compute.sendFloat("time", glfwGetTime());
		glBindImageTexture(0, tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		render.bind();
		render.sendMat4("viewMatrix", cam.getViewMatrix());
		render.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		glBindVertexArray(render_vao);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
		render.unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}