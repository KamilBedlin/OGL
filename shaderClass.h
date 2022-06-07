#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* nazwaPliku);

class Shader
{
public:
	// ID programu shaderów
	GLuint ID;
	// Buduje program shaderów z shadera werteksów i shadera fragmentów
	Shader(const char* plikShaderaWerteksów, const char* plikShaderaFragmentów);

	// Aktywuje program shaderów
	void Aktywuj();
	// Usuwa program shaderów
	void Usuñ();
private:
	// Sprawdza czy shader siê skompilowa³
	void kompilujShadery(unsigned int shader, const char* typ);
};


#endif