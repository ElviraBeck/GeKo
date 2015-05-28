#include <iostream>
#include <GL/glew.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <vector>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "GeKo_Graphics/Geometry/Geometry.h"
int main(){
  auto r = ResourceManager();
  auto sponzaHandle = r.loadStaticMesh(RESOURCES_PATH "/sponza.obj");
  Geometry sponza = sponzaHandle.get().toGeometry();
  std::cout << sponza.getNormals().size() << std::endl;
  std::cout << sponza.getVertices().size() << std::endl;
  std::cout << sponza.getUV().size() << std::endl;
  std::cout << sponza.getIndexList().size() << std::endl;
}
