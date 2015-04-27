#include "Geko_Resource/Handle/Handle.hpp"
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
using namespace Handle;


struct ResourceManager{
  HandleManager<StaticMesh> geometries;
};
StaticMesh loadStaticMesh(std::string filepath){
  Assimp::Importer importer;
  std::vector<aiMesh*> meshEntries;
  const aiScene *scene = importer.ReadFile(filepath.c_str(),
                                           aiProcess_Triangulate |
                                           aiProcess_SplitLargeMeshes |
                                           aiProcess_ImproveCacheLocality);
  std::vector<glm::vec4> vertices;
  std::vector<Index> indices;
  std::vector<Uv> uvs;
  std::vector<Normal> normals;
  if (!scene) {
    printf("Unable to load mesh: %s\n", importer.GetErrorString());
  }
  //
  for (int i = 0; i < scene->mNumMeshes; i++){
    meshEntries.push_back(scene->mMeshes[i]);
  }
  for(auto *meshEntry : meshEntries){
    if(meshEntry->HasPositions() &&
       meshEntry->HasTextureCoords(0) &&
       meshEntry->HasNormals() &&
       meshEntry->HasFaces()){
      for(size_t i = 0; i < meshEntry->mNumVertices; ++i){
        auto vertex = meshEntry->mVertices[i];
        vertices.push_back(glm::vec4(vertex.x,vertex.y,vertex.z,1.0f));

        auto uv= meshEntry->mTextureCoords[0][i];
        uvs.push_back(glm::vec2(uv.x,uv.y));

        auto normal = meshEntry->mNormals[i];
        normals.push_back(glm::vec3(normal.x,normal.y,normal.z));
      }

      for (size_t k = 0; k < meshEntry->mNumFaces; k++){
        for (size_t l = 0; l < meshEntry->mFaces->mNumIndices; l++)
          indices.push_back(meshEntry->mFaces[k].mIndices[l]);
      }
    }
  }
  return StaticMesh{vertices,normals,uvs,indices};
}
int main(){
  auto r = ResourceManager();
  auto sponzaHandle = r.geometries.add(loadStaticMesh("/home/maik/Downloads/sponza_obj/sponza.obj"));
  Geometry sponza = sponzaHandle.get().toGeometry();
}
