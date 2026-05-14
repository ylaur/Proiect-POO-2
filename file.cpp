#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>
#include <limits>
#include <exception>

class JocError : public std::exception {
protected:
    std::string mesaj;
public:
    explicit JocError(const std::string& m) : mesaj(">>> EROARE JOC: " + m) {}
    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class InputInvalid : public JocError {
public:
    explicit InputInvalid(const std::string& m) : JocError("Input invalid! " + m) {}
};

class ResurseInsuficiente : public JocError {
public:
    explicit ResurseInsuficiente(const std::string& m) : JocError("Puncte insuficiente pentru actiunea: " + m) {}
};

class ConfigurareInvalida : public JocError {
public:
    explicit ConfigurareInvalida(const std::string& m) : JocError("Configurare fatala: " + m) {}
};

class Arma {
protected:
    int bonusDaune;
    virtual void descriere_impl() const = 0;

public:
    explicit Arma(int bonus) : bonusDaune(bonus) {
        if (bonus < 0) {
            throw ConfigurareInvalida("O arma nu poate avea bonus de daune negativ!");
        }
    }

    virtual ~Arma() = default;
    virtual Arma* clone() const = 0;

    void afiseazaArma() const {
        descriere_impl();
        std::cout << " (+ " << bonusDaune << " Daune)";
    }

    int getBonusDaune() const { return bonusDaune; }
    void adaugaBonus(int valoare) { bonusDaune += valoare; }

    virtual int calculeazaDauneEfective() const {
        return bonusDaune;
    }
};

class Sabie : public Arma {
protected:
    void descriere_impl() const override { std::cout << "Sabie Zabavnica"; }
public:
    explicit Sabie(int b = 15) : Arma(b) {}
    Arma* clone() const override { return new Sabie(*this); }

    int calculeazaDauneEfective() const override {
        int dmg = bonusDaune;
        if (rand() % 100 < 20) {
            std::cout << " [CRITICAL HIT CU SABIA!] ";
            dmg += 5;
        }
        return dmg;
    }
};

class Lance : public Arma {
protected:
    void descriere_impl() const override { std::cout << "Lance Lunga de Strapungere"; }
public:
    explicit Lance(int b = 18) : Arma(b) {}
    Arma* clone() const override { return new Lance(*this); }

    int calculeazaDauneEfective() const override {
        return bonusDaune;
    }
};

class Toiag : public Arma {
protected:
    void descriere_impl() const override { std::cout << "Toiag Magic Arcanic"; }
public:
    explicit Toiag(int b = 20) : Arma(b) {}
    Arma* clone() const override { return new Toiag(*this); }

    int calculeazaDauneEfective() const override {
        int fluctuatie = (rand() % 7) - 3;
        int dmg = bonusDaune + fluctuatie;
        return (dmg > 0) ? dmg : 1;
    }
};

class Arc : public Arma {
protected:
    void descriere_impl() const override { std::cout << "Arc de Precizie Elfesc"; }
public:
    explicit Arc(int b = 17) : Arma(b) {}
    Arma* clone() const override { return new Arc(*this); }

    int calculeazaDauneEfective() const override {
        int dmg = bonusDaune;
        if (rand() % 100 < 10) {
            std::cout << " [HEADSHOT!] ";
            dmg += 10;
        }
        return dmg;
    }
};

class Potiune {
protected:
    int valoareEfect;
public:
    explicit Potiune(int v) : valoareEfect(v) {
        if (v <= 0) {
            throw ConfigurareInvalida("Potiunea trebuie sa aiba un efect strict pozitiv.");
        }
    }

    virtual ~Potiune() = default;
    virtual Potiune* clone() const = 0;
    virtual void aplicaEfect(class Erou* erouTarget) const = 0;

    int getValoare() const { return valoareEfect; }
};

class PotiuneRegenerare : public Potiune {
public:
    explicit PotiuneRegenerare(int v = 40) : Potiune(v) {}
    Potiune* clone() const override { return new PotiuneRegenerare(*this); }
    void aplicaEfect(class Erou* erouTarget) const override;
};

class PotiuneScut : public Potiune {
public:
    explicit PotiuneScut(int v = 25) : Potiune(v) {}
    Potiune* clone() const override { return new PotiuneScut(*this); }
    void aplicaEfect(class Erou* erouTarget) const override;
};

class Entitate {
protected:
    std::string nume;
    int hpCurent;
    int hpMaxim;
    int dauneBaza;

    static int contorEntitati;

    virtual void afisare_identitate() const = 0;

public:
    Entitate(std::string n, int h, int d) : nume(n), hpCurent(h), hpMaxim(h), dauneBaza(d) {
        if (h <= 0 || d < 0) {
            throw ConfigurareInvalida("Statusuri invalide la crearea entitatii " + n + ".");
        }
        contorEntitati++;
    }

    virtual ~Entitate() {
        contorEntitati--;
    }

    bool esteViu() const { return hpCurent > 0; }
    std::string getNume() const { return nume; }
    int getHP() const { return hpCurent; }
    int getHPMax() const { return hpMaxim; }
    int getDauneBaza() const { return dauneBaza; }

    virtual void incaseazaDaune(int daune) {
        hpCurent -= daune;
        if (hpCurent < 0) hpCurent = 0;
    }

    void primesteVindecare(int vindecare) {
        if (!esteViu()) return;
        hpCurent += vindecare;
        if (hpCurent > hpMaxim) hpCurent = hpMaxim;
    }

    void afiseazaStatusCard() const {
        std::cout << "[";
        afisare_identitate();
        std::cout << "] " << nume << " - HP: " << hpCurent << "/" << hpMaxim;
    }

    static int obtineNumarEntitati() {
        return contorEntitati;
    }
};

int Entitate::contorEntitati = 0;

class Erou : public Entitate {
protected:
    Arma* armaEchipata;

public:
    Erou(std::string n, int h, int d) : Entitate(n, h, d), armaEchipata(nullptr) {}

    virtual ~Erou() {
        delete armaEchipata;
    }

    Erou(const Erou& other) : Entitate(other) {
        if (other.armaEchipata != nullptr) {
            this->armaEchipata = other.armaEchipata->clone();
        } else {
            this->armaEchipata = nullptr;
        }
    }

    Erou& operator=(const Erou& other) {
        if (this != &other) {
            this->nume = other.nume;
            this->hpCurent = other.hpCurent;
            this->hpMaxim = other.hpMaxim;
            this->dauneBaza = other.dauneBaza;
            delete this->armaEchipata;
            this->armaEchipata = other.armaEchipata ? other.armaEchipata->clone() : nullptr;
        }
        return *this;
    }

    void atribuieArma(const Arma& nouaArma) {
        delete armaEchipata;
        armaEchipata = nouaArma.clone();
    }

    Arma* getArma() const {
        return armaEchipata;
    }

    int calculeazaFortaTotala() const {
        int forta = dauneBaza;
        if (armaEchipata != nullptr) {
            forta += armaEchipata->calculeazaDauneEfective();
        }
        return forta;
    }

    void afisareCompleta() const {
        afiseazaStatusCard();
        std::cout << " | ";
        if (armaEchipata != nullptr) {
            armaEchipata->afiseazaArma();
        } else {
            std::cout << "FARA ARMA";
        }
        std::cout << " | Daune potentiale: " << calculeazaFortaTotala() << std::endl;
    }
};

void PotiuneRegenerare::aplicaEfect(Erou* erouTarget) const {
    std::cout << " * Potiunea de regenerare vindeca " << valoareEfect << " puncte de viata!\n";
    erouTarget->primesteVindecare(valoareEfect);
}

void PotiuneScut::aplicaEfect(Erou* erouTarget) const {
    std::cout << " * Potiunea ofera " << valoareEfect << " viata bonus deasupra limitelor normale!\n";
    int hpVechi = erouTarget->getHP();
    erouTarget->primesteVindecare(valoareEfect);
    if(erouTarget->getHP() == hpVechi) {
        erouTarget->incaseazaDaune(-valoareEfect);
    }
}

class Cavaler : public Erou {
protected:
    void afisare_identitate() const override { std::cout << "Cavaler"; }
public:
    explicit Cavaler(std::string n) : Erou(n, 150, 15) {
        atribuieArma(Sabie(10));
    }
};

class Print : public Erou {
protected:
    void afisare_identitate() const override { std::cout << "Print"; }
public:
    explicit Print(std::string n) : Erou(n, 120, 20) {
        atribuieArma(Lance(15));
    }
};

class Arcas : public Erou {
protected:
    void afisare_identitate() const override { std::cout << "Arcas"; }
public:
    explicit Arcas(std::string n) : Erou(n, 100, 25) {
        atribuieArma(Arc(18));
    }
};

class Magician : public Erou {
protected:
    void afisare_identitate() const override { std::cout << "Magician"; }
public:
    explicit Magician(std::string n) : Erou(n, 80, 30) {
        atribuieArma(Toiag(22));
    }
};

class Inamic : public Entitate {
protected:
    void afisare_identitate() const override { std::cout << "Inamic"; }
public:
    Inamic(std::string n, int h, int d) : Entitate(n, h, d) {}
};

class ManagerJoc {
private:
    std::vector<Erou*> echipaEroi;
    std::vector<Inamic*> hoardaInamici;
    int puncteJucator;
    int rundaCurenta;

    const std::vector<std::string> numeInamici = {
        "Demonul Umbrelor", "Regele Nimicirii", "Goblinul Furios",
        "Bestia de Piatra", "Spectrul Noptii", "Lupul Intunecat",
        "Orcul Sangvin", "Fantriomul Ascuns"
    };

    int citesteInt() {
        int val;
        while (!(std::cin >> val)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << ">>> Eroare Input: Te rugam sa introduci un numar intreg: ";
        }
        return val;
    }

    void afisareSeparatoare() const {
        std::cout << "\n========================================================\n";
    }

    void afisareMiniSeparator() const {
        std::cout << "--------------------------------------------------------\n";
    }

    void curataEcran() const {
        for(int i = 0; i < 2; ++i) std::cout << std::endl;
    }

public:
    ManagerJoc() : puncteJucator(0), rundaCurenta(1) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    ~ManagerJoc() {
        for (auto e : echipaEroi) delete e;
        for (auto i : hoardaInamici) delete i;
    }

    ManagerJoc(const ManagerJoc&) = delete;
    ManagerJoc& operator=(const ManagerJoc&) = delete;

    void fazaSelectieEroi() {
        afisareSeparatoare();
        std::cout << "          FAZA 1: SELECTIA EROILOR TAI\n";
        std::cout << "Alege 2 campioni care vor lupta in prima linie.\n";
        std::cout << "1. Cavaler  (Tanc pur, HP mare, daune mici)\n";
        std::cout << "2. Print    (Echilibrat, specializat in lupta frontala)\n";
        std::cout << "3. Arcas    (Daune la distanta ridicate, HP vulnerabil)\n";
        std::cout << "4. Magician (Damage extrem, cel mai scazut HP)\n";

        for (int i = 1; i <= 2; ++i) {
            std::cout << "\n-> Alege clasa pentru eroul " << i << " (tasteaza o cifra intre 1-4): ";
            int clasaOpt = citesteInt();

            std::string numeAles;
            std::cout << "-> Introdu un nume legendar pentru eroul " << i << ": ";
            std::cin >> numeAles;

            switch (clasaOpt) {
                case 1: echipaEroi.push_back(new Cavaler(numeAles)); break;
                case 2: echipaEroi.push_back(new Print(numeAles)); break;
                case 3: echipaEroi.push_back(new Arcas(numeAles)); break;
                case 4: echipaEroi.push_back(new Magician(numeAles)); break;
                default:
                    std::cout << ">>> Optiune de clasa invalida! S-a atribuit eroul: Cavaler.\n";
                    echipaEroi.push_back(new Cavaler(numeAles));
                    break;
            }
        }
        std::cout << "\nEchipa a fost fortificata si este gata de lupta!\n";
    }

    void fazaGenerareInamici() {
        int bazaHP = 0;
        for(auto e : echipaEroi) {
            bazaHP += e->getHPMax();
        }
        bazaHP = bazaHP / 3;

        for (int i = 0; i < 3; ++i) {
            std::string numeRand = numeInamici[std::rand() % numeInamici.size()];
            int hp = bazaHP + (std::rand() % 40) + (rundaCurenta * 5);
            int dmg = 10 + (std::rand() % 10) + (rundaCurenta * 3);

            hoardaInamici.push_back(new Inamic(numeRand, hp, dmg));
        }
        std::cout << "\nO hoarda de 3 inamici brutali se indreapta spre echipa ta!\n";
    }

    void fazaMagazin() {
        bool inMagazin = true;
        while (inMagazin) {
            curataEcran();
            afisareSeparatoare();
            std::cout << "          MAGAZINUL TIMPULUI (Sfarsitul Rundei " << (rundaCurenta - 1) << ")\n";
            std::cout << "          Fonduri disponibile: " << puncteJucator << " Puncte Aur\n";
            afisareMiniSeparator();
            std::cout << "1. Ascute Arma (+10 Daune Baza permanent)   | Cost: 50 Puncte\n";
            std::cout << "2. Bea Potiune de Regenerare (+40 HP)       | Cost: 30 Puncte\n";
            std::cout << "3. Baza Potiune de Scut (+25 HP Temporar)   | Cost: 40 Puncte\n";
            std::cout << "0. Iesire din magazin si intoarcere in lupta\n";
            std::cout << "-> Alege o actiune: ";

            int opt = citesteInt();

            if (opt == 0) {
                inMagazin = false;
                std::cout << "Ai parasit magazinul.\n";
                continue;
            }

            if (opt < 1 || opt > 3) {
                std::cout << ">>> Optiune invalida in magazin.\n";
                continue;
            }

            std::cout << "\nPentru care erou aplici obiectul?\n";
            for (size_t i = 0; i < echipaEroi.size(); ++i) {
                std::cout << i << ". " << echipaEroi[i]->getNume() << " (HP: " << echipaEroi[i]->getHP() << ")\n";
            }
            std::cout << "-> Introduce ID-ul eroului: ";
            int eIdx = citesteInt();

            if (eIdx < 0 || eIdx >= (int)echipaEroi.size()) {
                std::cout << ">>> Erou inexistent! Tranzactie abandonata.\n";
                continue;
            }

            Erou* erouSelectat = echipaEroi[eIdx];

            try {
                if (opt == 1) {
                    if (puncteJucator < 50) {
                        throw ResurseInsuficiente("Upgrade Arma (Ai " + std::to_string(puncteJucator) + ")");
                    }
                    puncteJucator -= 50;
                    if (erouSelectat->getArma()) {
                        erouSelectat->getArma()->adaugaBonus(10);
                        std::cout << ">>> Succes! Arma lui " << erouSelectat->getNume() << " a primit un upgrade mortal!\n";
                    }
                    else {
                        std::cout << ">>> Eroul nu are arma de imbunatatit!\n";
                    }
                }
                else if (opt == 2) {
                    if (puncteJucator < 30) {
                        throw ResurseInsuficiente("Potiune Regen (Ai " + std::to_string(puncteJucator) + ")");
                    }
                    puncteJucator -= 30;
                    PotiuneRegenerare p;
                    p.aplicaEfect(erouSelectat);
                }
                else if (opt == 3) {
                    if (puncteJucator < 40) {
                        throw ResurseInsuficiente("Potiune Scut (Ai " + std::to_string(puncteJucator) + ")");
                    }
                    puncteJucator -= 40;
                    PotiuneScut p;
                    p.aplicaEfect(erouSelectat);
                }
            }
            catch (const JocError& e) {
                std::cout << e.what() << "\n";
            }

            std::cout << "\n[Apasati ENTER pentru a continua]";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }

    void afiseazaStareCurentaLupta() const {
        std::cout << "\n[ FORMATIA TA DE LUPTA ]\n";
        for (size_t i = 0; i < echipaEroi.size(); ++i) {
            std::cout << " ID: " << i << " | ";
            echipaEroi[i]->afisareCompleta();
        }

        std::cout << "\n[ FORTELE INAMICE ]\n";
        for (size_t i = 0; i < hoardaInamici.size(); ++i) {
            if (hoardaInamici[i]->esteViu()) {
                std::cout << " ID: " << i << " | ";
                hoardaInamici[i]->afiseazaStatusCard();
                std::cout << " | Capacitate Distrugere: " << hoardaInamici[i]->getDauneBaza() << "\n";
            }
        }
    }

    void proceseazaAbilitatiPasiveDeClasa() {
        bool pasivaActivata = false;
        std::cout << "\n--- Efecte de Final de Runda ---\n";

        for (auto e : echipaEroi) {
            if (!e->esteViu()) continue;

            if (Magician* m = dynamic_cast<Magician*>(e)) {
                std::cout << " * [PASIVA DE CLASA] Meditatia Magicianului " << m->getNume() << " recupereaza 8 HP.\n";
                m->primesteVindecare(8);
                pasivaActivata = true;
            }
            else if (Print* p = dynamic_cast<Print*>(e)) {
                std::cout << " * [PASIVA DE CLASA] Prezenta Regala a Printului " << p->getNume() << " aduce 5 Puncte Aur in vistierie.\n";
                puncteJucator += 5;
                pasivaActivata = true;
            }
        }

        if (!pasivaActivata) {
            std::cout << " * Nicio abilitate pasiva nu a fost declansata.\n";
        }
    }

    void executaTurulJucatorului() {
        std::cout << "\n>>> ESTE RANDUL TAU SA LOVESTI! <<<\n";
        std::cout << "-> Tasteaza ID-ul Eroului cu care vrei sa ataci: ";
        int atkIdx = citesteInt();

        std::cout << "-> Tasteaza ID-ul Inamicului pe care vrei sa il distrugi: ";
        int defIdx = citesteInt();

        if (atkIdx >= 0 && atkIdx < (int)echipaEroi.size() && defIdx >= 0 && defIdx < (int)hoardaInamici.size()) {
            Erou* erouAtacator = echipaEroi[atkIdx];
            Inamic* inamicTinta = hoardaInamici[defIdx];

            if (erouAtacator->esteViu() && inamicTinta->esteViu()) {
                int dauneDistructive = erouAtacator->calculeazaFortaTotala();

                std::cout << "\n!!! " << erouAtacator->getNume() << " s-a lansat asupra lui " << inamicTinta->getNume() << " !!!\n";
                inamicTinta->incaseazaDaune(dauneDistructive);
                std::cout << "!!! Impactul a cauzat " << dauneDistructive << " daune!\n";

                if (!inamicTinta->esteViu()) {
                    std::cout << ">>> INAMIC ELIMINAT! Castigi prada de runda: +45 Puncte.\n";
                    puncteJucator += 45;
                }
            }
            else {
                std::cout << "\n>>> Actiune blocata: Unul dintre luptatorii selectati este deja mort.\n";
            }
        }
        else {
            std::cout << "\n>>> Comanda de atac a esuat din cauza indicilor gresiti! Sansa ta a fost irosita in aceasta runda.\n";
        }
    }

    void executaTurulInamicilor() {
        std::cout << "\n>>> INAMICII RETALIAZA... <<<\n";

        for (auto inamic : hoardaInamici) {
            if (inamic->esteViu()) {
                std::vector<Erou*> tinteDisponibile;
                for (auto e : echipaEroi) {
                    if (e->esteViu()) tinteDisponibile.push_back(e);
                }

                if (!tinteDisponibile.empty()) {
                    Erou* victima = tinteDisponibile[std::rand() % tinteDisponibile.size()];

                    std::cout << " !!! " << inamic->getNume() << " sfasie defensiva lui " << victima->getNume() << "!\n";
                    victima->incaseazaDaune(inamic->getDauneBaza());
                    std::cout << " !!! Eroul tau a incasat " << inamic->getDauneBaza() << " daune!\n";
                }
            }
        }
    }

    void fazaLupta() {
        afisareSeparatoare();
        std::cout << "                 RUNDA DE COMBAT " << rundaCurenta << "\n";

        afiseazaStareCurentaLupta();

        executaTurulJucatorului();
        executaTurulInamicilor();

        proceseazaAbilitatiPasiveDeClasa();

        puncteJucator += 20;
        rundaCurenta++;
    }

    void ruleazaJocul() {
        fazaSelectieEroi();
        fazaGenerareInamici();

        bool luptaInDesfasurare = true;
        while (luptaInDesfasurare) {

            bool eroiInPicioare = false;
            for (auto e : echipaEroi) {
                if (e->esteViu()) eroiInPicioare = true;
            }
            if (!eroiInPicioare) {
                afisareSeparatoare();
                std::cout << "                    GAME OVER\n";
                std::cout << "   Legenda s-a sfarsit tragic in runda " << rundaCurenta << ".\n";
                break;
            }

            bool inamiciRamas = false;
            for (auto i : hoardaInamici) {
                if (i->esteViu()) inamiciRamas = true;
            }
            if (!inamiciRamas) {
                afisareSeparatoare();
                std::cout << "                 VICTORIE EPICA!\n";
                std::cout << "   Ai salvat regatul si ai adunat in total " << puncteJucator << " puncte!\n";
                break;
            }

            if ((rundaCurenta - 1) > 0 && (rundaCurenta - 1) % 3 == 0) {
                fazaMagazin();
            }

            fazaLupta();

            std::cout << "\n[Apasati tasta ENTER pentru a trece in urmatoarea faza...]";
            std::cin.ignore(10000, '\n');
            std::cin.get();
        }
    }
};

int main() {
    try {
        std::cout << "========================================================\n";
        std::cout << "          Bine ai venit la \"Eroi vs inamici\"\n";
        std::cout << "========================================================\n";

        ManagerJoc rpgEngine;
        rpgEngine.ruleazaJocul();

        std::cout << "\n[MONITOR MEMORIE STATICA] Entitati active ramase in sistem: "
                  << Entitate::obtineNumarEntitati() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "\n>>> FATAL CRASH: " << e.what() << "\n";
    } catch (...) {
        std::cerr << "\n>>> FATAL CRASH: Tip de exceptie neidentificat!\n";
    }

    return 0;
}
