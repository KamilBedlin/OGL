#include"VAO.h"

// Generuje VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Linkuje atrybut VBO do VAO
void VAO::Pod³¹czAtrybut(VBO& VBO, GLuint layout, GLuint liczbaSk³adowych, GLenum typ, GLsizeiptr krok, void* offset)
{
	VBO.Wi¹¿();
	glVertexAttribPointer(layout, liczbaSk³adowych, typ, GL_FALSE, krok, offset);
	glEnableVertexAttribArray(layout);
	VBO.Rozwi¹¿();
}

// Wi¹¿e VAO z bie¿¹cym buforem
void VAO::Wi¹¿()
{
	glBindVertexArray(ID);
}

// Odwi¹zuje VAO od bie¿¹cego bufora
void VAO::Rozwi¹¿()
{
	glBindVertexArray(0);
}

// Usuwa VAO
void VAO::Usuñ()
{
	glDeleteVertexArrays(1, &ID);
}