#include "ManagerJoc.h"
#include <iostream>
#include <limits>
#include <ctime>

// Constructor ManagerJoc: initiaza random seed-ul si lista de nume posibile
ManagerJoc::ManagerJoc() : puncteJucator(0), rundaCurenta(1), 
numeInamici({"Umbra", "Rege", "Goblin", "Bestie", "Spectru", "Lup", "Orc"}) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// Destructor ManagerJoc: dezaloca toata memoria dinamica a eroilor si inamicilor
ManagerJoc::~ManagerJoc() {
    for (auto e : echipaEroi) delete e;
    for (auto i : hoardaInamici) delete i;
}

// Previne crash-ul programului la introducere de caractere in loc de numere, maxim 10000
int ManagerJoc::citesteInt() {
    int v;
    while (!(std::cin >> v)) {
      std::cin.clear();
      std::cin.ignore(10000, '\n');
      std::cout << "Introdu o cifra: "; }
    return v;
}

// Faza 1: Instanteaza 2 Eroi in functie de deciziile jucatorului
void ManagerJoc::fazaSelectieEroi() {
    for (int i = 1; i <= 2; ++i) {
        std::cout << "Alege erou " << i << " (1.Cav, 2.Print, 3.Arc, 4.Mag): ";
        int opt = citesteInt();
        std::string n; std::cout << "Nume: "; std::cin >> n;
        if (opt == 1) echipaEroi.push_back(new Cavaler(n));
        else if (opt == 2) echipaEroi.push_back(new Print(n));
        else if (opt == 3) echipaEroi.push_back(new Arcas(n));
        else echipaEroi.push_back(new Magician(n));
    }
}

// Faza 2: Adauga la vectorul de inamici generand statistici adaptate (dupa eroii alesi de User)
void ManagerJoc::fazaGenerareInamici() {
    for (int i = 0; i < 3; ++i) {
        hoardaInamici.push_back(new Inamic(numeInamici[rand() % numeInamici.size()], 50 + rundaCurenta * 5, 10 + rundaCurenta * 2));
    }
}

// Utilizare throw / try / catch pentru verificarea banilor si logica magazinului
void ManagerJoc::fazaMagazin() {
    std::cout << "\n=== MAGAZIN (Puncte: " << puncteJucator << ") ===\n";
    std::cout << "1.Upgrade Arma(50p) 2.Regen(30p) 3.Scut(40p) 0.Iesire\n";
    int opt = citesteInt();
    if (opt == 0) return;
    std::cout << "ID Erou (0/1): "; int id = citesteInt();
    if (id < 0 || id > 1) return;
    
    try {
        if (opt == 1 && puncteJucator >= 50) { echipaEroi[id]->getArma()->adaugaBonus(10); puncteJucator -= 50; }
        else if (opt == 2 && puncteJucator >= 30) { PotiuneRegenerare(40).aplicaEfect(echipaEroi[id]); puncteJucator -= 30; }
        else if (opt == 3 && puncteJucator >= 40) { PotiuneScut(25).aplicaEfect(echipaEroi[id]); puncteJucator -= 40; }
        else throw ResurseInsuficiente("Scor mic!"); // Throw pentru clasa de exceptii
    } catch (const JocError& e) { std::cout << e.what() << "\n"; }
}

// dynamic_cast pentru downcast; Executam o actiune specifica STRICT daca Eroul este un Magician
void ManagerJoc::proceseazaAbilitatiPasiveDeClasa() {
    for (auto e : echipaEroi) {
        if (e->esteViu()) {
            if (Magician* m = dynamic_cast<Magician*>(e)) { 
                m->primesteVindecare(8); std::cout << "Magie pasiva (Heal)!\n"; 
            }
        }
    }
}

// Preia logica atacului manual al jucatorului dintr-un array in altul
void ManagerJoc::executaTurulJucatorului() {
    std::cout << "ID Erou (0-1) si ID Inamic (0-2): ";
    int e = citesteInt(), i = citesteInt();
    if (e < 0 || e > 1 || i < 0 || i > 2 || !echipaEroi[e]->esteViu() || !hoardaInamici[i]->esteViu()) return;
    int dmg = echipaEroi[e]->calculeazaFortaTotala();
    hoardaInamici[i]->incaseazaDaune(dmg);
    if (!hoardaInamici[i]->esteViu()) puncteJucator += 45;
}

// Parcurge inamicii vii si aplica damage pe un erou valid ales random
void ManagerJoc::executaTurulInamicilor() {
    for (auto in : hoardaInamici) {
        if (in->esteViu()) {
            for (auto er : echipaEroi) if (er->esteViu()) { er->incaseazaDaune(in->getDauneBaza()); break; }
        }
    }
}

// Metoda care executa ordinea unei runde
void ManagerJoc::fazaLupta() {
    afiseazaStareCurentaLupta();
    executaTurulJucatorului();
    executaTurulInamicilor();
    proceseazaAbilitatiPasiveDeClasa();
    puncteJucator += 20; rundaCurenta++;
}

// Printeaza in consola statusul echipelor din vectori
void ManagerJoc::afiseazaStareCurentaLupta() const {
    for (auto e : echipaEroi) e->afisareCompleta();
    for (auto i : hoardaInamici) if (i->esteViu()) { i->afiseazaStatusCard(); std::cout << "\n"; }
}

// Loop-ul principal care verifica victoriile/infrangerile si magazinul (Runda % 3 == 0)
void ManagerJoc::ruleazaJocul() {
    fazaSelectieEroi(); fazaGenerareInamici();
    while (true) {
        bool vE = false, vI = false;
        for (auto e : echipaEroi) if (e->esteViu()) vE = true;
        for (auto i : hoardaInamici) if (i->esteViu()) vI = true;
        if (!vE || !vI) break;
        if ((rundaCurenta - 1) % 3 == 0) fazaMagazin();
        fazaLupta();
        std::cout << "\n[ENTER]"; std::cin.ignore(10000, '\n'); std::cin.get();
    }
}
