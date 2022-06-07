#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Kamera.h"
#include"Tekstura.h"

class Aktor
{
public:
	std::vector <Werteks> werteksy;
	std::vector <GLuint> indeksy;
	std::vector <Tekstura> tekstury;
	// Przechowuje VAO, ¿eby mo¿na by³o go u¿yæ w funkcji Rysuj
	VAO VAO;

	// Inicjuje Aktora
	Aktor(std::vector <Werteks>& werteksy, std::vector <GLuint>& indeksy, std::vector <Tekstura>& tekstury);

	// Rysuje Aktora
	void Rysuj(
		Shader& shader, 
		Kamera& kamera,
		glm::mat4 macierz = glm::mat4(1.0f),
		glm::vec3 pozycja = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotacja = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 skala = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};
#endif