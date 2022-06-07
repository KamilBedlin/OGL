#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Aktor.h"

using json = nlohmann::json;


class Model
{
public:
	// �aduje model z pliku i przechowuje go w zmiennych
	Model(const char* plik);

	void Rysuj(Shader& shader, Kamera& kamera);

private:
	// Zmienne do �atwego dost�pu
	const char* plik;
	std::vector<unsigned char> dane;
	json JSON;

	// Wszyscy aktorzy i transformacje
	std::vector<Aktor> aktorzy;
	std::vector<glm::vec3> pozycjeAktor�w;
	std::vector<glm::quat> rotacjeAktor�w;
	std::vector<glm::vec3> skaleAktor�w;
	std::vector<glm::mat4> macierzAktor�w;

	// Nie pozwala teksturom wczyta� si� 2 razy
	std::vector<std::string> nazwaWczytanejTekst;
	std::vector<Tekstura> wczytanaTekst;

	// Wczytuje pojedy�czego aktora po jego indeksie
	void wczytajAktora(unsigned int indAktor);

	// Przechodzi rekursywnie przez wszystkie w�z�y
	void przeszukajW�z�y(unsigned int nast�pnyW�ze�, glm::mat4 macierz = glm::mat4(1.0f));

	// Pobiera binarne dane z pliku
	std::vector<unsigned char> pobierzDane();
	// Interpretuje pobrane dane
	std::vector<float> pobierzFloaty(json accessor);
	std::vector<GLuint> pobierzIndeksy(json accessor);
	std::vector<Tekstura> pobierzTekstury();

	// ��czy floaty w werteksy
	std::vector<Werteks> ��czWWerteksy
	(
		std::vector<glm::vec3> pozycje,
		std::vector<glm::vec3> normalne,
		std::vector<glm::vec2> tekstury
	);

	// Pomagaj� z ��czeniem w werteksy powy�ej
	std::vector<glm::vec2> grupujFloatyWVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> grupujFloatyWVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> grupujFloatyWVec4(std::vector<float> floatVec);
};
#endif