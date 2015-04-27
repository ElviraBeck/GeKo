#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include <GeKo_Debugger/Debugger.h>

/*
Renderer example:

1. per hand we build a quad
2. we set a window
3. set the shader with a fragmentshader & vertexshader
4. set a renderer and give him the context
5. set a vec3 buffer with the quad
6. gameloop
6.1 shader binding
6.2 fragmentShader get an uniform var
6.3 rendering & unbinding the shader
*/
int main()
{
    //our example quad
  std::vector<glm::vec3> vertices;
  vertices.push_back(glm::vec3(0.5, -0.5, 0.0));
  vertices.push_back(glm::vec3(-0.5, -0.5, 0.0));
  vertices.push_back(glm::vec3(0.5, 0.5, 0.0));
  vertices.push_back(glm::vec3(-0.5, 0.5, 0.0));
  
  glfwInit();

	//the debugger
  Debugger debugger;

  //our window
  Window window(550, 50, 800, 600, "Renderer Example");
 
  glewInit();

  //set debug callback
  debugger.setDebugCallback();	
	
	//our shader
  VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader/ColorShader.vert")));
  FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader/ColorShader.frag")));
  ShaderProgram shader(vs, fs);
  
  //Make sure that ALL occuring errors will be recorded
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	//Generate some fake-errors for testing
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0x4752415A, GL_DEBUG_SEVERITY_HIGH, -1, "Test Message 1");
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0x4752414A, GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Test Message 2");
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0x4752414A, GL_DEBUG_SEVERITY_LOW, -1, "Test Message 3");


	//our renderer
  OpenGL3Context context;
  Renderer renderer(context);
  Buffer<glm::vec3> buffer(vertices,STATIC_DRAW);

  //Gameloop
  while (!glfwWindowShouldClose(window.getWindow()))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shading & rendering
      shader.bind();
      shader.sendVec4("color", glm::vec4(0.5,0.2,0.8,1.0));
      renderer.draw(buffer);
      shader.unbind();

      glfwSwapBuffers(window.getWindow());
      glfwPollEvents();
    }

    window.close();
    return 0;
}
