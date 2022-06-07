#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

// Struktura werteksu
struct Werteks
{
	glm::vec3 pozycja;
	glm::vec3 normalne;
	glm::vec3 kolor;
	glm::vec2 tekstury;
};



class VBO
{
public:
	// VBO ID
	GLuint ID;
	// Generuje VBO i podpina do niego werteksy
	VBO(std::vector<Werteks>& werteksy);

	// Wi¹¿e VBO z bie¿¹cym buforem
	void Wi¹¿();
	// Odwi¹zuje VBO od bie¿¹cego bufora
	void Rozwi¹¿();
	// Usuwa VBO
	void Usuñ();
};

#endif