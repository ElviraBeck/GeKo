#include <GeKo_Graphics/Renderer/Renderer.h>
#include <GeKo_Graphics/Shader/Shader.h>
#include <GeKo_Graphics/Geometry/Cube.h>
#include <GeKo_Graphics/Camera/Camera.h>
#include <GeKo_Graphics/Camera/Trackball.h>
#include <GeKo_Gameplay/Input/InputHandler.h>
#include <GeKo_Gameplay/Input/InputMap.h>
#include <GeKo_Graphics/Geometry/Teapot.h>
#include <GeKo_Graphics/Material/Texture.h>
#include <GeKo_Graphics/Scenegraph/Scene.h>
#include <GeKo_Graphics/Scenegraph/Node.h>
#include <GeKo_Graphics/Window.h>

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

    //our window
    Window window(550, 50, 400, 300, "Renderer Example", glm::vec3(0.2, 0.0, 0.0));

    glewInit();

	//our shader
    VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader/ColorShader.vert")));
    FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader/ColorShader.frag")));
    ShaderProgram shader(vs, fs);
    
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
	
		window.swapAndPoll();
    }

    window.close();
    return 0;
}
