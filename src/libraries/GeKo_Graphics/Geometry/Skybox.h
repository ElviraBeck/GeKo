#pragma once
#include <GeKo_Graphics/Geometry/Geometry.h>

///A Skybox Object which will show a Texture of 6 piece-textures
class Skybox : public Geometry 
{
public:
	///Constructor which needs an Array with 6 Images
	Skybox(const char **fileNames);
	~Skybox();
	
	///Returns m_textureID
	unsigned int getSkyboxTexture();

protected:
	GLuint m_textureID;
	int m_width;
	int m_height;

private:
	///create a new texture with the 6 texture maps 
	bool load(const char **fileNames);
	///Initalize the different texture Units
	bool Init(const char **fileNames);
};