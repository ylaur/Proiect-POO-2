#pragma once
#include "Exceptii.h"
#include <iostream>

class Erou; 

class Arma {
protected:
    int bonusDaune;
    virtual void descriere_impl() const = 0;
public:
    explicit Arma(int bonus);
    virtual ~Arma() = default;
    virtual Arma* clone() const = 0;
    void afiseazaArma() const;
    int getBonusDaune() const;
    void adaugaBonus(int valoare);
    virtual int calculeazaDauneEfective() const;
};

class Sabie : public Arma {
protected:
    void descriere_impl() const override;
public:
    using Arma::Arma;
    Arma* clone() const override;
    int calculeazaDauneEfective() const override;
};

class Lance : public Arma {
protected:
    void descriere_impl() const override;
public:
    using Arma::Arma;
    Arma* clone() const override;
};

class Toiag : public Arma {
protected:
    void descriere_impl() const override;
public:
    using Arma::Arma;
    Arma* clone() const override;
    int calculeazaDauneEfective() const override;
};

class Arc : public Arma {
protected:
    void descriere_impl() const override;
public:
    using Arma::Arma;
    Arma* clone() const override;
    int calculeazaDauneEfective() const override;
};

class Potiune {
protected:
    int valoareEfect;
public:
    explicit Potiune(int v);
    virtual ~Potiune() = default;
    virtual Potiune* clone() const = 0;
    virtual void aplicaEfect(Erou* erouTarget) const = 0;
    int getValoare() const;
};

class PotiuneRegenerare : public Potiune {
public:
    using Potiune::Potiune;
    Potiune* clone() const override;
    void aplicaEfect(Erou* erouTarget) const override;
};

class PotiuneScut : public Potiune {
public:
    using Potiune::Potiune;
    Potiune* clone() const override;
    void aplicaEfect(Erou* erouTarget) const override;
};
