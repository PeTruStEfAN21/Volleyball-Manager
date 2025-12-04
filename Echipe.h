#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include <map>

#include "jucator.h"

using namespace std;

class BazaDeDate;
class jucator;

using BazaDeDateptr = std::shared_ptr<BazaDeDate>;
using jucatorptr = std::shared_ptr<jucator>;
using Echipeptr = std::shared_ptr<Echipe>;

class Echipe: public std::enable_shared_from_this<Echipe>{
private:
    string nume;
    vector<jucatorptr> jucatori;
    array<jucatorptr, 6> jucatori_fixi;
    float ovr_tot,ovr_primii_6;
    int punctaj, seturi;
    int buget;
    bool serve;
    float momentumFactor;


public:
//FUNCTII PENTRU GUI
    float valideaza_si_set_start_6(const std::vector<jucatorptr>& selection);

    static const std::map<std::string, int> REGULI_START_6;

    static bool verificaReguliStart6(const std::vector<jucatorptr>& selection);

    void creare_in_gui(const vector<jucatorptr>& lista);

    void achizitioneaza_pentru_draft(jucatorptr jucatorAles, const std::vector<jucatorptr>& jucatoriDisponibili);

[[nodiscard]] bool poate_achizitiona_si_finaliza(
    jucatorptr jucatorNou,
    const std::vector<jucatorptr>& totiJucatoriiDisponibili
) const;

    bool adaugare_jucatorGUI(jucatorptr jucator) ;


    std::vector<jucatorptr> get_jucatori_pe_teren() const;

    std::vector<jucatorptr> get_jucatori_de_pe_banca() const;

    void schimba_jucator_pe_teren(jucatorptr out, jucatorptr in);





    //FUNCTII PENTRU CONSOLA

    static const float MOMENTUM_POINT_WIN;
    static const float MOMENTUM_POINT_LOSS;
    static const float MOMENTUM_SET_WIN;
    static const float MOMENTUM_SET_LOSS;
    static const float MOMENTUM_MIN;
    static const float MOMENTUM_MAX;

    float getMomentum() const;
    void resetMomentum();
    void updateMomentum(float change);

    void vinde(jucatorptr j);

    jucatorptr get_jucator(int index) const;

    vector<jucatorptr> get_jucatori() const;

    const int pret_jucator(int index) const;

    const int get_nr_jucatori() const;

    void eliminaJucator(jucatorptr j) ;

    void adaugaJucatorExistent(jucatorptr j) ;

    void serve_on();

    void serve_off();

    const bool get_serve() const;

    void construire_echipa_primii_6();

    ~Echipe() = default;

    int get_buget() const ;

    void set_buget(const int buget) ;

    Echipe(const string &nume, const vector<jucatorptr> &jucatori, float ovr, int punctaj, int seturi, int buget, array<jucatorptr, 6> jucatori_fixi);
    explicit Echipe(const string& nume);
    Echipe();
    Echipe(const Echipe &other);
    Echipe &operator=(const Echipe &other);


    void set_overall();
    int get_overall();
    void creare(const vector<jucatorptr>& lista);
    friend ostream& operator<<(ostream& os, const Echipe& e);
    void adaugare_jucatori(const vector<jucatorptr>& jucatori);

    const string& getNume() const;
    int getPunctaj();
    int getSeturi();
    void modifPunctaj();
    void modifSetur();
    void resetPunctaj();
    void resetSetur();



    void set_nume();
    void set_nume(string nume);
    void adaugare_buget(int n);
    void set_overall(int ovr);
    void adaugare_jucator(BazaDeDateptr baza);




};