#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	// EBO ID
	GLuint ID;
	// Generuje EBO i podpina do niego indeksy
	EBO(std::vector<GLuint>& indeksy);

	// Wi��e EBO z bie��cym buforem
	void Wi��();
	// Odwi�zuje EBO od bie��cego bufora
	void Rozwi��();
	// Usuwa EBO
	void Usu�();
};

#endif