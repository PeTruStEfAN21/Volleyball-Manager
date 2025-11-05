#pragma once

#include <vector>
#include "Meci.h"

class BazaDeDate;
class Echipe;
class jucator;
class manageri;

using BazaDeDateptr = std::shared_ptr<BazaDeDate>;
using jucatorptr = std::shared_ptr<jucator>;
using Echipeptr = std::shared_ptr<Echipe>;

class Liga : public Meci {
private:
    BazaDeDateptr baza;
    std::vector<int> punctaje;
    int nrEchipe = 0;
    std::vector<std::vector<bool>> ales;
    std::vector<bool> etapa_jucat;
    std::vector<Echipeptr> echipe;

public:
    void reset_etapa();

    Liga(BazaDeDateptr baza);

    Liga();

    ~Liga();

    void creare_liga() ;

    Echipeptr meci();
    void afisare_punctaje();
    void meciuri();
    int liga();
    void adaugare_echipa(Echipeptr echipa);
};