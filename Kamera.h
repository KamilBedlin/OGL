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
	// G��wne wektory kamery
	glm::vec3 Pozycja;
	glm::vec3 Orientacja = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 G�ra = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 macierzKamery = glm::mat4(1.0f);

	// Kamera nie skacze
	bool pierwszeKlikni�cie = true;

	// Szeroko�� i wysoko�� okna
	int szeroko��;
	int wysoko��;

	// Szybko�� kamery i jej czu�o��
	float szybko�� = 0.001f;
	float czu�o�� = 100.0f;

	// Konstruktor kamery
	Kamera(int szeroko��, int wysoko��, glm::vec3 pozycja);

	// Aktualizacja macierzy kamery do shaderu werteks�w
	void aktualMacierz(float k�tPolaWidzenia, float bli�, float dal);
	// Eksportuje macierz kamery do shadera
	void Macierz(Shader& shader, const char* uniform);
	// Obs�uga sterowania kamer�
	void Sterowanie(GLFWwindow* okno);
};
#endif