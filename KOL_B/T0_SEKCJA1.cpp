#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#pragma once
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cmath>
#include <iostream>
#include <cassert>

enum class Dostepnosc
{
	Dostepna,
	Niedostepna,
};

class Autor {
private:
	char nazwisko[50];
public:
	void setAutor(const char* nazwa) {
		assert(nazwa != nullptr);
		strcpy(nazwisko, nazwa);
	}
	const char* getNazwa() const { return nazwisko; }
	Autor(const char* nazwa) { setAutor(nazwa); }
};


class Ksiazka
{
protected:
	char tytul[50];
	int strony;
	Dostepnosc dostep;
	Autor* autor;

	static Ksiazka* wzorzec;
public:
	virtual double cena() { return ((double)get_strony()); }

	Ksiazka() {
		assert(wzorzec != nullptr);
		set_all(wzorzec->get_tytul(), wzorzec->get_strony(), wzorzec->get_dostep(), wzorzec->get_Autor());
	};

	Ksiazka(const char* nowyTytul, int ilosc, Dostepnosc nowyDostep, Autor* nowyAutor) {
		assert(nowyAutor != nullptr);
		set_all(nowyTytul, ilosc, nowyDostep, nowyAutor);
	};


	void set_tytul(const char* nowyTytul) {
		assert(nowyTytul != nullptr);
		strcpy(tytul, nowyTytul);
	}

	void set_strony(int ilosc) {
		if (ilosc < 5) { strony = 5; }
		else strony = ilosc;
	}

	void set_dostep(Dostepnosc nowyDostep) { dostep = nowyDostep; }

	void set_Autor(Autor* nowyAutor) {
		assert(nowyAutor != nullptr);
		autor = nowyAutor;
	}

	static void set_wzorzec(Ksiazka* wzor) { wzorzec = wzor; }

	void set_all(const char* nowyTytul, int ilosc, Dostepnosc nowyDostep, Autor* nowyAutor) {
		set_tytul(nowyTytul);
		set_strony(ilosc);
		set_dostep(nowyDostep);
		set_Autor(nowyAutor);
	}


	const char* get_tytul() const { return tytul; }
	int get_strony() { return strony; }
	Dostepnosc get_dostep() { return dostep; }
	Autor* get_Autor() { return autor; }

	explicit operator const char* () {

		const char* title = get_tytul();
		const char* author = get_Autor()->getNazwa();
		char* opis = new char[100];
		strcpy(opis, title);
		strcat(opis, ", ");
		strcat(opis, author);
		return (opis);
	}

	bool operator==(Ksiazka& ksi)
	{

		if (this != &ksi) {
			if (this->get_strony() != ksi.get_strony())
				return false;

			if (this->get_Autor() != ksi.get_Autor())
				return false;
			if (this->get_dostep() != ksi.get_dostep())
				return false;
		}
		return true;

	}
};
Ksiazka* Ksiazka::wzorzec = nullptr;

class EBook : public Ksiazka {
private:
	int rozmiar;
public:
	void set_rozmiar(int size) {
		if (size < 1) rozmiar = 1;
		else rozmiar = size;
	}

	int get_rozmiar() { return rozmiar; }

	EBook(const char* nowyTytul, int ilosc, Dostepnosc nowyDostep, Autor* nowyAutor, int size) : Ksiazka(nowyTytul, ilosc, nowyDostep, nowyAutor) {
		set_rozmiar(size);
	}

	double cena() override { return ((double)get_strony() - ((double)get_strony() * 0.25)); }


};


int main() {
	Autor Bartlomiej = "Pawlik";
	Ksiazka ksiazka1("nowyTytul", 5, Dostepnosc::Dostepna, &Bartlomiej);
	EBook EBook1("bebook", 6, Dostepnosc::Dostepna, &Bartlomiej, 10);
	cout << ksiazka1.get_tytul() << ", " << ksiazka1.get_strony() << ", " << (bool)ksiazka1.get_dostep() << ", " << ksiazka1.get_Autor()->getNazwa() << endl;
	cout << ksiazka1.cena() << endl;

	Ksiazka::set_wzorzec(&ksiazka1);
	Ksiazka ksiazka2;
	cout << ksiazka2.get_tytul() << ", " << ksiazka2.get_strony() << ", " << (bool)ksiazka2.get_dostep() << ", " << ksiazka2.get_Autor()->getNazwa() << endl;
	cout << ksiazka2.cena() << endl;

	cout << "Czy rozne: " << (ksiazka1 == ksiazka2) << endl;
	cout << "Czy rozne: " << (ksiazka1 == EBook1) << endl;

	cout << EBook1.get_tytul() << ", " << EBook1.get_strony() << ", " << (bool)EBook1.get_dostep() << ", " << EBook1.get_Autor()->getNazwa() << endl;
	cout << EBook1.cena() << endl << endl;
	const char* ksi = static_cast<const char*>(ksiazka1);
	cout << ksi;
};
