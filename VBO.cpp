#include"VBO.h"

// Generuje VBO i podpina do niego werteksy
VBO::VBO(std::vector<Werteks>& werteksy)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, werteksy.size() * sizeof(Werteks), werteksy.data(), GL_STATIC_DRAW);
}

// Wi��e VBO z bie��cym buforem
void VBO::Wi��()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Odwi�zuje VBO od bie��cego bufora
void VBO::Rozwi��()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Usuwa VBO
void VBO::Usu�()
{
	glDeleteBuffers(1, &ID);
}