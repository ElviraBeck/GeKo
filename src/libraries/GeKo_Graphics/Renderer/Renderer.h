#pragma once

#include <GeKo_Graphics/Defs.h>
#include <GeKo_Graphics/Buffer.h>
#include <GeKo_Graphics/Shader/Shader.h>
#include <GeKo_Graphics/Scenegraph/Scene.h>
#include <GeKo_Graphics/Shader/FBO.h>
#include <GeKo_Graphics/Scenegraph/Node.h>
#include <GeKo_Graphics/Geometry/Rect.h>
#include <GeKo_Graphics/Camera/Pilotview.h>
#include <glm/glm.hpp>


struct RenderObject{

};
struct RenderContext{
  virtual void bindContext() const = 0;
};
struct OpenGL3Context : RenderContext {
  /*
  by initializing, the renderer check, if the OS support OpenGL
  */
  virtual void bindContext() const override;
};

class GUI;
class FBO;
class Scene;
class Shader;
class Window;
class Node;

class Renderer{
public:
  Renderer(const RenderContext &context);
  ~Renderer();

  template<typename T>
  void draw(const Buffer<T> &buffer) {
    buffer.bind();
    //FIXME - Needs generic interface
    glEnableVertexAttribArray(0);
    // MAGIC NUMBER - needs proper information for vertex size
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, buffer.size);
    glDisableVertexAttribArray(0);
    buffer.unbind();
  }
  /*
  print Information about the supported OpenGL & GLSL version, Vendor and Renderer
  */
  void printInfo();

  void init(int windowWidth, int windowHeight);
  
  void renderScene(Scene &scene, Window &window);
  void renderSkybox(Skybox &skybox);

  ///renders the given GUI
  void renderGUI(GUI &guiToRender, Window &window);

  void useReflections(bool useReflections, float *reflectionStrength = new float(0.2f));
  void useAntiAliasing(bool useAntiAliasing);
  void useDeferredShading(bool useDeferredShading, Node *lightRootNode = nullptr, glm::fvec3 *lightColor = new glm::fvec3(1.0,1.0,1.0));
  void useBloom(bool useBloom, float *bloomStrength = new float(0.20f));
  void useBlur(bool useBlur, float *blurStrength = new float(1.0f));
  void useRadialBlur(bool useRadialBlur, float *radialBlurStrength = new float(1.0f));
  void useDoF(bool useDoF, float *focusDepth = new float(0.04f));
  void useSSAO(bool useSSAO, float *quality = new float(30.0f), float *radius = new float(0.1f));
  void useShadowMapping(bool useShadowMapping, int *usePCF, ConeLight *coneLight = nullptr);

  void addGui(GUI *guiToAdd);

private:
  void bindFBO();
  void unbindFBO();
	FBO* getLastFBO();

  void renderReflections(float camNear, float camFar);
  void renderAntiAliasing();
  void renderDeferredShading();
  void renderBloom();
  void renderBlur();
  void renderRadialBlur();
  void renderDoF(Scene& scene);
  void renderSSAO();
  void renderShadowMapping(Scene& scene);
  
  std::vector<GUI*> m_guis;

  bool m_useReflections;
  bool m_useAntiAliasing;
  bool m_useBloom;
  bool m_useBlur;
  bool m_useRadialBlur;
  bool m_useDoF;
  bool m_useDeferredShading;
  bool m_useSSAO;
  bool m_useShadowMapping;
  
  Node *m_dsLightRootNode;
  glm::fvec3 *m_dsLightColor;

  float *m_ssaoQuality;
  float *m_ssaoRadius;

  float *m_screenSpaceReflectionsStrength;
  float *m_blurStrength;
  float *m_radialBlurStrength;
  float *m_focusDepth;
  float *m_depthBias;
  float *m_bloomStrength;
  int *m_pcf;

  ConeLight *m_smConeLight;
  Pilotview *m_smCam;

	bool m_currentFBOIndex;
  bool m_firstRender;

  FBO *m_gBuffer;
  FBO *m_smFBO;
	FBO *m_ping;
	FBO *m_pong;

	glm::mat4 m_currentProjectionMatrix;
	glm::mat4 m_currentViewMatrix;

  int m_windowWidth;
  int m_windowHeight;
	
  ShaderProgram *m_shaderGBuffer;
  ShaderProgram *m_shaderRLR;
  ShaderProgram *m_shaderSFQ;
  ShaderProgram *m_shaderFXAA;
  ShaderProgram *m_shaderDSLighting;
  ShaderProgram *m_shaderDSCompositing;
  ShaderProgram *m_shaderBloom;
  ShaderProgram *m_shaderBlur;
  ShaderProgram *m_shaderRadialBlur;
  ShaderProgram *m_shaderDoF;
  ShaderProgram *m_shaderDepth;
  ShaderProgram *m_shaderSSAOcalc;
  ShaderProgram *m_shaderSSAOblur;
  ShaderProgram *m_shaderSSAOfinal;
  ShaderProgram *m_shaderShadowMapping;
  
  Rect m_sfq;
};