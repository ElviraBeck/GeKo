#include <GeKo_Graphics/Geometry/Mesh.h>

Mesh::Mesh(const char *filename)
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_SplitLargeMeshes |
		aiProcess_ImproveCacheLocality);
	if (!scene) {
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	for (int i = 0; i < scene->mNumMeshes; i++){
		meshEntries.push_back(scene->mMeshes[i]);
	}
	initializeData();
}

Mesh::~Mesh()
{

}

void Mesh::initializeData()
{
	setIndexTrue();
	for (int i = 0; i < meshEntries.size(); i++)
	{

		for (int j = 0; j < meshEntries.at(i)->mNumVertices; j++)
		{
			m_points += meshEntries.at(i)->mNumVertices;
			m_indices += meshEntries.at(i)->mNumFaces;

			if (meshEntries.at(i)->HasPositions())
				m_vertices.push_back(glm::vec4(meshEntries.at(i)->mVertices[j].x, meshEntries.at(i)->mVertices[j].y, meshEntries.at(i)->mVertices[j].z, 1.0));

			if (meshEntries.at(i)->HasNormals()){
				//setNormalsTrue();
				//m_normals.push_back(glm::vec3(meshEntries.at(i)->mNormals[j].x, meshEntries.at(i)->mNormals[j].y, meshEntries.at(i)->mNormals[j].z));
			}
			/*if (meshEntries.at(i)->mTextureCoords != NULL){
			setUVTrue();
			m_uvs.push_back(glm::vec2(meshEntries.at(i)->mTextureCoords[j]->x, meshEntries.at(i)->mTextureCoords[j]->y));
			}*/

			if (meshEntries.at(i)->mTextureCoords[0] != NULL)
			{
				m_uvs.push_back(glm::vec2(meshEntries.at(i)->mTextureCoords[0]->x, meshEntries.at(i)->mTextureCoords[0]->z));
			}
		}

		for (int k = 0; k < meshEntries.at(i)->mNumFaces; k++){
			for (int l = 0; l < meshEntries.at(i)->mFaces->mNumIndices; l++)
				m_index.push_back(meshEntries.at(i)->mFaces[k].mIndices[l]);
		}
	}
}