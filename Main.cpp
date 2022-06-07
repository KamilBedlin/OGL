//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Aktor.h"
#include"Model.h"


const unsigned int szeroko��Okna = 800;
const unsigned int wysoko��Okna = 800;


int main()
{
	#pragma region InicjacjaOkna
	// Inicjuje glfw
	glfwInit();

	// M�wi glfw, �e u�ywamy wersji OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// M�wi glfw, �e u�ywamy profilu rdzennego
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Stw�rz okno o nazwie OGL
	GLFWwindow* okno = glfwCreateWindow(szeroko��Okna, wysoko��Okna, "OGL", NULL, NULL);
	// Sprawd�, czy okno zosta�o poprawnie utworzone
	if (okno == NULL)
	{
		std::cout << "Nie uda�o si� stworzy� okna GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Daj oknu obecny kontekst
	glfwMakeContextCurrent(okno);

	// �aduje glad do konfiguracji OpenGL
	gladLoadGL();
	// Stw�rz viewport OpenGL w oknie
	glViewport(0, 0, szeroko��Okna, wysoko��Okna);
	#pragma endregion

	// �cie�ka do folderu z teksturami
	std::string �cie�kaTekstur = "Resources/Tekstures/";

	// �cie�ka do folderu z modelami
	std::string �cie�kaModeli = "Resources/Models/";

	// Generuje program shader�w z shader.wert i shader.frag 
	Shader shaderProgram("shader.wert", "shader.frag");

	glm::vec4 kolor�wiat�a = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 pozycja�wiat�a = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 model�wiat�a = glm::mat4(1.0f);
	model�wiat�a = glm::translate(model�wiat�a, pozycja�wiat�a);

	shaderProgram.Aktywuj();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), kolor�wiat�a.x, kolor�wiat�a.y, kolor�wiat�a.z, kolor�wiat�a.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), pozycja�wiat�a.x, pozycja�wiat�a.y, pozycja�wiat�a.z);

	// W��cza bufor g��boko�ci
	glEnable(GL_DEPTH_TEST);

	// Tworzy kamer�
	Kamera kamera(szeroko��Okna, wysoko��Okna, glm::vec3(0.0f, 0.0f, 2.0f));

	Model model((�cie�kaModeli + "sword/scene.gltf").c_str());


	// G��wna p�tla
	while (!glfwWindowShouldClose(okno))
	{
		// Kolor t�a
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Wyczy�� bufor tylni i bufor g��boko�ci
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Obs�uga sterowania
		kamera.Sterowanie(okno);
		// Aktualizuje i eksportuje macierz kamery do shadera werteks�w
		kamera.aktualMacierz(45.0f, 0.1f, 100.0f);


		// Rysuje aktor�w
		// Draw a model
		model.Rysuj(shaderProgram, kamera);


		// Zamie� przedni bufor z tylnim
		glfwSwapBuffers(okno);
		// Zajmij si� zdarzeniami glfw
		glfwPollEvents();
	}



	// Usu� wszystkie stworzone obiekty
	shaderProgram.Usu�();
	// Usu� okno przed zako�czeniem programu
	glfwDestroyWindow(okno);
	// Zako�cz glfw przed zako�czeniem programu
	glfwTerminate();
	return 0;
}