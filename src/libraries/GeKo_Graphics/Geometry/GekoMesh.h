#pragma once

#include <GeKo_Graphics/Geometry/Mesh.h>

///This is the class for generating a Geko
/**Just use the standard constructor and a Geko-Object will be generated automatically. The .obj-data has to be in the
ressource folder!*/
class GekoMesh : public Mesh {

public:

	GekoMesh();

	~GekoMesh();
};