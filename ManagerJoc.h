#pragma once
#include <vector>
#include "Personaje.h"

class ManagerJoc {
private:
    std::vector<Erou*> echipaEroi;
    std::vector<Inamic*> hoardaInamici;
    int puncteJucator, rundaCurenta;
    const std::vector<std::string> numeInamici;

    int citesteInt();
    void fazaSelectieEroi();
    void fazaGenerareInamici();
    void fazaMagazin();
    void afiseazaStareCurentaLupta() const;
    void proceseazaAbilitatiPasiveDeClasa();
    void executaTurulJucatorului();
    void executaTurulInamicilor();
    void fazaLupta();
public:
    ManagerJoc();
    ~ManagerJoc();
    ManagerJoc(const ManagerJoc&) = delete;
    ManagerJoc& operator=(const ManagerJoc&) = delete;
    void ruleazaJocul();
};
