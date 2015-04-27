#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Experimental/opengl/gl.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <array>
#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace gl;
using namespace glm;
std::vector<GLfloat> cube_vertices = {
  // front
  -1.0, -1.0,  1.0,
  1.0, -1.0,  1.0,
  1.0,  1.0,  1.0,
  -1.0,  1.0,  1.0,
  // back
  -1.0, -1.0, -1.0,
  1.0, -1.0, -1.0,
  1.0,  1.0, -1.0,
  -1.0,  1.0, -1.0,
};
std::vector<GLfloat> cube_colors = {
  // front colors
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
  1.0, 1.0, 1.0,
  // back colors
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
  1.0, 1.0, 1.0,
};
std::vector<GLuint> cube_elements = {
  // front
  0, 1, 2,
  2, 3, 0,
  // top
  3, 2, 6,
  6, 7, 3,
  // back
  7, 6, 5,
  5, 4, 7,
  // bottom
  4, 5, 1,
  1, 0, 4,
  // left
  4, 0, 3,
  3, 7, 4,
  // right
  1, 5, 6,
  6, 2, 1,
};
static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{

  const char *vs_source =
      "#version 440\n\
      uniform mat4 proj;\n\
      layout(location = 0) in vec3 position;\n\
      layout(location = 1) in vec3 color;\n\
      layout(location = 2) in mat4 model;\n\
      out vec4 fs_color;\n\
      out gl_PerVertex\n\
  {\n\
        vec4 gl_Position;\n\
  };\n\
      void main() {\n\
        fs_color = vec4(color,1);\n\
        gl_Position = proj * model *  vec4(position,1);\n\
                  }";

  const char *fs_source =
      "#version 440\n\
      in vec4 fs_color;\n\
      out vec4 out_color;\n\
      void main() {\n\
        out_color = fs_color;\n\
                  }";
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  window = glfwCreateWindow(1600, 900, "Simple example", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_BUFFER_BIT);

  auto position_buffer= createBuffer();
  namedBufferData(position_buffer,cube_vertices,BufferUsage::StaticDraw);
  auto index_buffer = createBuffer();
  namedBufferData(index_buffer,cube_elements,BufferUsage::StaticDraw);
  auto color_buffer = createBuffer();
  namedBufferData(color_buffer,cube_colors,BufferUsage::StaticDraw);

  // 1 Shader goes into 1 Program
  auto vsp = createShaderProgram(GL_VERTEX_SHADER,1,&vs_source);
  auto fsp = createShaderProgram(GL_FRAGMENT_SHADER,1,&fs_source);
  // A Pipeline holds different shaders programs that can be hotswapped at runtime
  auto pipeline = createPipeline();
  useProgramStages(pipeline,ShaderTarget::VertexShaderBit,vsp);
  useProgramStages(pipeline,ShaderTarget::FragmentShaderBit,fsp);
  bindProgramPipeline(pipeline);

  // New Vao system, only stores the state of the VAO, not the buffers.
  auto vao = createVertexArray();
  enableVertexArrayAttrib(vao,0);
  vertexArrayAttribBinding(vao,0,0);
  vertexAttribFormat(vao,
                     0,
                     3,
                     GL_FLOAT,
                     GL_FALSE,
                     0);
  enableVertexArrayAttrib(vao,1);
  vertexArrayAttribBinding(vao,1,1);
  vertexAttribFormat(vao,
                     1,
                     3,
                     GL_FLOAT,
                     GL_FALSE,
                     0);
  enableVertexArrayAttrib(vao,2);
  vertexArrayAttribBinding(vao,2,2);
  vertexAttribFormat(vao,
                     2,
                     4,
                     GL_FLOAT,
                     GL_FALSE,
                     0);
  vertexArrayElementBuffer(vao,index_buffer);
  /* This is instancing. It means that input at location 2 should be invoked only 1 time per
   * shader call. Use the draw instanced command.
   */
  vertexArrayBindingDivisor(vao,2,2);

  float angle = 0.f;
  glm::mat4 proj = glm::perspective(45.0f, 16.f/9.f, 0.1f, 100.0f);
  while (!glfwWindowShouldClose(window))
  {
    angle += 0.01f;
    if(angle > 2 * glm::pi<float>()){
      angle = 0.f;
    }
    auto rot = glm::rotate(glm::mat4(1.0f),angle ,glm::vec3(0,1,0));

    // The offsetbuffer that changes every frame
    std::vector<glm::mat4> offset_data = {
      glm::translate(glm::mat4(1.0f),vec3(-8.0,0.0,0.0)) *rot,
      glm::translate(glm::mat4(1.0f),vec3(-4.0,0.0,0.0)) *rot,
      glm::translate(glm::mat4(1.0f),vec3(0.0,0.0,0.0))  *rot,
      glm::translate(glm::mat4(1.0f),vec3(4.0,0.0,0.0))  *rot,
      glm::translate(glm::mat4(1.0f),vec3(8.0,0.0,0.0))  *rot,
    };
    auto offset_buffer = createBuffer();
    namedBufferData(offset_buffer,offset_data,BufferUsage::DynamicDraw);
    glClear(GL_COLOR_BUFFER_BIT);
    bindVertexBuffer(vao,
                     0,
                     position_buffer,
                     0,
                     3 * sizeof(GLfloat));
    bindVertexBuffer(vao,
                     1,
                     color_buffer,
                     0,
                     3 * sizeof(GLfloat));
    // Loads a matrix into a buffer. Note that a matrix in opengl is composed of 4 vectors.
    for(size_t i = 0; i < 4; ++i){
      enableVertexArrayAttrib(vao,2+i);
      vertexArrayBindingDivisor(vao,2+i,1);
      bindVertexBuffer(vao,
                       2 + i,
                       offset_buffer,
                       i * sizeof(GLfloat) * 4,
                       sizeof(glm::mat4x4));
    }

    bindVertexArray(vao);
    glm::mat4 view = lookAt(glm::vec3(0,0,10),glm::vec3(0,0,0),glm::vec3(0,1,0));
    auto trans = glm::translate(glm::mat4(1.0f),glm::vec3(0,1,0));

    auto model =  trans * rot;
    glm::mat4 mvp = proj * view;
    programUniform(vsp,0,1,GL_FALSE,mvp);
    drawElementsInstanced(DrawMode::Triangles,cube_elements.size(),Type::UnsignedInt,0,offset_data.size());
    glfwSwapBuffers(window);
    glfwPollEvents();
    gl::free(offset_buffer);
  }
  gl::free(position_buffer);
  gl::free(index_buffer);
  gl::free(color_buffer);
  gl::free(vao);
  glfwDestroyWindow(window);
  glfwTerminate();
}
