Tema: Joc Video
Acesta este un proiect dezvoltat in C++ care simuleaza un joc de rol (RPG) in consola. Actiunea se desfasoara in runde, in care o echipa de eroi se confrunta cu o hoarda de inamici generati aleator. Jucatorii pot echipa arme specifice, pot cumpara si folosi potiuni pentru a se vindeca sau a dobandi scuturi, incercand sa supravietuiasca unui numar cat mai mare de atacuri.

Structura Proiectului
Codul este modularizat pentru a separa clar declaratiile de implementarile propriu-zise:

Exceptii.h: Contine ierarhia de erori, derivata din std::exception.

Iteme.h / Iteme.cpp: Implementeaza ierarhiile pentru arme (care aduc bonusuri si sanse de lovituri speciale) si potiuni.

Personaje.h / Personaje.cpp: Defineste baza jocului: Entitate, Erou, tipurile specifice de eroi (Cavaler, Print, Arcas, Magician) si Inamicii.

ManagerJoc.h / ManagerJoc.cpp: Motorul jocului care gestioneaza selectia, lupta, sistemul de magazin si punctajul.

main.cpp: Punctul de intrare in aplicatie, incluzand un bloc try-catch global.

Concepte OOP Implementate
Mostenire: Ierarhie pe 3 niveluri (Entitate -> Erou -> clase concrete). Baze independente, nederivate din clase predefinite.

Polimorfism: Functii virtuale pure utilizate pentru comportamente diferite la runtime (ex: sanse de atac critic pentru sabie).

Constructori virtuali: Utilizarea metodei clone() pentru copieri profunde (deep-copy) sigure.

Interfata Non-Virtuala: Separarea logicii interne de afisarea publica a informatiilor (ex: afisarea armei sau statusului).

Regula celor 3: Destructor, constructor de copiere si operator de atribuire suprascrise in clasa Erou pentru o gestionare corecta a resurselor.

Compozitie: Eroul detine un pointer la clasa de baza Arma, permitand schimbarea sau preluarea diferitelor arme in mod dinamic.

Downcasting: Utilizarea dynamic_cast pentru a activa pasive specifice (precum regenerarea Magicianului) in anumite momente ale luptei.

Exceptii custom: Ierarhie de exceptii folosita pentru validarea datelor (constructori) sau pentru verificarea resurselor la magazin.

Membri statici: Un contor global pentru monitorizarea entitatilor active si prevenirea memory leak-urilor.

STL si Const: Folosirea corecta a bibliotecilor standard (vector, string, random) si aplicarea stricta a specificatorului const pentru metode si parametri unde este cazul.
