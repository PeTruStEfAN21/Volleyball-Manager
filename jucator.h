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



class jucator {
protected:
    std::string nume, pozitie;
    int ovr, spike_power, receive, spike_accuracy, serve_power, serve_accuracy, vertical_jump, mobility, speed, pret, height;
    bool ales;
    Echipeptr echipa_curenta;
    bool lista_transferuri;

    virtual void overall();
    int set_val(int valoarea) const;
    virtual void set_pret();

public:

    void transferabil(bool tranfer);

    bool get_transferabil() const ;

    jucator(const std::string &nume, const std::string &pozitie, int ovr, int spike_power, int receive, int spike_accuracy,
            int serve_power, int serve_accuracy, int vertical_jump, int mobility, int speed, int pret, int height,
            bool ales, Echipeptr echipa);

    void adaugare_echipe(Echipeptr echipa);

    Echipeptr get_echipe();

    const std::string get_nume() const;

    virtual int get_pret() const;

    virtual ~jucator() = default;

    jucator();

    void set_poz(const std::string &poz);

    virtual void valori(std::vector<int> a);

    bool esteAles() const;
    void setAles(bool val);
    int get_ovr() const;

    virtual void afiseaza() const;



    friend std::ostream &operator<<(std::ostream &out, const jucator &j);

    jucator &operator=(const jucator &j);

    virtual void citeste(std::ifstream &in);

    const std::string get_poz() const;

    void set_nume(const std::string &nume);
};


class OutsideHitter : public jucator {
    int explosiveness;
public:
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
    explicit OppositeHitter(int explosiveness);
    OppositeHitter();
    ~OppositeHitter() override = default;
    void overall() override;
    void set_pret() override;
    void valori(std::vector <int> a) override;
    void citeste(std::ifstream &in) override;
};