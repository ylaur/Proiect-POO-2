#include "Personaje.h"

int Entitate::contorEntitati = 0;

// Constructor de baza Entitate; Validare cu throw si prelucrare atribut static
Entitate::Entitate(std::string n, int h, int d) : nume(std::move(n)), hpCurent(h), hpMaxim(h), dauneBaza(d) {
    if (h <= 0) throw ConfigurareInvalida("HP zero!");
    contorEntitati++;
}

// Destructor virtual Entitate; Actualizeaza numarul de entitati
Entitate::~Entitate() { contorEntitati--; }

// Getter: verifica daca viata este peste zero
bool Entitate::esteViu() const { return hpCurent > 0; }

// Getter: returneaza numele entitatii
std::string Entitate::getNume() const { return nume; }

// Getter: returneaza HP-ul curent
int Entitate::getHP() const { return hpCurent; }

// Getter: returneaza HP-ul maxim posibil
int Entitate::getHPMax() const { return hpMaxim; }

// Getter: returneaza daunele de baza
int Entitate::getDauneBaza() const { return dauneBaza; }

// Scade HP-ul si asigura ca nu pica sub 0
void Entitate::incaseazaDaune(int daune) { hpCurent -= daune; if (hpCurent < 0) hpCurent = 0; }

// Adauga HP si limiteaza la HP maxim
void Entitate::primesteVindecare(int v) { hpCurent += v; if (hpCurent > hpMaxim) hpCurent = hpMaxim; }

// Metoda pentru afisare in consola
void Entitate::afiseazaStatusCard() const {
    std::cout << "["; afisare_identitate(); std::cout << "] " << nume << " - HP: " << hpCurent << "/" << hpMaxim;
}

// Functie statica; Returneaza numarul global de entitati in memorie
int Entitate::obtineNumarEntitati() { return contorEntitati; }

// Constructor Erou; Seteaza armaEchipata pe null initial
Erou::Erou(std::string n, int h, int d) : Entitate(std::move(n), h, d), armaEchipata(nullptr) {}

// Regula celor 3: Destructor pentru pointerul Arma
Erou::~Erou() { delete armaEchipata; }

//  Regula celor 3: Copy Constructor folosind functia clone()
Erou::Erou(const Erou& other) : Entitate(other) {
    armaEchipata = other.armaEchipata ? other.armaEchipata->clone() : nullptr;
}

//  Regula celor 3: Suprascris op= pentru atribuiri (asignari) corecte
Erou& Erou::operator=(const Erou& other) {
    if (this != &other) {
        Entitate::operator=(other);
        delete armaEchipata;
        armaEchipata = other.armaEchipata ? other.armaEchipata->clone() : nullptr;
    }
    return *this;
}

// Functie de setare pentru atributul tip pointer de baza, realizand clonare
void Erou::atribuieArma(const Arma& a) { delete armaEchipata; armaEchipata = a.clone(); }

// Getter pentru pointerul Arma
Arma* Erou::getArma() const { return armaEchipata; }

// Aici apelam functiile virtuale prin pointer de baza (calculeazaDauneEfective)
int Erou::calculeazaFortaTotala() const { return dauneBaza + (armaEchipata ? armaEchipata->calculeazaDauneEfective() : 0); }

// Afiseaza statusul eroului + detaliile armei (daca are)
void Erou::afisareCompleta() const {
    afiseazaStatusCard();
    std::cout << " | ";
    if (armaEchipata) armaEchipata->afiseazaArma(); else std::cout << "FARA ARMA";
    std::cout << " | Forta: " << calculeazaFortaTotala() << "\n";
}

// Apelarea constructorului din clasa de baza (Erou) + adaugare arma
Cavaler::Cavaler(std::string n) : Erou(std::move(n), 150, 15) { atribuieArma(Sabie(10)); }

// Implementare functie virtuala pura
void Cavaler::afisare_identitate() const { std::cout << "Cavaler"; }

// Apelare constructor clasa de baza
Print::Print(std::string n) : Erou(std::move(n), 120, 20) { atribuieArma(Lance(15)); }

// Implementare functie virtuala pura
void Print::afisare_identitate() const { std::cout << "Print"; }

// Apelare constructor clasa de baza
Arcas::Arcas(std::string n) : Erou(std::move(n), 100, 25) { atribuieArma(Arc(18)); }

// Implementare functie virtuala pura
void Arcas::afisare_identitate() const { std::cout << "Arcas"; }

// Apelare constructor clasa de baza
Magician::Magician(std::string n) : Erou(std::move(n), 80, 30) { atribuieArma(Toiag(22)); }

// Implementare functie virtuala pura
void Magician::afisare_identitate() const { std::cout << "Magician"; }

// Implementare functie virtuala pura pentru inamic
void Inamic::afisare_identitate() const { std::cout << "Inamic"; }
