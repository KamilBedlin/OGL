#include"Kamera.h"



Kamera::Kamera(int szerokoœæ, int wysokoœæ, glm::vec3 pozycja)
{
	Kamera::szerokoœæ = szerokoœæ;
	Kamera::wysokoœæ = wysokoœæ;
	Pozycja = pozycja;
}

void Kamera::aktualMacierz(float k¹tPolaWidzenia, float bli¿, float dal)
{
	// Inicjalizazja macierzy 1
	glm::mat4 widok = glm::mat4(1.0f);
	glm::mat4 rzutowanie = glm::mat4(1.0f);

	// Kamera patrzy w odpowiednie miejsce z odpowiedniego miejsca
	widok = glm::lookAt(Pozycja, Pozycja + Orientacja, Góra);
	// Dodaje perspektywe
	rzutowanie = glm::perspective(glm::radians(k¹tPolaWidzenia), (float)szerokoœæ / wysokoœæ, bli¿, dal);

	// Sets new camera matrix
	macierzKamery = rzutowanie * widok;
}

void Kamera::Macierz(Shader& shader, const char* uniform)
{
	// Eksportuje macierz kamery
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(macierzKamery));
}



void Kamera::Sterowanie(GLFWwindow* okno)
{
	// Obs³uga sterowania klawiatury
	if (glfwGetKey(okno, GLFW_KEY_W) == GLFW_PRESS)
	{
		Pozycja += szybkoœæ * Orientacja;
	}
	if (glfwGetKey(okno, GLFW_KEY_A) == GLFW_PRESS)
	{
		Pozycja += szybkoœæ * -glm::normalize(glm::cross(Orientacja, Góra));
	}
	if (glfwGetKey(okno, GLFW_KEY_S) == GLFW_PRESS)
	{
		Pozycja += szybkoœæ * -Orientacja;
	}
	if (glfwGetKey(okno, GLFW_KEY_D) == GLFW_PRESS)
	{
		Pozycja += szybkoœæ * glm::normalize(glm::cross(Orientacja, Góra));
	}
	if (glfwGetKey(okno, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Pozycja += szybkoœæ * Góra;
	}
	if (glfwGetKey(okno, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Pozycja += szybkoœæ * -Góra;
	}
	if (glfwGetKey(okno, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		szybkoœæ = 0.004f;
	}
	else if (glfwGetKey(okno, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		szybkoœæ = 0.001f;
	}


	// Obs³uga sterowania myszki
	if (glfwGetMouseButton(okno, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Chowa kursor myszy przy przesuwaniu
		glfwSetInputMode(okno, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Kamera nie skacze
		if (pierwszeKlikniêcie)
		{
			glfwSetCursorPos(okno, (szerokoœæ / 2), (wysokoœæ / 2));
			pierwszeKlikniêcie = false;
		}

		// Pozycja myszki
		double myszX;
		double myszY;
		// Pobiera pozycjê myszki z okna
		glfwGetCursorPos(okno, &myszX, &myszY);

		// Normalizuje i zmienia pozycjê kursora na œrodek okna, a potem j¹ transformuje w stopnie
		float rotX = czu³oœæ * (float)(myszY - (wysokoœæ / 2)) / wysokoœæ;
		float rotY = czu³oœæ * (float)(myszX - (szerokoœæ / 2)) / szerokoœæ;

		// Kalkuluje obrót w pionie dla Orientacji
		glm::vec3 newOrientation = glm::rotate(Orientacja, glm::radians(-rotX), glm::normalize(glm::cross(Orientacja, Góra)));

		// Sprawdza czy nowa Orientacja jest mo¿liwa, nie chcemy kamery do góry nogami
		if (abs(glm::angle(newOrientation, Góra) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientacja = newOrientation;
		}

		// Obraca Orientacjê w poziomie
		Orientacja = glm::rotate(Orientacja, glm::radians(-rotY), Góra);

		// Ustawia pozycjê kursora w œrodku okna
		glfwSetCursorPos(okno, (szerokoœæ / 2), (wysokoœæ / 2));
	}
	else if (glfwGetMouseButton(okno, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Przestaje ukrywaæ kursor
		glfwSetInputMode(okno, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Kamera nie skacze
		pierwszeKlikniêcie = true;
	}
}