#include"shaderClass.h"

// Czyta plik i zwraca jego zawartoœæ jako stringa
std::string get_file_contents(const char* nazwaPliku)
{
	std::ifstream plik(nazwaPliku, std::ios::binary);
	if (plik)
	{
		std::string zawartoœæ;
		plik.seekg(0, std::ios::end);
		zawartoœæ.resize(plik.tellg());
		plik.seekg(0, std::ios::beg);
		plik.read(&zawartoœæ[0], zawartoœæ.size());
		plik.close();
		return(zawartoœæ);
	}
	throw(errno);
}

// Buduje program shaderów z shadera werteksów i shadera fragmentów
Shader::Shader(const char* plikShaderaWerteksów, const char* plikShaderaFragmentów)
{
	// Czyta pliki shaderów to stringów
	std::string kodShaderaWerteksów = get_file_contents(plikShaderaWerteksów);
	std::string kodShaderaFragmentów = get_file_contents(plikShaderaFragmentów);

	// Konwertuje stringi w tablice char
	const char* kodWerteks = kodShaderaWerteksów.c_str();
	const char* kodFragment = kodShaderaFragmentów.c_str();

	// Tworzy obiekt shadera werteksów i referencjê do niego
	GLuint shaderWerteksów = glCreateShader(GL_VERTEX_SHADER);
	// Do³¹cza kod shadera werteksów do obiektu
	glShaderSource(shaderWerteksów, 1, &kodWerteks, NULL);
	// Kompiluje shader werteksów
	glCompileShader(shaderWerteksów);
	// Sprawdza czy shader zosta³ skompilowany poprawnie
	kompilujShadery(shaderWerteksów, "VERTEX");

	// Tworzy obiekt shadera fragmentów i referencjê do niego
	GLuint shaderFragmentów = glCreateShader(GL_FRAGMENT_SHADER);
	// Do³¹cza kod shadera fragmentów do obiektu
	glShaderSource(shaderFragmentów, 1, &kodFragment, NULL);
	// Kompiluje shader fragmentów
	glCompileShader(shaderFragmentów);
	// Sprawdza czy shader zosta³ skompilowany poprawnie
	kompilujShadery(shaderFragmentów, "FRAGMENT");

	// Tworzy obiekt programu shaderów i referencjê do niego
	ID = glCreateProgram();
	// Do³¹cz shader werteksów i shader fragmentów do programu shaderów
	glAttachShader(ID, shaderWerteksów);
	glAttachShader(ID, shaderFragmentów);
	// Po³¹cz wszystkie shadery w program shaderów
	glLinkProgram(ID);
	// Sprawdza czy shadery zosta³y po³¹czone poprawnie
	kompilujShadery(ID, "PROGRAM");

	// Usuwa zbêdne ju¿ shadery
	glDeleteShader(shaderWerteksów);
	glDeleteShader(shaderFragmentów);

}

// Aktywuje program shaderów
void Shader::Aktywuj()
{
	glUseProgram(ID);
}

// Usuwa program shaderów
void Shader::Usuñ()
{
	glDeleteProgram(ID);
}

// Sprawdza czy shader siê skompilowa³
void Shader::kompilujShadery(unsigned int shader, const char* typ)
{
	// Sk³aduje status kompilacji
	GLint skompilowano;
	// Sk³aduje wiadomoœæ b³êdu
	char infoLog[1024];
	if (typ != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &skompilowano);
		if (skompilowano == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "B£¥D_KOMPILACJI_SHADERA - " << typ << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &skompilowano);
		if (skompilowano == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "B£¥D_£¥CZENIA_SHADERÓW - " << typ << "\n" << infoLog << std::endl;
		}
	}
}