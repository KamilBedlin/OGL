#include"Model.h"

Model::Model(const char* file)
{
	// Tworzy obiekt JSON
	std::string tekst = get_file_contents(file);
	JSON = json::parse(tekst);

	// Pobierz binarne dane
	Model::plik = file;
	dane = pobierzDane();

	// Traverse all nodes
	przeszukajW�z�y(0);
}

void Model::Rysuj(Shader& shader, Kamera& kamera)
{
	// Rysuje wszystkich aktor�w
	for (unsigned int i = 0; i < aktorzy.size(); i++)
	{
		aktorzy[i].Aktor::Rysuj(shader, kamera, macierzAktor�w[i]);
	}
}

void Model::wczytajAktora(unsigned int indAktora)
{
	// Pobierz wszystkie indeksy akcesora
	unsigned int pozAkcInd = JSON["meshes"][indAktora]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAkcInd = JSON["meshes"][indAktora]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int tekstAkcInd = JSON["meshes"][indAktora]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAkcInd = JSON["meshes"][indAktora]["primitives"][0]["indices"];

	// Pobierz wszystkie komponenty werteks�w po indeksach akcesora
	std::vector<float> pozVec = pobierzFloaty(JSON["accessors"][pozAkcInd]);
	std::vector<glm::vec3> pozycje = grupujFloatyWVec3(pozVec);
	std::vector<float> normalVec = pobierzFloaty(JSON["accessors"][normalAkcInd]);
	std::vector<glm::vec3> normalne = grupujFloatyWVec3(normalVec);
	std::vector<float> tekstVec = pobierzFloaty(JSON["accessors"][tekstAkcInd]);
	std::vector<glm::vec2> tekstWsp�rz�dne = grupujFloatyWVec2(tekstVec);

	// Po��cz komponenty werteks�w oraz pobierz indeksy i tekstury
	std::vector<Werteks> werteksy = ��czWWerteksy(pozycje, normalne, tekstWsp�rz�dne);
	std::vector<GLuint> indeksy = pobierzIndeksy(JSON["accessors"][indAkcInd]);
	std::vector<Tekstura> tekstury = pobierzTekstury();

	// Po��cz powy�sze w aktora
	aktorzy.push_back(Aktor(werteksy, indeksy, tekstury));
}

void Model::przeszukajW�z�y(unsigned int nast�pnyW�ze�, glm::mat4 macierz)
{
	// Obecny w�ze�
	json w�ze� = JSON["nodes"][nast�pnyW�ze�];

	// Pobierz pozycj�, je�li jest
	glm::vec3 pozycja = glm::vec3(0.0f, 0.0f, 0.0f);
	if (w�ze�.find("translation") != w�ze�.end())
	{
		float pozWarto�ci[3];
		for (unsigned int i = 0; i < w�ze�["translation"].size(); i++)
			pozWarto�ci[i] = (w�ze�["translation"][i]);
		pozycja = glm::make_vec3(pozWarto�ci);
	}
	// Pobierz rotacj�, je�li jest
	glm::quat rotacja = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (w�ze�.find("rotation") != w�ze�.end())
	{
		float rotWarto�ci[4] =
		{
			w�ze�["rotation"][3],
			w�ze�["rotation"][0],
			w�ze�["rotation"][1],
			w�ze�["rotation"][2]
		};
		rotacja = glm::make_quat(rotWarto�ci);
	}
	// Pobierz skal�, je�li jest
	glm::vec3 skala = glm::vec3(1.0f, 1.0f, 1.0f);
	if (w�ze�.find("scale") != w�ze�.end())
	{
		float skalaWarto�ci[3];
		for (unsigned int i = 0; i < w�ze�["scale"].size(); i++)
			skalaWarto�ci[i] = (w�ze�["scale"][i]);
		skala = glm::make_vec3(skalaWarto�ci);
	}
	// Pobierz macierz, je�li jest
	glm::mat4 macierzW�ze� = glm::mat4(1.0f);
	if (w�ze�.find("matrix") != w�ze�.end())
	{
		float macierzWarto�ci[16];
		for (unsigned int i = 0; i < w�ze�["matrix"].size(); i++)
			macierzWarto�ci[i] = (w�ze�["matrix"][i]);
		macierzW�ze� = glm::make_mat4(macierzWarto�ci);
	}

	// Inicjuje macierze
	glm::mat4 poz = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 ska = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	poz = glm::translate(poz, pozycja);
	rot = glm::mat4_cast(rotacja);
	ska = glm::scale(ska, skala);

	// Mno�y wszystkie macierze
	glm::mat4 macierzNastW�z�a = macierz * macierzW�ze� * poz * rot * ska;

	// Sprawdza czy w�ze� posiada aktora i czy mo�e go za�adowa�
	if (w�ze�.find("mesh") != w�ze�.end())
	{
		pozycjeAktor�w.push_back(pozycja);
		rotacjeAktor�w.push_back(rotacja);
		skaleAktor�w.push_back(skala);
		macierzAktor�w.push_back(macierzNastW�z�a);

		wczytajAktora(w�ze�["mesh"]);
	}

	// Sprawdza, czy w�ze� ma dzieci i je�li ma wywo�uje t� funkcj� dla niego
	if (w�ze�.find("children") != w�ze�.end())
	{
		for (unsigned int i = 0; i < w�ze�["children"].size(); i++)
			przeszukajW�z�y(w�ze�["children"][i], macierzNastW�z�a);
	}
}

std::vector<unsigned char> Model::pobierzDane()
{
	// Tworzy miejsce do przechowywania tesktu z pliku
	std::string bajtyTekst;
	std::string uri = JSON["buffers"][0]["uri"];

	std::string plikStr = std::string(plik);
	std::string folderPliku = plikStr.substr(0, plikStr.find_last_of('/') + 1);
	bajtyTekst = get_file_contents((folderPliku + uri).c_str());

	// Zmienia czysty tekst w bajty i wstawia je do vectora
	std::vector<unsigned char> data(bajtyTekst.begin(), bajtyTekst.end());
	return data;
}

std::vector<float> Model::pobierzFloaty(json akcesor)
{
	std::vector<float> floatVec;

	// Pobierz w�a�ciwo�ci z akcesora
	unsigned int buffViewInd = akcesor.value("bufferView", 1);
	unsigned int count = akcesor["count"];
	unsigned int akcByteOffset = akcesor.value("byteOffset", 0);
	std::string type = akcesor["type"];

	// Pobierz w�a�ciwo�ci z bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpretuj typ i przechowaj w zmiennej
	unsigned int liczbaNaWerteks;
	if (type == "SCALAR") liczbaNaWerteks = 1;
	else if (type == "VEC2") liczbaNaWerteks = 2;
	else if (type == "VEC3") liczbaNaWerteks = 3;
	else if (type == "VEC4") liczbaNaWerteks = 4;
	else throw std::invalid_argument("Typ nieprawid�owy (nie SCALAR, VEC2, VEC3, ani VEC4)");

	// Przejd� przez wszystkie zapisane bajty u�ywaj�c powy�szych w�a�ciwo�ci
	unsigned int pocz�tekDanych = byteOffset + akcByteOffset;
	unsigned int d�ugo��Danych = count * 4 * liczbaNaWerteks;
	for (unsigned int i = pocz�tekDanych; i < pocz�tekDanych + d�ugo��Danych; i)
	{
		unsigned char bajty[] = { dane[i++], dane[i++], dane[i++], dane[i++] };
		float wart;
		std::memcpy(&wart, bajty, sizeof(float));
		floatVec.push_back(wart);
	}

	return floatVec;
}

std::vector<GLuint> Model::pobierzIndeksy(json akcesor)
{
	std::vector<GLuint> indeksy;

	// Pobierz w�a�ciwo�ci z akcesora
	unsigned int buffViewInd = akcesor.value("bufferView", 0);
	unsigned int count = akcesor["count"];
	unsigned int accByteOffset = akcesor.value("byteOffset", 0);
	unsigned int componentType = akcesor["componentType"];

	// Pobierz w�a�ciwo�ci z bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Pobierz indeksy uwzgl�dniaj�c typy
	unsigned int pocz�tekDanych = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = pocz�tekDanych; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bajty[] = { dane[i++], dane[i++], dane[i++], dane[i++] };
			unsigned int wart;
			std::memcpy(&wart, bajty, sizeof(unsigned int));
			indeksy.push_back((GLuint)wart);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = pocz�tekDanych; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bajty[] = { dane[i++], dane[i++] };
			unsigned short wart;
			std::memcpy(&wart, bajty, sizeof(unsigned short));
			indeksy.push_back((GLuint)wart);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = pocz�tekDanych; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bajty[] = { dane[i++], dane[i++] };
			short wart;
			std::memcpy(&wart, bajty, sizeof(short));
			indeksy.push_back((GLuint)wart);
		}
	}

	return indeksy;
}

std::vector<Tekstura> Model::pobierzTekstury()
{
	std::vector<Tekstura> tekstury;

	std::string plikStr = std::string(plik);
	std::string folderPliku = plikStr.substr(0, plikStr.find_last_of('/') + 1);

	// Przejd� przez obrazy
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri obecnego obrazu
		std::string tekstURI = JSON["images"][i]["uri"];

		// Sprawdza, czy tekstura zosta�a ju� za�adowana
		bool pomi� = false;
		for (unsigned int j = 0; j < nazwaWczytanejTekst.size(); j++)
		{
			if (nazwaWczytanejTekst[j] == tekstURI)
			{
				tekstury.push_back(wczytanaTekst[j]);
				pomi� = true;
				break;
			}
		}

		// Je�li tesktura zosta�a ju� wczytana pomi�
		if (!pomi�)
		{
			// Za�aduj diffused tekstur�
			if (tekstURI.find("baseColor") != std::string::npos)
			{
				Tekstura diffuse = Tekstura((folderPliku + tekstURI).c_str(), "diffuse", wczytanaTekst.size());
				tekstury.push_back(diffuse);
				wczytanaTekst.push_back(diffuse);
				nazwaWczytanejTekst.push_back(tekstURI);
			}
			// Za�aduj specular tekstur�
			else if (tekstURI.find("metallicRoughness") != std::string::npos)
			{
				Tekstura specular = Tekstura((folderPliku + tekstURI).c_str(), "specular", wczytanaTekst.size());
				tekstury.push_back(specular);
				wczytanaTekst.push_back(specular);
				nazwaWczytanejTekst.push_back(tekstURI);
			}
		}
	}

	return tekstury;
}

std::vector<Werteks> Model::��czWWerteksy
(
	std::vector<glm::vec3> pozycje,
	std::vector<glm::vec3> normalne,
	std::vector<glm::vec2> tekstWsp�rz�dne
)
{
	std::vector<Werteks> werteksy;
	for (int i = 0; i < pozycje.size(); i++)
	{
		werteksy.push_back
		(
			Werteks
			{
				pozycje[i],
				normalne[i],
				glm::vec3(1.0f, 1.0f, 1.0f),
				tekstWsp�rz�dne[i]
			}
		);
	}
	return werteksy;
}

std::vector<glm::vec2> Model::grupujFloatyWVec2(std::vector<float> floatVec)
{
	std::vector<glm::vec2> wektory;
	for (int i = 0; i < floatVec.size(); i)
	{
		wektory.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}
	return wektory;
}
std::vector<glm::vec3> Model::grupujFloatyWVec3(std::vector<float> floatVec)
{
	std::vector<glm::vec3> wektory;
	for (int i = 0; i < floatVec.size(); i)
	{
		wektory.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return wektory;
}
std::vector<glm::vec4> Model::grupujFloatyWVec4(std::vector<float> floatVec)
{
	std::vector<glm::vec4> wektory;
	for (int i = 0; i < floatVec.size(); i)
	{
		wektory.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}
	return wektory;
}