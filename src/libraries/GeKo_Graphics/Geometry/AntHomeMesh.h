#pragma once

#include <GeKo_Graphics/Geometry/Mesh.h>

///This is the class for generating an Ant-Home
/**Just use the standard constructor and an Ant-Home-Object will be generated automatically. The .obj-data has to be in the
ressource folder!*/
class AntHomeMesh : public Mesh {

public:

	AntHomeMesh();

	~AntHomeMesh();
};