#include"Kamera.h"



Kamera::Kamera(int szeroko��, int wysoko��, glm::vec3 pozycja)
{
	Kamera::szeroko�� = szeroko��;
	Kamera::wysoko�� = wysoko��;
	Pozycja = pozycja;
}

void Kamera::aktualMacierz(float k�tPolaWidzenia, float bli�, float dal)
{
	// Inicjalizazja macierzy 1
	glm::mat4 widok = glm::mat4(1.0f);
	glm::mat4 rzutowanie = glm::mat4(1.0f);

	// Kamera patrzy w odpowiednie miejsce z odpowiedniego miejsca
	widok = glm::lookAt(Pozycja, Pozycja + Orientacja, G�ra);
	// Dodaje perspektywe
	rzutowanie = glm::perspective(glm::radians(k�tPolaWidzenia), (float)szeroko�� / wysoko��, bli�, dal);

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
	// Obs�uga sterowania klawiatury
	if (glfwGetKey(okno, GLFW_KEY_W) == GLFW_PRESS)
	{
		Pozycja += szybko�� * Orientacja;
	}
	if (glfwGetKey(okno, GLFW_KEY_A) == GLFW_PRESS)
	{
		Pozycja += szybko�� * -glm::normalize(glm::cross(Orientacja, G�ra));
	}
	if (glfwGetKey(okno, GLFW_KEY_S) == GLFW_PRESS)
	{
		Pozycja += szybko�� * -Orientacja;
	}
	if (glfwGetKey(okno, GLFW_KEY_D) == GLFW_PRESS)
	{
		Pozycja += szybko�� * glm::normalize(glm::cross(Orientacja, G�ra));
	}
	if (glfwGetKey(okno, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Pozycja += szybko�� * G�ra;
	}
	if (glfwGetKey(okno, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Pozycja += szybko�� * -G�ra;
	}
	if (glfwGetKey(okno, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		szybko�� = 0.004f;
	}
	else if (glfwGetKey(okno, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		szybko�� = 0.001f;
	}


	// Obs�uga sterowania myszki
	if (glfwGetMouseButton(okno, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Chowa kursor myszy przy przesuwaniu
		glfwSetInputMode(okno, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Kamera nie skacze
		if (pierwszeKlikni�cie)
		{
			glfwSetCursorPos(okno, (szeroko�� / 2), (wysoko�� / 2));
			pierwszeKlikni�cie = false;
		}

		// Pozycja myszki
		double myszX;
		double myszY;
		// Pobiera pozycj� myszki z okna
		glfwGetCursorPos(okno, &myszX, &myszY);

		// Normalizuje i zmienia pozycj� kursora na �rodek okna, a potem j� transformuje w stopnie
		float rotX = czu�o�� * (float)(myszY - (wysoko�� / 2)) / wysoko��;
		float rotY = czu�o�� * (float)(myszX - (szeroko�� / 2)) / szeroko��;

		// Kalkuluje obr�t w pionie dla Orientacji
		glm::vec3 newOrientation = glm::rotate(Orientacja, glm::radians(-rotX), glm::normalize(glm::cross(Orientacja, G�ra)));

		// Sprawdza czy nowa Orientacja jest mo�liwa, nie chcemy kamery do g�ry nogami
		if (abs(glm::angle(newOrientation, G�ra) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientacja = newOrientation;
		}

		// Obraca Orientacj� w poziomie
		Orientacja = glm::rotate(Orientacja, glm::radians(-rotY), G�ra);

		// Ustawia pozycj� kursora w �rodku okna
		glfwSetCursorPos(okno, (szeroko�� / 2), (wysoko�� / 2));
	}
	else if (glfwGetMouseButton(okno, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Przestaje ukrywa� kursor
		glfwSetInputMode(okno, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Kamera nie skacze
		pierwszeKlikni�cie = true;
	}
}