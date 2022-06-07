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
	przeszukajWêz³y(0);
}

void Model::Rysuj(Shader& shader, Kamera& kamera)
{
	// Rysuje wszystkich aktorów
	for (unsigned int i = 0; i < aktorzy.size(); i++)
	{
		aktorzy[i].Aktor::Rysuj(shader, kamera, macierzAktorów[i]);
	}
}

void Model::wczytajAktora(unsigned int indAktora)
{
	// Pobierz wszystkie indeksy akcesora
	unsigned int pozAkcInd = JSON["meshes"][indAktora]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAkcInd = JSON["meshes"][indAktora]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int tekstAkcInd = JSON["meshes"][indAktora]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAkcInd = JSON["meshes"][indAktora]["primitives"][0]["indices"];

	// Pobierz wszystkie komponenty werteksów po indeksach akcesora
	std::vector<float> pozVec = pobierzFloaty(JSON["accessors"][pozAkcInd]);
	std::vector<glm::vec3> pozycje = grupujFloatyWVec3(pozVec);
	std::vector<float> normalVec = pobierzFloaty(JSON["accessors"][normalAkcInd]);
	std::vector<glm::vec3> normalne = grupujFloatyWVec3(normalVec);
	std::vector<float> tekstVec = pobierzFloaty(JSON["accessors"][tekstAkcInd]);
	std::vector<glm::vec2> tekstWspó³rzêdne = grupujFloatyWVec2(tekstVec);

	// Po³¹cz komponenty werteksów oraz pobierz indeksy i tekstury
	std::vector<Werteks> werteksy = ³¹czWWerteksy(pozycje, normalne, tekstWspó³rzêdne);
	std::vector<GLuint> indeksy = pobierzIndeksy(JSON["accessors"][indAkcInd]);
	std::vector<Tekstura> tekstury = pobierzTekstury();

	// Po³¹cz powy¿sze w aktora
	aktorzy.push_back(Aktor(werteksy, indeksy, tekstury));
}

void Model::przeszukajWêz³y(unsigned int nastêpnyWêze³, glm::mat4 macierz)
{
	// Obecny wêze³
	json wêze³ = JSON["nodes"][nastêpnyWêze³];

	// Pobierz pozycjê, jeœli jest
	glm::vec3 pozycja = glm::vec3(0.0f, 0.0f, 0.0f);
	if (wêze³.find("translation") != wêze³.end())
	{
		float pozWartoœci[3];
		for (unsigned int i = 0; i < wêze³["translation"].size(); i++)
			pozWartoœci[i] = (wêze³["translation"][i]);
		pozycja = glm::make_vec3(pozWartoœci);
	}
	// Pobierz rotacjê, jeœli jest
	glm::quat rotacja = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (wêze³.find("rotation") != wêze³.end())
	{
		float rotWartoœci[4] =
		{
			wêze³["rotation"][3],
			wêze³["rotation"][0],
			wêze³["rotation"][1],
			wêze³["rotation"][2]
		};
		rotacja = glm::make_quat(rotWartoœci);
	}
	// Pobierz skalê, jeœli jest
	glm::vec3 skala = glm::vec3(1.0f, 1.0f, 1.0f);
	if (wêze³.find("scale") != wêze³.end())
	{
		float skalaWartoœci[3];
		for (unsigned int i = 0; i < wêze³["scale"].size(); i++)
			skalaWartoœci[i] = (wêze³["scale"][i]);
		skala = glm::make_vec3(skalaWartoœci);
	}
	// Pobierz macierz, jeœli jest
	glm::mat4 macierzWêze³ = glm::mat4(1.0f);
	if (wêze³.find("matrix") != wêze³.end())
	{
		float macierzWartoœci[16];
		for (unsigned int i = 0; i < wêze³["matrix"].size(); i++)
			macierzWartoœci[i] = (wêze³["matrix"][i]);
		macierzWêze³ = glm::make_mat4(macierzWartoœci);
	}

	// Inicjuje macierze
	glm::mat4 poz = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 ska = glm::mat4(1.0f);

	// Use translation, rotation, and scale to change the initialized matrices
	poz = glm::translate(poz, pozycja);
	rot = glm::mat4_cast(rotacja);
	ska = glm::scale(ska, skala);

	// Mno¿y wszystkie macierze
	glm::mat4 macierzNastWêz³a = macierz * macierzWêze³ * poz * rot * ska;

	// Sprawdza czy wêze³ posiada aktora i czy mo¿e go za³adowaæ
	if (wêze³.find("mesh") != wêze³.end())
	{
		pozycjeAktorów.push_back(pozycja);
		rotacjeAktorów.push_back(rotacja);
		skaleAktorów.push_back(skala);
		macierzAktorów.push_back(macierzNastWêz³a);

		wczytajAktora(wêze³["mesh"]);
	}

	// Sprawdza, czy wêze³ ma dzieci i jeœli ma wywo³uje t¹ funkcjê dla niego
	if (wêze³.find("children") != wêze³.end())
	{
		for (unsigned int i = 0; i < wêze³["children"].size(); i++)
			przeszukajWêz³y(wêze³["children"][i], macierzNastWêz³a);
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

	// Pobierz w³aœciwoœci z akcesora
	unsigned int buffViewInd = akcesor.value("bufferView", 1);
	unsigned int count = akcesor["count"];
	unsigned int akcByteOffset = akcesor.value("byteOffset", 0);
	std::string type = akcesor["type"];

	// Pobierz w³aœciwoœci z bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Interpretuj typ i przechowaj w zmiennej
	unsigned int liczbaNaWerteks;
	if (type == "SCALAR") liczbaNaWerteks = 1;
	else if (type == "VEC2") liczbaNaWerteks = 2;
	else if (type == "VEC3") liczbaNaWerteks = 3;
	else if (type == "VEC4") liczbaNaWerteks = 4;
	else throw std::invalid_argument("Typ nieprawid³owy (nie SCALAR, VEC2, VEC3, ani VEC4)");

	// PrzejdŸ przez wszystkie zapisane bajty u¿ywaj¹c powy¿szych w³aœciwoœci
	unsigned int pocz¹tekDanych = byteOffset + akcByteOffset;
	unsigned int d³ugoœæDanych = count * 4 * liczbaNaWerteks;
	for (unsigned int i = pocz¹tekDanych; i < pocz¹tekDanych + d³ugoœæDanych; i)
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

	// Pobierz w³aœciwoœci z akcesora
	unsigned int buffViewInd = akcesor.value("bufferView", 0);
	unsigned int count = akcesor["count"];
	unsigned int accByteOffset = akcesor.value("byteOffset", 0);
	unsigned int componentType = akcesor["componentType"];

	// Pobierz w³aœciwoœci z bufferView
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	// Pobierz indeksy uwzglêdniaj¹c typy
	unsigned int pocz¹tekDanych = byteOffset + accByteOffset;
	if (componentType == 5125)
	{
		for (unsigned int i = pocz¹tekDanych; i < byteOffset + accByteOffset + count * 4; i)
		{
			unsigned char bajty[] = { dane[i++], dane[i++], dane[i++], dane[i++] };
			unsigned int wart;
			std::memcpy(&wart, bajty, sizeof(unsigned int));
			indeksy.push_back((GLuint)wart);
		}
	}
	else if (componentType == 5123)
	{
		for (unsigned int i = pocz¹tekDanych; i < byteOffset + accByteOffset + count * 2; i)
		{
			unsigned char bajty[] = { dane[i++], dane[i++] };
			unsigned short wart;
			std::memcpy(&wart, bajty, sizeof(unsigned short));
			indeksy.push_back((GLuint)wart);
		}
	}
	else if (componentType == 5122)
	{
		for (unsigned int i = pocz¹tekDanych; i < byteOffset + accByteOffset + count * 2; i)
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

	// PrzejdŸ przez obrazy
	for (unsigned int i = 0; i < JSON["images"].size(); i++)
	{
		// uri obecnego obrazu
		std::string tekstURI = JSON["images"][i]["uri"];

		// Sprawdza, czy tekstura zosta³a ju¿ za³adowana
		bool pomiñ = false;
		for (unsigned int j = 0; j < nazwaWczytanejTekst.size(); j++)
		{
			if (nazwaWczytanejTekst[j] == tekstURI)
			{
				tekstury.push_back(wczytanaTekst[j]);
				pomiñ = true;
				break;
			}
		}

		// Jeœli tesktura zosta³a ju¿ wczytana pomiñ
		if (!pomiñ)
		{
			// Za³aduj diffused teksturê
			if (tekstURI.find("baseColor") != std::string::npos)
			{
				Tekstura diffuse = Tekstura((folderPliku + tekstURI).c_str(), "diffuse", wczytanaTekst.size());
				tekstury.push_back(diffuse);
				wczytanaTekst.push_back(diffuse);
				nazwaWczytanejTekst.push_back(tekstURI);
			}
			// Za³aduj specular teksturê
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

std::vector<Werteks> Model::³¹czWWerteksy
(
	std::vector<glm::vec3> pozycje,
	std::vector<glm::vec3> normalne,
	std::vector<glm::vec2> tekstWspó³rzêdne
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
				tekstWspó³rzêdne[i]
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