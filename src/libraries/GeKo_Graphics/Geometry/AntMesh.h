#pragma once

#include <GeKo_Graphics/Geometry/Mesh.h>


///This is the class for generating a Ant
/**Just use the standard constructor and a Ant-Object will be generated automatically. The .obj-data has to be in the
ressource folder!*/
class AntMesh : public Mesh {

public:
		AntMesh();
		~AntMesh();
};