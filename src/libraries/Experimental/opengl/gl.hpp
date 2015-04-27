#pragma once

#include <vector>
#include <array>
#include <iostream>
#include "glm/common.hpp"
#include "glm/mat4x4.hpp"
#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
enum class TextureTarget{
  Texture2D = GL_TEXTURE_2D,
};
enum class TextureFormat{
  RGB = GL_RGB
};
enum class TextureType{
  UnsignedByte = GL_UNSIGNED_BYTE
};

enum class FrameBufferTarget{
  ColorAttachment = GL_COLOR_ATTACHMENT0
};

enum class BufferUsage{
  StaticDraw = GL_STATIC_DRAW,
  DynamicDraw = GL_DYNAMIC_DRAW,
};
enum class Type{
  UnsignedInt = GL_UNSIGNED_INT
};

enum class DrawMode{
  Triangles = GL_TRIANGLES,
};

enum class ShaderTarget{
  VertexShaderBit = GL_VERTEX_SHADER_BIT,
  FragmentShaderBit = GL_FRAGMENT_SHADER_BIT,
};

enum class BufferCopy{
  CopyRead = GL_COPY_READ_BUFFER,
  CopyWrite = GL_COPY_WRITE_BUFFER,
};

enum class BufferTarget{
  ArrayBuffer = GL_ARRAY_BUFFER,
  AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
  Texture = GL_TEXTURE_BUFFER,
  Uniform = GL_UNIFORM_BUFFER,
  ElementArray = GL_ELEMENT_ARRAY_BUFFER
};

namespace gl{
static GLenum BUFFER_TARGET = GL_ARRAY_BUFFER;
struct Buffer{
  GLuint handle;
};
Buffer createBuffer(){
  GLuint buffer = 0;
  glGenBuffers(1,&buffer);
  return Buffer{buffer};
}

void namedBufferData(	Buffer buffer,
                      GLsizei size,
                      const void *data,
                      BufferUsage usage){
  glBindBuffer(BUFFER_TARGET,buffer.handle);
  glBufferData(BUFFER_TARGET,size,data,static_cast<GLenum>(usage));
}
template<class T>
void namedBufferData(	Buffer buffer,
                      const std::vector<T> &data,
                      BufferUsage usage){
  glBindBuffer(BUFFER_TARGET,buffer.handle);
  glBufferData(BUFFER_TARGET,data.size() * sizeof(T),&data.at(0),static_cast<GLenum>(usage));
}
template<class T,size_t N>
void namedBufferData(	Buffer buffer,
                      const std::array<T,N> &data,
                      BufferUsage usage){
  glBindBuffer(BUFFER_TARGET,buffer.handle);
  glBufferData(BUFFER_TARGET,N * sizeof(T),&data[0],static_cast<GLenum>(usage));
}
struct VertexArray{
  GLuint handle;
};

void bindVertexArray(VertexArray vao){
  glBindVertexArray(vao.handle);
}

VertexArray createVertexArray(){
  GLuint handle;
  glGenVertexArrays(1,&handle);
  return VertexArray{handle};
}
void enableVertexArrayAttrib(VertexArray vao,GLuint pos){
  glBindVertexArray(vao.handle);
  glEnableVertexAttribArray(pos);
}
void vertexArrayBindingDivisor(VertexArray vao,
                               GLuint bindingindex,
                               GLuint divisor){
  glBindVertexArray(vao.handle);
  glVertexBindingDivisor(bindingindex,divisor);
}

void vertexAttribFormat(VertexArray vao,GLuint attribindex,
                        GLint size,
                        GLenum type,
                        GLboolean normalized,
                        GLuint relativeoffset){
  glBindVertexArray(vao.handle);
  glVertexAttribFormat(attribindex,size,type,normalized,relativeoffset);
}
void bindVertexBuffer(VertexArray vao,
                      GLuint bindingindex,
                      Buffer buffer,
                      GLintptr offset,
                      GLintptr stride){
  glBindVertexArray(vao.handle);
  glBindVertexBuffer(bindingindex,buffer.handle,offset,stride);
}

void vertexArrayAttribBinding(VertexArray vao,
                              GLuint attribindex,
                              GLuint bindingindex){
  glBindVertexArray(vao.handle);
  glVertexAttribBinding(attribindex,bindingindex);
}

struct Program{
  GLuint handle;
};
Program createShaderProgram(GLenum type,
                            GLsizei count,
                            const char **strings){
  const GLuint shader = glCreateShader(type);
  if (shader) {
    glShaderSource(shader, count, strings, NULL);
    glCompileShader(shader);
    const GLuint program = glCreateProgram();
    if (program) {
      GLint compiled = GL_FALSE;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
      glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
      if (compiled) {
        glAttachShader(program, shader);
        glLinkProgram(program);
        glDetachShader(program, shader);
      }
      else{
        /* append-shader-info-log-to-program-info-log */
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar* infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

        std::cout << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
        delete[] infoLog;
      }
    }
    glDeleteShader(shader);
    GLint bLink;
    glGetProgramiv(program, GL_LINK_STATUS,&bLink);
    std::cout << "Link: " << bLink << std::endl;
    return Program{program};
  } else {
    return Program{0};
  }
}

void useProgram(Program p){
  glUseProgram(p.handle);
}
struct BaseShader{
  GLuint handle;
  BaseShader(const char *shaderSource, GLenum shaderType){
    handle = glCreateShader(shaderType);
    glShaderSource(handle, 1, &shaderSource, NULL);
    glCompileShader(handle);
    GLint compiled = GL_FALSE;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
    /* append-handle-info-log-to-program-info-log */
    if(!compiled){
      GLint infoLogLength;
      glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar* infoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(handle, infoLogLength, NULL, infoLog);

      std::cout << "ERROR: Unable to compile shader" << std::endl << infoLog << std::endl;
      delete[] infoLog;
    }
  }
};
struct VertexShader : public BaseShader{
  VertexShader(const char *shaderSource) : BaseShader(shaderSource, GL_VERTEX_SHADER){}
};
struct FragmentShader : public BaseShader{
  FragmentShader(const char *shaderSource) : BaseShader(shaderSource, GL_FRAGMENT_SHADER){}
};

Program createProgram(VertexShader vs, FragmentShader fs){
  GLuint p = glCreateProgram();
  glAttachShader(p,vs.handle);
  glAttachShader(p,fs.handle);
  glLinkProgram(p);
  return Program{p};
}
void namedBufferStorage(Buffer buffer,
                        GLsizei size,
                        const void *data,
                        GLbitfield flags){
  glBindBuffer(BUFFER_TARGET,buffer.handle);
  glBufferStorage(BUFFER_TARGET,size,data,flags);

}
template<class T>
void namedBufferStorage(Buffer buffer,
                        std::vector<T> data,
                        GLbitfield flags){
  glBindBuffer(BUFFER_TARGET,buffer.handle);
  glBufferStorage(BUFFER_TARGET,data.size() * sizeof(T),&data[0],flags);
}
template<class T,size_t N>
void namedBufferStorage(Buffer buffer,
                        std::array<T,N> data,
                        GLbitfield flags){
  glBindBuffer(BUFFER_TARGET,buffer.handle);
  glBufferStorage(BUFFER_TARGET,N * sizeof(T),&data[0],flags);
}
void vertexArrayElementBuffer(VertexArray vao,
                              Buffer buffer){
  glBindVertexArray(vao.handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer.handle);
}
void programUniform(Program p,GLint loc,GLsizei count,GLboolean transpose,const glm::mat4 &mat){
  glProgramUniformMatrix4fv(p.handle,loc,count,transpose,glm::value_ptr(mat));
}

struct Pipeline{
  GLuint handle;
};
Pipeline createPipeline(){
  GLuint pipeline;
  glGenProgramPipelines(1,&pipeline);
  return Pipeline{pipeline};
}
void useProgramStages(Pipeline pipeline, ShaderTarget target, Program program){
  glUseProgramStages(pipeline.handle,static_cast<GLenum>(target),program.handle);
}
void bindProgramPipeline(Pipeline p){
  glBindProgramPipeline(p.handle);
}

void drawArrays(DrawMode mode,GLint first,GLsizei count){
  glDrawArrays(static_cast<GLenum>(mode),first,count);
}
void drawElements(DrawMode mode,GLsizei count,Type type, const GLvoid* ind ){
  glDrawElements(static_cast<GLenum>(mode),count,static_cast<GLenum>(type),ind);
}

void drawElementsInstanced(DrawMode mode,GLsizei count,Type type,const void *indices, GLsizei primcount ){
  glDrawElementsInstanced(static_cast<GLenum>(mode),count,static_cast<GLenum>(type),indices, primcount);

}
void free(Buffer b){
  glDeleteBuffers(1,&b.handle);
}
void free(VertexArray vao){
  glDeleteVertexArrays(1,&vao.handle);
}
struct Texture{
  GLuint handle;
};
Texture createTexture(){
  GLuint handle;
  glGenTextures(1,&handle);
  return Texture{handle};
}
void bindTexture(TextureTarget target, Texture tex){
  glBindTexture(static_cast<GLenum>(target),tex.handle);
}

void texImage2D(TextureTarget target,
                GLint level,
                GLint internalFormat,
                GLsizei width,
                GLsizei height,
                GLint border,
                TextureFormat format,
                TextureType type,
                const GLvoid * data){
  glTexImage2D(static_cast<GLenum>(target),level,internalFormat,width,height,border,static_cast<GLenum>(format),static_cast<GLenum>(type),data);

}

struct Framebuffer{
  GLuint handle;
};
Framebuffer createFramebuffer(){
  GLuint handle;
  glGenFramebuffers(1,&handle);
  return Framebuffer{handle};
}
void bindFramebuffer(Framebuffer fb){
  glBindFramebuffer(GL_FRAMEBUFFER,fb.handle);
}

void namedFramebufferTexture(Framebuffer fb,FrameBufferTarget target, GLuint targetOffset,GLuint texture,GLuint level){
  bindFramebuffer(fb);
  glFramebufferTexture(fb.handle,static_cast<GLenum>(target) + targetOffset, texture,level);
}


}
