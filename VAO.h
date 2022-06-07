#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// VAO ID
	GLuint ID;
	// Generuje VAO ID
	VAO();

	// Linkuje atrybut VBO do VAO
	void Pod��czAtrybut(VBO& VBO, GLuint layout, GLuint numComponents, GLenum typ, GLsizeiptr krok, void* offset);
	// Wi��e VAO z bie��cym buforem
	void Wi��();
	// Odwi�zuje VAO od bie��cego bufora
	void Rozwi��();
	// Usuwa VAO
	void Usu�();
};

#endif