#include <iostream>
#include "ManagerJoc.h"

// Functia main
int main() {
    // Utilizare try/catch direct in main pentru a prinde orice derivata a std::exception
    try {
        ManagerJoc joc;
        joc.ruleazaJocul();
        
        // Apelarea si testarea a metodei statice
        std::cout << "\nEntitati ramase (Contorizare statica): " << Entitate::obtineNumarEntitati() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "CRASH: " << e.what() << "\n";
    }
    return 0;
}
