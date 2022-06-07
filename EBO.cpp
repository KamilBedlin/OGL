#include"EBO.h"

// Generuje EBO i podpina do niego indeksy
EBO::EBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Wi¹¿e EBO z bie¿¹cym buforem
void EBO::Wi¹¿()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Odwi¹zuje EBO od bie¿¹cego bufora
void EBO::Rozwi¹¿()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Usuwa EBO
void EBO::Usuñ()
{
	glDeleteBuffers(1, &ID);
}