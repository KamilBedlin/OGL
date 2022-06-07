#include "Aktor.h"

Aktor::Aktor(std::vector <Werteks>& werteksy, std::vector <GLuint>& indeksy, std::vector <Tekstura>& tekstury)
{
	Aktor::werteksy = werteksy;
	Aktor::indeksy = indeksy;
	Aktor::tekstury = tekstury;

	VAO.Wi¹¿();
	// Generuje VBO i pod³¹cza do niego werteksy
	VBO VBO(werteksy);
	// Generuje EBO i pod³¹cza do niego indeksy
	EBO EBO(indeksy);
	// Pod³¹cza atrybuty VBO do VAO
	VAO.Pod³¹czAtrybut(VBO, 0, 3, GL_FLOAT, sizeof(Werteks), (void*)0);
	VAO.Pod³¹czAtrybut(VBO, 1, 3, GL_FLOAT, sizeof(Werteks), (void*)(3 * sizeof(float)));
	VAO.Pod³¹czAtrybut(VBO, 2, 3, GL_FLOAT, sizeof(Werteks), (void*)(6 * sizeof(float)));
	VAO.Pod³¹czAtrybut(VBO, 3, 2, GL_FLOAT, sizeof(Werteks), (void*)(9 * sizeof(float)));
	// Odwi¹zuje wszystkie, ¿eby ich przypadkiem nie zmodyfikowaæ
	VAO.Rozwi¹¿();
	VBO.Rozwi¹¿();
	EBO.Rozwi¹¿();
}


void Aktor::Rysuj(
	Shader& shader, 
	Kamera& kamera,
	glm::mat4 macierz,
	glm::vec3 pozycja,
	glm::quat rotacja,
	glm::vec3 skala
)
{
	// Pod³¹cza shader, ¿eby mieæ dostêp do uniformów
	shader.Aktywuj();
	VAO.Wi¹¿();

	// Trzyma informacje ile ró¿nych typów tekstur mamy
	unsigned int liczDiffuse = 0;
	unsigned int liczSpecular = 0;

	for (unsigned int i = 0; i < tekstury.size(); i++)
	{
		std::string liczba;
		std::string typ = tekstury[i].typ;
		if (typ == "diffuse")
		{
			liczba = std::to_string(liczDiffuse++);
		}
		else if (typ == "specular")
		{
			liczba = std::to_string(liczSpecular++);
		}
		tekstury[i].jednTekstury(shader, (typ + liczba).c_str(), i);
		tekstury[i].Wi¹¿();
	}
	// Zajmuje siê macierz¹ kamery
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), kamera.Pozycja.x, kamera.Pozycja.y, kamera.Pozycja.z);
	kamera.Macierz(shader, "camMatrix");

	// Initialize matrices
	glm::mat4 poz = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 ska = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	poz = glm::translate(poz, pozycja);
	rot = glm::mat4_cast(rotacja);
	ska = glm::scale(ska, skala);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(poz));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(ska));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(macierz));

	// Rysuje aktora
	glDrawElements(GL_TRIANGLES, indeksy.size(), GL_UNSIGNED_INT, 0);
}