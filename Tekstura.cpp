#include"Tekstura.h"

Tekstura::Tekstura(const char* obraz, const char* typTekstury, GLuint miejsce)
{
	// Do³¹cza typ tekstury do obiektu tekstury
	typ = typTekstury;

	// Przechowuje szerokoœæ, wysokoœæ i liczbê kana³ów kolorów
	int szerokoœæObrazka, wysokoœæObrazka, liczbaKana³ów;
	// Obraca obraz, ze wzglêdu na ró¿nice w czytaniu obrazów przez stb i openGL
	stbi_set_flip_vertically_on_load(true);
	// Czyta obraz z pliku i go przechowuje w zmiennej
	unsigned char* bajty = stbi_load(obraz, &szerokoœæObrazka, &wysokoœæObrazka, &liczbaKana³ów, 0);

	// Generuje obiekt tekstury OpenGL
	glGenTextures(1, &ID);
	// Przypisuje teksturê do jednostki tekstury
	glActiveTexture(GL_TEXTURE0 + miejsce);
	jednostka = miejsce;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Konfiguruje algorytm do powiêkszania i zmniejszania obrazu
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Konfiguruje sposób w jaki tekstura siê powtarza (lub nie)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Przypisuje obraz do obiektu tekstury OpenGL
	if (liczbaKana³ów == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			szerokoœæObrazka,
			wysokoœæObrazka,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bajty
		);
	else if (liczbaKana³ów == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			szerokoœæObrazka,
			wysokoœæObrazka,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bajty
		);
	else if (liczbaKana³ów == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			szerokoœæObrazka,
			wysokoœæObrazka,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bajty
		);
	else
		throw std::invalid_argument("Automatyczne wykrywanie typu tesktury siê nie powiod³o");

	// Generuje Mipmapy
	glGenerateMipmap(GL_TEXTURE_2D);

	// Usuwa dane obrazu, bo znajduj¹ siê ju¿ w obiekcie tekstury OpenGL
	stbi_image_free(bajty);

	// Odwi¹zuje obiekt tekstury OpenGL, ¿eby go przypadkiem nie modyfikowaæ
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tekstura::jednTekstury(Shader& shader, const char* uniform, GLuint unit)
{
	// Wyci¹ga lokacje uniformu
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Trzeba aktywowaæ shader zanim bêdzie mo¿na modyfikowaæ uniform
	shader.Aktywuj();
	// Usstawia wartoœæ uniformu
	glUniform1i(texUni, unit);
}

void Tekstura::Wi¹¿()
{
	glActiveTexture(GL_TEXTURE0 + jednostka);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Tekstura::Rozwi¹¿()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tekstura::Usuñ()
{
	glDeleteTextures(1, &ID);
}