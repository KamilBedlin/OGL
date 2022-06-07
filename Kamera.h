#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Kamera
{
public:
	// G³ówne wektory kamery
	glm::vec3 Pozycja;
	glm::vec3 Orientacja = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Góra = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 macierzKamery = glm::mat4(1.0f);

	// Kamera nie skacze
	bool pierwszeKlikniêcie = true;

	// Szerokoœæ i wysokoœæ okna
	int szerokoœæ;
	int wysokoœæ;

	// Szybkoœæ kamery i jej czu³oœæ
	float szybkoœæ = 0.001f;
	float czu³oœæ = 100.0f;

	// Konstruktor kamery
	Kamera(int szerokoœæ, int wysokoœæ, glm::vec3 pozycja);

	// Aktualizacja macierzy kamery do shaderu werteksów
	void aktualMacierz(float k¹tPolaWidzenia, float bli¿, float dal);
	// Eksportuje macierz kamery do shadera
	void Macierz(Shader& shader, const char* uniform);
	// Obs³uga sterowania kamer¹
	void Sterowanie(GLFWwindow* okno);
};
#endif