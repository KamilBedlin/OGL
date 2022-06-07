#include"shaderClass.h"

// Czyta plik i zwraca jego zawarto�� jako stringa
std::string get_file_contents(const char* nazwaPliku)
{
	std::ifstream plik(nazwaPliku, std::ios::binary);
	if (plik)
	{
		std::string zawarto��;
		plik.seekg(0, std::ios::end);
		zawarto��.resize(plik.tellg());
		plik.seekg(0, std::ios::beg);
		plik.read(&zawarto��[0], zawarto��.size());
		plik.close();
		return(zawarto��);
	}
	throw(errno);
}

// Buduje program shader�w z shadera werteks�w i shadera fragment�w
Shader::Shader(const char* plikShaderaWerteks�w, const char* plikShaderaFragment�w)
{
	// Czyta pliki shader�w to string�w
	std::string kodShaderaWerteks�w = get_file_contents(plikShaderaWerteks�w);
	std::string kodShaderaFragment�w = get_file_contents(plikShaderaFragment�w);

	// Konwertuje stringi w tablice char
	const char* kodWerteks = kodShaderaWerteks�w.c_str();
	const char* kodFragment = kodShaderaFragment�w.c_str();

	// Tworzy obiekt shadera werteks�w i referencj� do niego
	GLuint shaderWerteks�w = glCreateShader(GL_VERTEX_SHADER);
	// Do��cza kod shadera werteks�w do obiektu
	glShaderSource(shaderWerteks�w, 1, &kodWerteks, NULL);
	// Kompiluje shader werteks�w
	glCompileShader(shaderWerteks�w);
	// Sprawdza czy shader zosta� skompilowany poprawnie
	kompilujShadery(shaderWerteks�w, "VERTEX");

	// Tworzy obiekt shadera fragment�w i referencj� do niego
	GLuint shaderFragment�w = glCreateShader(GL_FRAGMENT_SHADER);
	// Do��cza kod shadera fragment�w do obiektu
	glShaderSource(shaderFragment�w, 1, &kodFragment, NULL);
	// Kompiluje shader fragment�w
	glCompileShader(shaderFragment�w);
	// Sprawdza czy shader zosta� skompilowany poprawnie
	kompilujShadery(shaderFragment�w, "FRAGMENT");

	// Tworzy obiekt programu shader�w i referencj� do niego
	ID = glCreateProgram();
	// Do��cz shader werteks�w i shader fragment�w do programu shader�w
	glAttachShader(ID, shaderWerteks�w);
	glAttachShader(ID, shaderFragment�w);
	// Po��cz wszystkie shadery w program shader�w
	glLinkProgram(ID);
	// Sprawdza czy shadery zosta�y po��czone poprawnie
	kompilujShadery(ID, "PROGRAM");

	// Usuwa zb�dne ju� shadery
	glDeleteShader(shaderWerteks�w);
	glDeleteShader(shaderFragment�w);

}

// Aktywuje program shader�w
void Shader::Aktywuj()
{
	glUseProgram(ID);
}

// Usuwa program shader�w
void Shader::Usu�()
{
	glDeleteProgram(ID);
}

// Sprawdza czy shader si� skompilowa�
void Shader::kompilujShadery(unsigned int shader, const char* typ)
{
	// Sk�aduje status kompilacji
	GLint skompilowano;
	// Sk�aduje wiadomo�� b��du
	char infoLog[1024];
	if (typ != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &skompilowano);
		if (skompilowano == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "B��D_KOMPILACJI_SHADERA - " << typ << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &skompilowano);
		if (skompilowano == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "B��D_��CZENIA_SHADER�W - " << typ << "\n" << infoLog << std::endl;
		}
	}
}