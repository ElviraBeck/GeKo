#pragma once

#include <GeKo_Graphics/Defs.h>

///This Class is to create Texture Objects by loading an image from the Ressources Folder

/*
To create a Texture use Texture(char* fileName). The file must be in the Ressources Folder.
*/

class Texture
{
public:
	Texture(char* fileName);	///The file must be in the Ressources folder
	Texture(GLuint texture);
	~Texture();

	bool load(char* fileName);
	void bind();
	void use(GLenum texturePosition);
	void unUse();

	void setTexture(GLuint texture);
	unsigned int getTexture();
	char* getFilepath();

private:
	unsigned int m_textureID;
	int m_width, m_heigth;
	char* filepath;

protected:

};