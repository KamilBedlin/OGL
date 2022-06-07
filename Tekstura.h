#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Tekstura
{
public:
	GLuint ID;
	const char* typ;
	GLuint jednostka;

	Tekstura(const char* obraz, const char* typTekstury, GLuint miejsce);

	// Przypisuje jednostke tekstury do tekstury
	void jednTekstury(Shader& shader, const char* uniform, GLuint jednostka);
	// Wi��e tekstur�
	void Wi��();
	// Odwi�zuje tekstur�
	void Rozwi��();
	// Usuwa tekstur�
	void Usu�();
};
#endif