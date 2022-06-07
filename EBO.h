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

	// Wi¹¿e EBO z bie¿¹cym buforem
	void Wi¹¿();
	// Odwi¹zuje EBO od bie¿¹cego bufora
	void Rozwi¹¿();
	// Usuwa EBO
	void Usuñ();
};

#endif