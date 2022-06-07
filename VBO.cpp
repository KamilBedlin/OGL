#include"VBO.h"

// Generuje VBO i podpina do niego werteksy
VBO::VBO(std::vector<Werteks>& werteksy)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, werteksy.size() * sizeof(Werteks), werteksy.data(), GL_STATIC_DRAW);
}

// Wi¹¿e VBO z bie¿¹cym buforem
void VBO::Wi¹¿()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Odwi¹zuje VBO od bie¿¹cego bufora
void VBO::Rozwi¹¿()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Usuwa VBO
void VBO::Usuñ()
{
	glDeleteBuffers(1, &ID);
}