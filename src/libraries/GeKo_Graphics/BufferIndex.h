#pragma once

#include <vector>
#include "GL/glew.h"
#include "glm/vec3.hpp"

/**This class provides one single Buffer-Object which can be filled with Object informations like Normals, Vertices, UVs etc.*/
enum BufferIndexDraw{
  STATIC_DRAW_INDEX = GL_STATIC_DRAW,
  DYNAMIC_DRAW_INDEX = GL_DYNAMIC_DRAW
};
template<typename T>
struct BufferIndex{
  GLuint handle;
  size_t size;
  BufferIndex(const std::vector<T> &v, BufferIndexDraw bufferDraw){
    size = v.size();
    glGenBuffers(1, &handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), &v.at(0), bufferDraw);
    unbind();
  }

  ~BufferIndex(){
    glDeleteBuffers(1, &handle);
  }
  void bind() const{
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
  }
  void unbind() const{
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
};