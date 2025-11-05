#pragma once

#include <vector>
#include <iostream>
#include <memory>

class jucator;
class Echipe; 

using jucatorptr = std::shared_ptr<jucator>;
using Echipeptr = std::shared_ptr<Echipe>;

class BazaDeDate {
private:
    std::vector<jucatorptr> jucatori;
    std::vector<Echipeptr> echipe_disponibile;

public:
    void eliminaEchipa(Echipeptr e);
    explicit BazaDeDate(const std::vector<jucatorptr> &jucatori);
    BazaDeDate();

    void afisare_jucatori_necontractati();

    ~BazaDeDate();

    void adaugaJucator(jucatorptr j);
    void adaugaEchipe(Echipeptr e);

    jucatorptr getJucator(int index) const;

    void afiseazaJucatori() const;

    Echipeptr getEchipe(size_t index) const;


    void afiseazaEchipe() const;

    const std::vector<jucatorptr>& getLista() const;

    const std::vector<Echipeptr>& getListe() const;

    Echipeptr alege_echipa_random();

    int alege_echipa_random_index();

    BazaDeDate(const BazaDeDate &other);

    BazaDeDate & operator=(const BazaDeDate &other);
};