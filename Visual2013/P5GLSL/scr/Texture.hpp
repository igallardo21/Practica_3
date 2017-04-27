#ifndef __TEXTURE__
#define __TEXTURE__

#include "auxiliar.hpp"

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>

class Texture
{
private:
	unsigned int textureId;
	unsigned int type;
	
public:
	Texture()
	{
		
	}

	~Texture()
	{
		glDeleteTextures(1, &textureId);
	}

	//Añadir nuevas texturas
	void Texture::addColorTexture(const char *pTexture){
		textureId = loadTex(pTexture);
		type = 0;
	}

	void Texture::addEmiTexture(const char *pTexture){
		textureId = loadTex(pTexture);
		type = 1;
	}

	void Texture::addNormalTexture(const char *pTexture){
		textureId = loadTex(pTexture);
		type = 2;
	}

	void Texture::addSpecTexture(const char *pTexture){
		textureId = loadTex(pTexture);
		type = 3;
	}
	
	int Texture::getTextureType(){
		return type;
	}

	
	//Cargar fichero de textura
	unsigned int Texture::loadTex(const char *fileName)
	{
		unsigned char *map;
		unsigned int w, h;
		map = loadTexture(fileName, w, h);
		if (!map)
		{
			std::cout << "Error cargando el fichero: "
				<< fileName << std::endl;
			exit(-1);
		}

		unsigned int texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);

		delete[] map;

		glGenerateMipmap(GL_TEXTURE_2D);

		//Anisotrópico
		if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		{
			GLfloat fLargest;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		}

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		return texId;
	}

	//Renderizar distintas texturas
	void Texture::renderColorTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	void Texture::renderEmiTexture()
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	void Texture::renderNormalTexture()
	{
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	void Texture::renderSpecTexture()
	{
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

}; //class Texture

#endif