#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Aktor.h"

using json = nlohmann::json;


class Model
{
public:
	// £aduje model z pliku i przechowuje go w zmiennych
	Model(const char* plik);

	void Rysuj(Shader& shader, Kamera& kamera);

private:
	// Zmienne do ³atwego dostêpu
	const char* plik;
	std::vector<unsigned char> dane;
	json JSON;

	// Wszyscy aktorzy i transformacje
	std::vector<Aktor> aktorzy;
	std::vector<glm::vec3> pozycjeAktorów;
	std::vector<glm::quat> rotacjeAktorów;
	std::vector<glm::vec3> skaleAktorów;
	std::vector<glm::mat4> macierzAktorów;

	// Nie pozwala teksturom wczytaæ siê 2 razy
	std::vector<std::string> nazwaWczytanejTekst;
	std::vector<Tekstura> wczytanaTekst;

	// Wczytuje pojedyñczego aktora po jego indeksie
	void wczytajAktora(unsigned int indAktor);

	// Przechodzi rekursywnie przez wszystkie wêz³y
	void przeszukajWêz³y(unsigned int nastêpnyWêze³, glm::mat4 macierz = glm::mat4(1.0f));

	// Pobiera binarne dane z pliku
	std::vector<unsigned char> pobierzDane();
	// Interpretuje pobrane dane
	std::vector<float> pobierzFloaty(json accessor);
	std::vector<GLuint> pobierzIndeksy(json accessor);
	std::vector<Tekstura> pobierzTekstury();

	// £¹czy floaty w werteksy
	std::vector<Werteks> ³¹czWWerteksy
	(
		std::vector<glm::vec3> pozycje,
		std::vector<glm::vec3> normalne,
		std::vector<glm::vec2> tekstury
	);

	// Pomagaj¹ z ³¹czeniem w werteksy powy¿ej
	std::vector<glm::vec2> grupujFloatyWVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> grupujFloatyWVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> grupujFloatyWVec4(std::vector<float> floatVec);
};
#endif