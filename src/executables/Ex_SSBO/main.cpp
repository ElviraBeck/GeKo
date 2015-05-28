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

struct pos{
	float x, y, z, w;
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
	
	/*MAGIC*/

	ComputeShader cs(loadShaderSource(SHADERS_PATH + std::string("/ComputeShader/simpleComputeShader.comp")));
	ShaderProgram compute(cs);

	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ComputeShader/ExComputeShader.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ComputeShader/ExComputeShader.frag")));
	ShaderProgram render(vs, fs);

	//GLuint render_vao;
	//glGenVertexArrays(1, &render_vao);
	//glBindVertexArray(render_vao);

	GLuint position_ssbo;
	glGenBuffers(1, &position_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, PARTICLE_COUNT * sizeof(struct pos), NULL, GL_STATIC_DRAW);

	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
	struct pos* positions = (struct pos*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, PARTICLE_COUNT * sizeof(struct pos), bufMask);
	for (int i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i].x = ((rand() % 100) / 100.0f) - 0.5;
		positions[i].y = ((rand() % 100) / 100.0f) - 0.5;
		positions[i].z = ((rand() % 100) / 100.0f) - 0.5;
		positions[i].w = 1;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	//glEnableVertexAttribArray(0);

	//std::cout << "HIER MUSS 000 STEHEN:   " << positions[1].x << positions[1].y << positions[1].z << std::endl;
	/*END*/

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
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, position_ssbo);
		compute.sendFloat("time", glfwGetTime());
		glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
		compute.unbind();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		render.bind();
		glBindBuffer(GL_ARRAY_BUFFER, position_ssbo);
		render.sendMat4("viewMatrix", cam.getViewMatrix());
		render.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		glVertexPointer(4, GL_FLOAT, 0, (void*)0);
		glEnableClientState(GL_VERTEX_ARRAY);
		//glPointSize(1.0);
		glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		render.unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}