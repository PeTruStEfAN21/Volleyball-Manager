#pragma once
#include <vector>
#include <string>
#include <memory>
#include "jucator.h"
#include "Echipe.h"
#include "Gestiune.h"

class Pachet {
protected:
    std::string nume;
    int cost;
public:
    Pachet(std::string n, int c);
    virtual ~Pachet() = default;

    int getCost() const;
    std::string getNume() const;

    virtual jucatorptr deschide(const std::vector<jucatorptr>& baza) = 0;
    jucatorptr extrageDupaOvr(const std::vector<jucatorptr>& lista, int minOvr, int maxOvr);
};

class PachetBronze : public Pachet {
public:
    PachetBronze();
    jucatorptr deschide(const std::vector<jucatorptr>& baza) override;
};

class PachetGold : public Pachet {
public:
    PachetGold();
    jucatorptr deschide(const std::vector<jucatorptr>& baza) override;
};