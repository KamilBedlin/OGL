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
	// ID programu shader�w
	GLuint ID;
	// Buduje program shader�w z shadera werteks�w i shadera fragment�w
	Shader(const char* plikShaderaWerteks�w, const char* plikShaderaFragment�w);

	// Aktywuje program shader�w
	void Aktywuj();
	// Usuwa program shader�w
	void Usu�();
private:
	// Sprawdza czy shader si� skompilowa�
	void kompilujShadery(unsigned int shader, const char* typ);
};


#endif