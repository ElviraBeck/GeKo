#pragma once
#include <GeKo_Graphics/Geometry/Geometry.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

///This is the class for importing static meshes via Assimp.
/**As imported meshes can consist of more than one model, this class has a vector named MeshEntry, where all the separate
 * meshes can be found.
 * To use the class you have to generate a new mesh via Mesh *name = new Mesh("dataname.format");
 **/
class Mesh : public Geometry {

public:
		///Loads the file with the "filename" as a Mesh-Object
		/***/
		Mesh(const char *filename);

		~Mesh();

		///This method loads all the data of the meshes into the Geometry class
		/**/
		void initializeData();

protected:
	std::vector<aiMesh*> meshEntries;
};