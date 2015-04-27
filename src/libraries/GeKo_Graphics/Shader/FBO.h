#pragma once
#include <GeKo_Graphics/Defs.h>
#include <GeKo_Graphics/include.h>

/*
* @brief Class for OpenGL frame buffer objects (FBOs).
*
* A FBO has a given width and height. All textures used by it
* or created by it have these values for width and height. It
* is possible to create a fbo with multiple color buffers, each
* with rgba values per pixel. A fbo can use a depth buffer and
* a stencil buffer, which are not necesserily needed.
*
* To create a fbo, the constructor has to be called with the
* preferred width and height. Additionally the number of color
* textures and the use of depth or stencil buffer can be assigned.
* An example creation of a fbo with 1 color buffer and a depth
* buffer is:
* CVK::FBO fbo( 400, 300, 1, true, true);
*
* To use a fbo, it has to be bound to OpenGL. If the standard
* OpenGL frame buffer should be used, the fbo has to be unbound.
* fbo.bind();
* ...
* fbo.unbind();
*
* To use the result of the fbo, getters for the colorTextures
* and the depth texture exist.
* GLuint colorTexture = fbo.getColorTexture(0);
* GLuint depthTexture = fbo.getDepthTexture();
*/
class FBO
{
public:
	/*
	Constructor - create a certain number of color textures and depth and stencil texture if wanted.
	you can get the textures this e.g. with getColorTexture(0), getDepthTexture() & getSzencilTexture()??
	*/
	FBO(int width, int height, int numColorTextures = 1, bool depthTexture = false, bool stencilTexture = false);
	/*
	Destructor - if object getting destroyed, the texture should be removed
	*/
	~FBO();
	/*
	if you want to use this fbo instead the standard fbo from OpenGL
	*/
	void bind();
	/*
	if you want to use the standard fbo from OpenGL instead this fbo
	*/
	void unbind();
	/*
	generate a depth and a stencil texture and certain number of color textures.
	this function calls generateColorTexture(), generateDepthTexture() & generateStencilTexture()
	*/
	void generateTextures(int width, int height, unsigned int numColorTextures, bool depthTexture, bool stencilTexture);
	/*
	generate a color texture with a certain index (index ist dependet with how much color textures you want)
	this function calls createTexture()
	*/
	void generateColorTexture(std::vector<GLenum> &drawBuffers, unsigned int index);
	/*
	generate a depth texture - m_depthTexture
	this function calls createTexture()
	*/
	void generateDepthTexture();
	/*
	generate a stencil texture - m_stencilTexture
	this function calls createTexture()
	*/
	void generateStencilTexture();
	/*
	generate a texture
	*/
	GLuint createTexture();
	/*
	generate the textures with a new size
	*/
	void resize(int width, int height);
	/*
	delete color, depth and stencil textures
	*/
	void reset();
	/*
	give the color texture, if index is allowed
	*/
	GLuint getColorTexture(unsigned int index);
	/*
	give the depth texture, if one exist
	*/
	GLuint getDepthTexture();
	/*
	give the stencil texture, if one exist
	*/
	GLuint getStencilTexture();

private:
	int m_width, m_height;
	std::vector<GLuint> m_colorTextures;
	GLuint m_depthTexture, m_stencilTexture;
	GLuint m_frameBufferHandle;
};