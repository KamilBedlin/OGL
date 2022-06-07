#include"Tekstura.h"

Tekstura::Tekstura(const char* obraz, const char* typTekstury, GLuint miejsce)
{
	// Do��cza typ tekstury do obiektu tekstury
	typ = typTekstury;

	// Przechowuje szeroko��, wysoko�� i liczb� kana��w kolor�w
	int szeroko��Obrazka, wysoko��Obrazka, liczbaKana��w;
	// Obraca obraz, ze wzgl�du na r�nice w czytaniu obraz�w przez stb i openGL
	stbi_set_flip_vertically_on_load(true);
	// Czyta obraz z pliku i go przechowuje w zmiennej
	unsigned char* bajty = stbi_load(obraz, &szeroko��Obrazka, &wysoko��Obrazka, &liczbaKana��w, 0);

	// Generuje obiekt tekstury OpenGL
	glGenTextures(1, &ID);
	// Przypisuje tekstur� do jednostki tekstury
	glActiveTexture(GL_TEXTURE0 + miejsce);
	jednostka = miejsce;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Konfiguruje algorytm do powi�kszania i zmniejszania obrazu
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Konfiguruje spos�b w jaki tekstura si� powtarza (lub nie)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Przypisuje obraz do obiektu tekstury OpenGL
	if (liczbaKana��w == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			szeroko��Obrazka,
			wysoko��Obrazka,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bajty
		);
	else if (liczbaKana��w == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			szeroko��Obrazka,
			wysoko��Obrazka,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bajty
		);
	else if (liczbaKana��w == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			szeroko��Obrazka,
			wysoko��Obrazka,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bajty
		);
	else
		throw std::invalid_argument("Automatyczne wykrywanie typu tesktury si� nie powiod�o");

	// Generuje Mipmapy
	glGenerateMipmap(GL_TEXTURE_2D);

	// Usuwa dane obrazu, bo znajduj� si� ju� w obiekcie tekstury OpenGL
	stbi_image_free(bajty);

	// Odwi�zuje obiekt tekstury OpenGL, �eby go przypadkiem nie modyfikowa�
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tekstura::jednTekstury(Shader& shader, const char* uniform, GLuint unit)
{
	// Wyci�ga lokacje uniformu
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Trzeba aktywowa� shader zanim b�dzie mo�na modyfikowa� uniform
	shader.Aktywuj();
	// Usstawia warto�� uniformu
	glUniform1i(texUni, unit);
}

void Tekstura::Wi��()
{
	glActiveTexture(GL_TEXTURE0 + jednostka);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Tekstura::Rozwi��()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tekstura::Usu�()
{
	glDeleteTextures(1, &ID);
}