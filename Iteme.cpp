#include "Iteme.h"
#include "Personaje.h"
#include <cstdlib>

// Constructor Arma
Arma::Arma(int bonus) : bonusDaune(bonus) {
    if (bonus < 0) throw ConfigurareInvalida("Bonus negativ!");
}

// Interfata non-virtuala care apeleaza o functie virtuala pura
void Arma::afiseazaArma() const {
    descriere_impl();
    std::cout << " (+ " << bonusDaune << " Daune)";
}

// Getter simplu pentru daunele de baza ale armei
int Arma::getBonusDaune() const { return bonusDaune; }

// Adauga daune
void Arma::adaugaBonus(int valoare) { bonusDaune += valoare; }

// Functie virtuala de baza pentru calcularea daunelor
int Arma::calculeazaDauneEfective() const { return bonusDaune; }

// Implementare functie pur virtuala din clasa de baza.
void Sabie::descriere_impl() const { std::cout << "Sabie Greoaie"; }

// Constructor virtual (clone). Returneaza un pointer la noul obiect alocat
Arma* Sabie::clone() const { return new Sabie(*this); }

// Functie virtuala specifica clasei. Suprascrie logica de damage adaugand Critical Hit
int Sabie::calculeazaDauneEfective() const {
    if (rand() % 100 < 20) { std::cout << " [CRITIC!] "; return bonusDaune + 5; }
    return bonusDaune;
}

// Implementare functie virtuala pura
void Lance::descriere_impl() const { std::cout << "Lance Lunga"; }

//  Constructor virtual (clone).
Arma* Lance::clone() const { return new Lance(*this); }

// Implementare functie virtuala pura
void Toiag::descriere_impl() const { std::cout << "Toiag Magic"; }

// Constructor virtual (clone)
Arma* Toiag::clone() const { return new Toiag(*this); }

// Functie virtuala specifica temei. Adauga fluctuatie random la daune folosind rand
int Toiag::calculeazaDauneEfective() const { return bonusDaune + (rand() % 7 - 3); }

// Implementare functie pur virtuala
void Arc::descriere_impl() const { std::cout << "Arc Elfesc"; }

// Constructor virtual (clone)
Arma* Arc::clone() const { return new Arc(*this); }

// Functie virtuala specifica clasei, adauga mecanica de Headshot
int Arc::calculeazaDauneEfective() const {
    if (rand() % 100 < 10) { std::cout << " [HEADSHOT!] "; return bonusDaune + 10; }
    return bonusDaune;
}

// Constructor Potiune, unde avem si Throw in constructor
Potiune::Potiune(int v) : valoareEfect(v) {
    if (v <= 0) throw ConfigurareInvalida("Efect invalid!");
}

// Getter simplu pentru valoarea potiunii.
int Potiune::getValoare() const { return valoareEfect; }

// Constructor virtual (clone)
Potiune* PotiuneRegenerare::clone() const { return new PotiuneRegenerare(*this); }

// Implementare functie pur virtuala (specifice temei)
void PotiuneRegenerare::aplicaEfect(Erou* e) const { e->primesteVindecare(valoareEfect); }

// Constructor virtual (clone)
Potiune* PotiuneScut::clone() const { return new PotiuneScut(*this); }

// Implementare functie pur virtuala (specifice temei)
void PotiuneScut::aplicaEfect(Erou* e) const { e->incaseazaDaune(-valoareEfect); }
