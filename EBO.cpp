#include"EBO.h"

// Generuje EBO i podpina do niego indeksy
EBO::EBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Wi��e EBO z bie��cym buforem
void EBO::Wi��()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Odwi�zuje EBO od bie��cego bufora
void EBO::Rozwi��()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Usuwa EBO
void EBO::Usu�()
{
	glDeleteBuffers(1, &ID);
}