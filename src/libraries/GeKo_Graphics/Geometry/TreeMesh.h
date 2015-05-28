#pragma once

#include <GeKo_Graphics/Geometry/Mesh.h>

///This is the class for importing static meshes via Assimp.
/**Just use the standard constructor and a TreeMesh-Object will be generated automatically. The .obj-data has to be in the
ressource folder!*/

class TreeMesh : public Mesh {


public:

		TreeMesh();

		~TreeMesh();
};