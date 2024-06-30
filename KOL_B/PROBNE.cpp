#define _CRT_SECURE_NO_WARNINGS
using namespace std;

/*
* Uwaga: Zadania kod rozwiazania zaania umieszczać pomiędzy #ifdef a #endif.
*/

#pragma once
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <cmath>
#include <iostream>
#include <cassert>
//###############################################
enum class aktywnosc { nieaktywne, aktywne, };//#		//AKTYWNOSC
//###############################################



//#################################################################		//KLIENT
class Klient {
private:
	char nazwaKlienta[50];
public:
	void setKlient(const char* nazwa) {
		assert(nazwa != nullptr);
		strcpy(nazwaKlienta, nazwa);
	}
	const char* getNazwa() const { return nazwaKlienta; }
	Klient(const char* nazwa) { setKlient(nazwa); }
};
//#################################################################		//KLIENT



//#################################################################		//KONTO BANKOWE
class kontoBankowe {
protected:
	int nrKonta[26];
	double stanKonta;
	aktywnosc aktKonta;
	Klient* KlientBank;
	static int liczbaKont;

	//Metoda prywatne													//KONTO BANK - METODY
	static void inkrementuj_LiczKont() { liczbaKont++; }
	static void dekrementuj_LiczKont() { liczbaKont--; }

public:

	//Metoda użytkowa
	void generujNr() {
		srand(time(NULL));
		for (int i = 0; i < 26; i++)
		{
			nrKonta[i] = (rand() % 10);
		}
	}

	virtual double prognoza() { return stanKonta; }


	//Setery															//KONTO BANK - SETERY
	void setStan(double stan) {
		if (stan >= 0)
			stanKonta = stan;
		else stanKonta = 0;
	}

	void setAkt(aktywnosc akt) {
		if (aktKonta == aktywnosc::aktywne && akt == aktywnosc::nieaktywne)
			dekrementuj_LiczKont();

		aktKonta = akt;
	}

	void setKlient2(Klient* nowyKlient) { KlientBank = nowyKlient; }

	void setNrKonta(const char* numer) {
		if (strlen(numer) == 26) {
			char tab[27];
			strcpy(tab, numer);

			for (int i = 0; i < 26; i++)
			{
				nrKonta[i] = ((int)tab[i] - 48);
			}
		}
		else generujNr();
	}

	//Getery															//KONTO BANK - GETERY
	int* get_nrKonta() { return nrKonta; }

	double get_stanKonta() { return stanKonta; }

	aktywnosc get_aktywnosc() { return aktKonta; }

	Klient* get_Klient() { return KlientBank; }

	static int get_liczKont() { return liczbaKont; }

	//Konstruktor														//KONTO BANK - KONSTRUKTORY
	kontoBankowe() {
	}

	kontoBankowe(double stan, aktywnosc akt, Klient* nowyKlient, const char* numer) {
		setStan(stan);
		setNrKonta(numer);
		setAkt(akt);
		setKlient2(nowyKlient);
		if (akt == aktywnosc::aktywne)
			inkrementuj_LiczKont();
	}

	//Destruktor														//KONTO BANK - DESTRUKTOR
	~kontoBankowe() {
		if (this->get_aktywnosc() == aktywnosc::aktywne)
			dekrementuj_LiczKont();
	}

	//Konwersje															//KONTO BANK - KONWERSJE
	explicit operator double() {
		return stanKonta;
	}

	static kontoBankowe fromKlient(Klient* klient) {
		kontoBankowe* konto = new kontoBankowe;
		konto->setStan(0);
		konto->generujNr();
		konto->setAkt(aktywnosc::aktywne);
		konto->KlientBank = klient;
		inkrementuj_LiczKont();
		return *konto;
	}
	//Operatory															//KONTO BANK - OPERATORY
	kontoBankowe& operator+=(double liczba) {
		double noweSaldo = this->get_stanKonta() + liczba;
		this->setStan(noweSaldo);
		return *this;
	}
	friend ostream& operator << (ostream& os, kontoBankowe& konto);
};

ostream& operator << (ostream& os, kontoBankowe& konto) {
	os << "[";
	for (int i = 0; i <= 25; i++) {
		int* Klin = konto.get_nrKonta();
		os << Klin[i];
	}
	if (konto.get_aktywnosc() == aktywnosc::aktywne) {
		os << "] [" << konto.get_Klient()->getNazwa() << "] : [" << konto.get_stanKonta() << "]" << endl;
	}
	else {
		os << "] [NIEAKTYWNE]" << endl;
	}
	return os;
}
//#################################################################		//KONTO BANKOWE



//#################################################################		//KONTO OCZEDNOSCIOWE
class kontoOszczednoscowe : public kontoBankowe {
private:
	double stopaOproc;
public:
	//Setery															//KONTO OSZCZEDNOSCIOWE - SETERY
	void setStopa(double stopa) {
		if (stopa < 0)
			stopa = 0;
		else if (stopa > 100)
			stopa = 100;
		stopaOproc = stopa;
	}
	//Getery															//KONTO OSZCZEDNOSCIOWE - GETERY
	double get_Stopa() { return stopaOproc; }

	double prognoza() override { return (get_stanKonta() * (1 + (stopaOproc / 100))); }

	//Konstruktor														//KONTO OSZCZEDNOSCIOWE - KONSTRUKTORY
	kontoOszczednoscowe(double stan, aktywnosc akt, Klient* nowyKlient, const char* numer, double stopa) : kontoBankowe(stan, akt, nowyKlient, numer) {
		setStopa(stopa);
	}
};
//#################################################################		//KONTO OCZEDNOSCIOWE


int kontoBankowe::liczbaKont = 0;										//INICJALIZACJA POLA STATYCZNEGO

int main() {
	{
		kontoBankowe konto1(777, aktywnosc::aktywne, new Klient("Bartek"), "12345678911234567892123456");
		kontoBankowe konto4(777, aktywnosc::nieaktywne, new Klient("Andrzej"), "12345678911234567892123456");
		cout << (double)konto1 << endl;

		Klient klient = "Adi";
		kontoBankowe konto2 = kontoBankowe::fromKlient(&klient);

		Klient klient2 = "Adam";
		kontoBankowe konto3 = kontoBankowe::fromKlient(&klient2);

		konto2 += 2;
		cout << konto1;
		cout << konto2;
		cout << konto3;
		cout << konto4;
		cout << konto2.get_liczKont() << endl;
	}

	kontoBankowe konto5(777, aktywnosc::aktywne, new Klient("Bartek"), "12345678911234567892123456");
	kontoOszczednoscowe konto6(888, aktywnosc::aktywne, new Klient("Zbyszek"), "12345678911234567892123456", 50);
	cout << konto5;
	cout << konto5.get_liczKont() << endl;
	cout << konto5.prognoza() << endl;
	cout << konto6.prognoza() << endl;
	cout << konto6;
};
