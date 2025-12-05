#include "jucator.h"
#include "Echipe.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <algorithm>
#include "BazaDeDate.h"

using namespace std;

Persoana::Persoana(const std::string& nume) : nume(nume) {}

Persoana::Persoana() : nume("Necunoscut") {}


// cppcheck-suppress unusedFunction
float Persoana::obtine_rating_risc_total() const {
    return 0.1f * calculeaza_indice_risc_financiar();
}

const std::string& Persoana::get_nume() const {
    return nume;
}

void Persoana::set_nume(const std::string& numelocal) {
    this->nume = numelocal;
}

std::ostream& operator<<(std::ostream& os, const Persoana& p) {
    os << "Nume: " << p.nume << ", ";
    p.afiseaza_detalii_specifice(os);
    return os;
}


float jucator::calculeaza_indice_risc_financiar() const {
    return (float)std::max(0, pret) / 1000.0f;
}

void jucator::afiseaza_detalii_specifice(std::ostream& os) const {
    os << "Pozitie: " << pozitie << ", OVR: " << ovr << ", Pret: " << pret << " lei.";
    if (get_transferabil() == true) {
        os << " [Disponibil]";
    }
}

float Persoana::obtine_scor_tactica_final() const {

    const jucator* j = dynamic_cast<const jucator*>(this);

    if (j) {

        float impact_specific = j->calculeaza_impact_meci();
        float ovr_pondere = j->get_ovr();

        return (impact_specific * 0.8f) + (ovr_pondere * 0.2f);

    } else {
        return 0.0f;
    }
}


void jucator::overall() {
    ovr = (spike_power + receive + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility + speed + height) / 9;
}

int jucator::set_val(int valoarea) const {
    if (valoarea < 0) valoarea = 0;
    if (valoarea > 99) valoarea = 99;
    return valoarea;
}

void jucator::set_pret() {
    pret = ovr * 200;

    if (ales == true)
        pret = (int)(pret * 1.5);
}

jucator::jucator(const string &nume_pers, const string &pozitie, int ovr, int spike_power, int receive, int spike_accuracy,
        int serve_power, int serve_accuracy, int vertical_jump, int mobility, int speed, int pret, int height,
        bool ales, Echipeptr echipa)
    :
     Persoana(nume_pers),
      pozitie(pozitie), ovr(ovr), spike_power(spike_power), receive(receive),
      spike_accuracy(spike_accuracy), serve_power(serve_power), serve_accuracy(serve_accuracy),
      vertical_jump(vertical_jump), mobility(mobility), speed(speed), pret(pret), height(height),
      ales(ales),  echipa_curenta(echipa), lista_transferuri(false)
      {}

jucator::jucator()

    : Persoana(), 
    

      pozitie("Necunoscuta"),
      ovr(0),
      spike_power(0),
      receive(0),
      spike_accuracy(0),
      serve_power(0),
      serve_accuracy(0),
      vertical_jump(0),
      mobility(0),
      speed(0),
      pret(0),
      height(0),
      ales(false),
      

      echipa_curenta(nullptr),
      lista_transferuri(false) 
{
   
}


  


void jucator::adaugare_echipe(Echipeptr echipa) {
    this->echipa_curenta = echipa;
    lista_transferuri = false;
}

void jucator::transferabil(bool tranfer) {
    this->lista_transferuri = tranfer;
}

bool jucator::get_transferabil() const {
    return this->lista_transferuri;
}

Echipeptr jucator::get_echipe() {
    return echipa_curenta;
}



void jucator::set_poz(const string &poz) {
    this->pozitie = poz;
}

int jucator::get_pret() const {
    float modifier = BazaDeDate::getGlobalMarketModifier();
    return (int)(this->pret * modifier);
}

void jucator::valori(vector<int> a) {
    spike_power = set_val(a[0]);
    receive = set_val(a[1]);
    spike_accuracy = set_val(a[2]);
    serve_power = set_val(a[3]);
    serve_accuracy = set_val(a[4]);
    vertical_jump = set_val(a[5]);
    mobility = set_val(a[6]);
    height = set_val(a[7]);
    speed = set_val(a[8]);
}

bool jucator::esteAles() const { return ales; }

void jucator::setAles(bool val) {
    ales = val;
    set_pret();
}

int jucator::get_ovr() const { return ovr; }

std::string jucator::to_string() const {
    std::string s = get_nume() + " (" + pozitie + ") - OVR: " + std::to_string(ovr) + ", Pret: " + std::to_string(pret);
    if (get_transferabil() == true)
        s += " Disponibil.";
    return s;
}


jucator &jucator::operator=(const jucator &j) {
    if (this != &j) {
        Persoana::operator=(j);

        ovr = j.ovr;
        spike_power = j.spike_power;
        height = j.height;
        receive = j.receive;
        spike_accuracy = j.spike_accuracy;
        serve_power = j.serve_power;
        serve_accuracy = j.serve_accuracy;
        vertical_jump = j.vertical_jump;
        mobility = j.mobility;
        speed = j.speed;
        pret = j.pret;
        pozitie = j.pozitie;
        lista_transferuri = j.lista_transferuri;
    }
    return *this;
}

void jucator::setOvr(const int ovrlocal) {
    this->ovr = ovrlocal;
}


 void jucator::citeste(ifstream &in) {
    std::string temp_nume;
    in >> temp_nume;
    set_nume(temp_nume);

    in >> spike_power;
    in >> receive;
    in >> spike_accuracy;
    in >> serve_power;
    in >> serve_accuracy;
    in >> vertical_jump;
    in >> mobility;
    in >> speed;
    in >> height;

    spike_power = set_val(spike_power);
    receive = set_val(receive);
    spike_accuracy = set_val(spike_accuracy);
    serve_power = set_val(serve_power);
    serve_accuracy = set_val(serve_accuracy);
    vertical_jump = set_val(vertical_jump);
    mobility = set_val(mobility);
    speed = set_val(speed);
    height = set_val(height);

    overall();
    set_pret();
    transferabil(false);
}




OutsideHitter::OutsideHitter(int explosiveness)
    : explosiveness(explosiveness) {
}

void OutsideHitter::valori(vector <int> a) {
    jucator::valori(a);
    explosiveness = set_val(a[9]);
    overall();
    set_pret();
}

OutsideHitter::OutsideHitter() {
    pozitie = "OutsideHitter";
    explosiveness = 0;
}

void OutsideHitter::overall(){
    ovr = ((spike_power * 2 + receive * 2 + spike_accuracy * 2 + serve_power + serve_accuracy + vertical_jump * 2 +
            mobility + speed + explosiveness * 3 + height * 2) / 17);
}



float OutsideHitter::calculeaza_impact_meci() const {
    float impact = (
        (float)spike_power * 0.40f +
        (float)receive * 0.30f +
        (float)explosiveness * 0.30f
    );
    return impact;
}


// cppcheck-suppress unusedFunction
Persoanaptr OutsideHitter::clone() const {

    return std::make_shared<OutsideHitter>(*this);
}

void OutsideHitter::set_pret() {
    long long ovr_elite_bonus = 0;
    int ovr_peste_80 = max(0, ovr - 80);
    if (ovr_peste_80 > 0) {
        ovr_elite_bonus = (long long)ovr_peste_80 * ovr_peste_80 * 250;
    }

    long long cost_critic =
        (long long)explosiveness * 30 +
        (long long)spike_power * 25 +
        (long long)receive * 15 +
        (long long)vertical_jump * 10;

    pret = (int)(15000 + ovr * 100 + cost_critic + ovr_elite_bonus);

    if (ales == true) pret = (int)(pret * 1.5);
}

void OutsideHitter::citeste(std::ifstream &in) {
    jucator::citeste(in);
    in >> explosiveness;
    explosiveness = set_val(explosiveness);
    overall();
    set_pret();
}


Setter::Setter(int set_precision, int set_iq)
    : set_precision(set_precision),
      set_iq(set_iq) {
}

Setter::Setter() {
    set_precision = set_iq = 0;
    pozitie = "Setter";
}

void Setter::valori(vector <int> a) {
    jucator::valori(a);
    set_precision = set_val(a[9]);
    set_iq = set_val(a[10]);
    overall();
    set_pret();
}

void Setter::overall() {
    ovr = (spike_power + receive + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility * 2 +
           speed * 2 + set_precision * 3 + set_iq * 3 + height) / 17;
}


// cppcheck-suppress unusedFunction
Persoanaptr Setter::clone() const {
    return std::make_shared<Setter>(*this);
}

float Setter::calculeaza_impact_meci() const {

    float impact = (
        (float)set_precision * 0.45f +
        (float)set_iq * 0.35f +
        (float)mobility * 0.20f
    );
    return impact;
}

void Setter::set_pret() {
    long long ovr_elite_bonus = 0;
    int ovr_peste_80 = max(0, ovr - 80);
    if (ovr_peste_80 > 0) {
        ovr_elite_bonus = (long long)ovr_peste_80 * ovr_peste_80 * 300;
    }

    long long cost_critic =
        (long long)set_precision * 35 +
        (long long)set_iq * 35 +
        (long long)mobility * 15 +
        (long long)speed * 10;

    pret = (int)(12000 + ovr * 90 + cost_critic + ovr_elite_bonus);

    if (ales == true) pret = (int)(pret * 1.5);
}

void Setter::citeste(std::ifstream &in) {
    jucator::citeste(in);
    in >> set_iq;
    in>> set_precision;
    set_precision = set_val(set_precision);
    set_iq  = set_val(set_iq);
    overall();
    set_pret();
}

const string& jucator::get_poz() const {
    return pozitie;
}


Libero::Libero(int reflexes)
    : reflexes(reflexes) {
}

Libero::Libero() {
    reflexes = 0;
    pozitie = "Libero";
}


void Libero::overall() {
    ovr = (spike_power + receive * 3 + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility * 2 +
           speed * 3 + reflexes * 3 + height) / 17;
}


float Libero::calculeaza_impact_meci() const {

    float impact = (
        (float)receive * 0.50f +
        (float)reflexes * 0.30f +
        (float)speed * 0.20f
    );
    return impact;
}

void Libero::set_pret() {
    long long ovr_elite_bonus = 0;
    int ovr_peste_80 = max(0, ovr - 80);
    if (ovr_peste_80 > 0) {
        ovr_elite_bonus = (long long)ovr_peste_80 * ovr_peste_80 * 150;
    }

    long long cost_critic =
        (long long)reflexes * 40 +
        (long long)receive * 30 +
        (long long)speed * 15;

    pret = (int)(8000 + ovr * 70 + cost_critic + ovr_elite_bonus);

    if (ales == true) pret = (int)(pret * 1.5);
}

// cppcheck-suppress unusedFunction
Persoanaptr Libero::clone() const {
    return std::make_shared<Libero>(*this);
}

void Libero::valori(vector <int> a) {
    jucator::valori(a);
    reflexes = set_val(a[9]);
    overall();
    set_pret();
}
void Libero::citeste(std::ifstream &in) {
    jucator::citeste(in);
    in >> reflexes;
    reflexes = set_val(reflexes);
    overall();
    set_pret();
}

MiddleBlocker::MiddleBlocker(int blocking)
    : blocking(blocking) {
}

MiddleBlocker::MiddleBlocker() {
    blocking = 0;
    pozitie = "MiddleBlocker";
}

void MiddleBlocker::overall() {
    ovr = (spike_power + receive + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility * 2 +
           speed * 2 + blocking * 3 + height * 4) / 17;
}


float MiddleBlocker::calculeaza_impact_meci() const {
    float impact = (
        (float)blocking * 0.40f +
        (float)height * 0.30f +
        (float)spike_power * 0.20f +
        (float)vertical_jump * 0.10f
    );
    return impact;
}

void MiddleBlocker::set_pret() {
    long long ovr_elite_bonus = 0;
    int ovr_peste_80 = max(0, ovr - 80);
    if (ovr_peste_80 > 0) {
        ovr_elite_bonus = (long long)ovr_peste_80 * ovr_peste_80 * 200;
    }

    long long cost_critic =
        (long long)height * 40 +
        (long long)blocking * 35 +
        (long long)vertical_jump * 15;

    pret = (int)(10000 + ovr * 90 + cost_critic + ovr_elite_bonus);

    if (ales == true) pret = (int)(pret * 1.5);
}
// cppcheck-suppress unusedFunction
Persoanaptr MiddleBlocker::clone() const {
    return std::make_shared<MiddleBlocker>(*this);
}

void MiddleBlocker::valori(vector <int> a) {
    jucator::valori(a);
    blocking = set_val(a[9]);
    overall();
    set_pret();
}
void MiddleBlocker::citeste(std::ifstream &in) {
    jucator::citeste(in);
    in >> blocking;
    blocking = set_val(blocking);
    overall();
    set_pret();
}


OppositeHitter::OppositeHitter(int explosiveness)
    : explosiveness(explosiveness) {
}

OppositeHitter::OppositeHitter() {
    explosiveness = 0;
    pozitie = "OppositeHitter";
}

void OppositeHitter::overall() {
    ovr = ((spike_power * 2 + receive * 2 + spike_accuracy * 2 + serve_power + serve_accuracy + vertical_jump * 2 +
            mobility + speed + explosiveness * 3 + height * 2) / 17);
}


float OppositeHitter::calculeaza_impact_meci() const {

    float impact = (
        (float)spike_power * 0.50f +
        (float)explosiveness * 0.30f +
        (float)spike_accuracy * 0.20f
    );
    return impact;
}

void OppositeHitter::set_pret() {
    long long ovr_elite_bonus = 0;
    int ovr_peste_80 = max(0, ovr - 80);
    if (ovr_peste_80 > 0) {
        ovr_elite_bonus = (long long)ovr_peste_80 * ovr_peste_80 * 250;
    }

    long long cost_critic =
        (long long)explosiveness * 30 +
        (long long)spike_power * 30 +
        (long long)vertical_jump * 15 +
        (long long)height * 10;

    pret = (int)(15000 + ovr * 100 + cost_critic + ovr_elite_bonus);

    if (ales == true) pret = (int)(pret * 1.5);
}
// cppcheck-suppress unusedFunction
Persoanaptr OppositeHitter::clone() const {
    return std::make_shared<OppositeHitter>(*this);
}

void OppositeHitter::valori(vector <int> a) {
    jucator::valori(a);
    explosiveness = set_val(a[9]);
    overall();
    set_pret();
}
void OppositeHitter::citeste(std::ifstream &in) {
    jucator::citeste(in);
    in >> explosiveness;
    explosiveness = set_val(explosiveness);
    overall();
    set_pret();
}