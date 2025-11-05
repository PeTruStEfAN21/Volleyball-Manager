#pragma once

#include <string>
#include <vector>
#include <memory>



class Transferuri;
class Echipe;
class BazaDeDate;
class jucator;

using BazaDeDateptr = std::shared_ptr<BazaDeDate>;
using jucatorptr = std::shared_ptr<jucator>;
using Echipeptr = std::shared_ptr<Echipe>;
using Transferuriptr = std::shared_ptr<Transferuri>;

class manageri {
private:
    Echipeptr echipa;
    std::string nume;
    std::vector<jucatorptr> lista;
    BazaDeDateptr baza;
    Transferuriptr transferuri_manager;

public:
    manageri(int buget, Echipeptr echipa, const std::string &nume);
    manageri();

    int citire_jucatori_liberi();

    int citire_baza_manager();

    BazaDeDateptr get_baza_manager();

    int get_buget() const;

    int citire_toti_jucatorii_si_echipe();

    void set_buget(int buget);
    
    manageri(const manageri& other);
    
    manageri& operator=(const manageri& other);

    ~manageri();

    Echipeptr get_echipa() const;
    void set_overall();
    void adaugare_jucatori_valabili(BazaDeDateptr baza);
    void alegere_echipa();
    void numeEchipa();
    std::string get_nume() const;

    int cariera();

    void meci_amical();

    friend std::ostream & operator<<(std::ostream &os, const manageri &obj);
};