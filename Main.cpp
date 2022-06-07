//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Aktor.h"
#include"Model.h"


const unsigned int szerokoœæOkna = 800;
const unsigned int wysokoœæOkna = 800;


int main()
{
	#pragma region InicjacjaOkna
	// Inicjuje glfw
	glfwInit();

	// Mówi glfw, ¿e u¿ywamy wersji OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Mówi glfw, ¿e u¿ywamy profilu rdzennego
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Stwórz okno o nazwie OGL
	GLFWwindow* okno = glfwCreateWindow(szerokoœæOkna, wysokoœæOkna, "OGL", NULL, NULL);
	// SprawdŸ, czy okno zosta³o poprawnie utworzone
	if (okno == NULL)
	{
		std::cout << "Nie uda³o siê stworzyæ okna GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Daj oknu obecny kontekst
	glfwMakeContextCurrent(okno);

	// £aduje glad do konfiguracji OpenGL
	gladLoadGL();
	// Stwórz viewport OpenGL w oknie
	glViewport(0, 0, szerokoœæOkna, wysokoœæOkna);
	#pragma endregion

	// Œcie¿ka do folderu z teksturami
	std::string œcie¿kaTekstur = "Resources/Tekstures/";

	// Œcie¿ka do folderu z modelami
	std::string œcie¿kaModeli = "Resources/Models/";

	// Generuje program shaderów z shader.wert i shader.frag 
	Shader shaderProgram("shader.wert", "shader.frag");

	glm::vec4 kolorŒwiat³a = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 pozycjaŒwiat³a = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 modelŒwiat³a = glm::mat4(1.0f);
	modelŒwiat³a = glm::translate(modelŒwiat³a, pozycjaŒwiat³a);

	shaderProgram.Aktywuj();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), kolorŒwiat³a.x, kolorŒwiat³a.y, kolorŒwiat³a.z, kolorŒwiat³a.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), pozycjaŒwiat³a.x, pozycjaŒwiat³a.y, pozycjaŒwiat³a.z);

	// W³¹cza bufor g³êbokoœci
	glEnable(GL_DEPTH_TEST);

	// Tworzy kamerê
	Kamera kamera(szerokoœæOkna, wysokoœæOkna, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model((œcie¿kaModeli + "sword/scene.gltf").c_str());


	// G³ówna pêtla
	while (!glfwWindowShouldClose(okno))
	{
		// Kolor t³a
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Wyczyœæ bufor tylni i bufor g³êbokoœci
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Obs³uga sterowania
		kamera.Sterowanie(okno);
		// Aktualizuje i eksportuje macierz kamery do shadera werteksów
		kamera.aktualMacierz(45.0f, 0.1f, 100.0f);


		// Rysuje aktorów
		// Draw a model
		model.Rysuj(shaderProgram, kamera);


		// Zamieñ przedni bufor z tylnim
		glfwSwapBuffers(okno);
		// Zajmij siê zdarzeniami glfw
		glfwPollEvents();
	}



	// Usuñ wszystkie stworzone obiekty
	shaderProgram.Usuñ();
	// Usuñ okno przed zakoñczeniem programu
	glfwDestroyWindow(okno);
	// Zakoñcz glfw przed zakoñczeniem programu
	glfwTerminate();
	return 0;
}