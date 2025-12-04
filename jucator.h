#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <memory>


class BazaDeDate;
class Echipe;

using Echipeptr = std::shared_ptr<Echipe> ;

#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

class Persoana;
using Persoanaptr = std::shared_ptr<Persoana>;

class Persoana {
protected:
    std::string nume;

    virtual float calculeaza_indice_risc_financiar() const = 0;

    virtual void afiseaza_detalii_specifice(std::ostream& os) const = 0;

public:
    virtual ~Persoana() = default;

    virtual Persoanaptr clone() const = 0;

    Persoana(const std::string& nume);
    Persoana();

    float obtine_rating_risc_total() const;

    const std::string get_nume() const;
    void set_nume(const std::string& nume);

    float obtine_scor_tactica_final() const;

    friend std::ostream& operator<<(std::ostream& os, const Persoana& p);
};





class jucator : public Persoana {
protected:
    std::string pozitie;
    int ovr, spike_power, receive, spike_accuracy, serve_power, serve_accuracy, vertical_jump, mobility, speed, pret, height;
    bool ales;
    Echipeptr echipa_curenta;
    bool lista_transferuri;

    virtual void overall();
    int set_val(int valoarea) const;
    virtual void set_pret();

    float calculeaza_indice_risc_financiar() const override;
    void afiseaza_detalii_specifice(std::ostream& os) const override;


public:
    jucator(const std::string &nume, const std::string &pozitie, int ovr, int spike_power, int receive, int spike_accuracy,
            int serve_power, int serve_accuracy, int vertical_jump, int mobility, int speed, int pret, int height,
            bool ales, Echipeptr echipa);

    jucator();

    virtual float calculeaza_impact_meci() const = 0;


    virtual ~jucator() = default;

    void transferabil(bool tranfer);
    bool get_transferabil() const ;
    void adaugare_echipe(Echipeptr echipa);
    Echipeptr get_echipe();

    virtual int get_pret() const;
    void set_poz(const std::string &poz);
    virtual void valori(std::vector<int> a);
    bool esteAles() const;
    void setAles(bool val);
    int get_ovr() const;
    virtual std::string to_string() const;

    jucator &operator=(const jucator &j);

    virtual void citeste(std::ifstream &in);
    const std::string get_poz() const;


};

class OutsideHitter : public jucator {
    int explosiveness;
public:

    Persoanaptr clone() const override;

    float calculeaza_impact_meci() const override;

    explicit OutsideHitter(int explosiveness);
    ~OutsideHitter() override = default;
    void valori(std::vector <int> a) override;
    OutsideHitter();
    void overall() override;
    void set_pret() override;
    void citeste(std::ifstream &in) override;
};

class Setter : public jucator {
    int set_precision, set_iq;
public:

    Persoanaptr clone() const override;

    float calculeaza_impact_meci() const override;

    Setter(int set_precision, int set_iq);
    Setter();
    ~Setter() override = default;
    void valori(std::vector <int> a) override;
    void overall() override;
    void set_pret() override;
    void citeste(std::ifstream &in) override;
};

class Libero : public jucator {
    int reflexes;
public:

    Persoanaptr clone() const override;
    float calculeaza_impact_meci() const override;

    explicit Libero(int reflexes);
    Libero();
    ~Libero() override = default;
    void overall() override;
    void set_pret() override;
    void valori(std::vector <int> a) override;
    void citeste(std::ifstream &in) override;
};

class MiddleBlocker : public jucator {
    int blocking;
public:

    Persoanaptr clone() const override;
    float calculeaza_impact_meci() const override;

    explicit MiddleBlocker(int blocking);
    MiddleBlocker();
    ~MiddleBlocker() override = default;
    void overall() override;
    void set_pret() override;
    void valori(std::vector <int> a) override;
    void citeste(std::ifstream &in) override;
};

class OppositeHitter : public jucator {
    int explosiveness;
public:

    Persoanaptr clone() const override;
    float calculeaza_impact_meci() const override;

    explicit OppositeHitter(int explosiveness);
    OppositeHitter();
    ~OppositeHitter() override = default;
    void overall() override;
    void set_pret() override;
    void valori(std::vector <int> a) override;
    void citeste(std::ifstream &in) override;
};

