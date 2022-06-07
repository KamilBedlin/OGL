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
	void Pod³¹czAtrybut(VBO& VBO, GLuint layout, GLuint numComponents, GLenum typ, GLsizeiptr krok, void* offset);
	// Wi¹¿e VAO z bie¿¹cym buforem
	void Wi¹¿();
	// Odwi¹zuje VAO od bie¿¹cego bufora
	void Rozwi¹¿();
	// Usuwa VAO
	void Usuñ();
};

#endif