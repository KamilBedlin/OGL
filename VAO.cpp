#include"VAO.h"

// Generuje VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Linkuje atrybut VBO do VAO
void VAO::Pod��czAtrybut(VBO& VBO, GLuint layout, GLuint liczbaSk�adowych, GLenum typ, GLsizeiptr krok, void* offset)
{
	VBO.Wi��();
	glVertexAttribPointer(layout, liczbaSk�adowych, typ, GL_FALSE, krok, offset);
	glEnableVertexAttribArray(layout);
	VBO.Rozwi��();
}

// Wi��e VAO z bie��cym buforem
void VAO::Wi��()
{
	glBindVertexArray(ID);
}

// Odwi�zuje VAO od bie��cego bufora
void VAO::Rozwi��()
{
	glBindVertexArray(0);
}

// Usuwa VAO
void VAO::Usu�()
{
	glDeleteVertexArrays(1, &ID);
}