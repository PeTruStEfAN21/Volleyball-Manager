#include "BazaDeDate.h"
#include "jucator.h"
#include "Echipe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;


BazaDeDate::BazaDeDate(const vector<jucatorptr> &jucatori)
    : jucatori(jucatori) {
}

BazaDeDate::BazaDeDate() = default;

BazaDeDate::~BazaDeDate() {
}

BazaDeDate::BazaDeDate(const BazaDeDate &other)
    : jucatori(other.jucatori),
      echipe_disponibile(other.echipe_disponibile) {
}

BazaDeDate & BazaDeDate::operator=(const BazaDeDate &other) {
    if (this == &other)
        return *this;
    jucatori = other.jucatori;
    echipe_disponibile = other.echipe_disponibile;
    return *this;
}


void BazaDeDate::adaugaJucator(jucatorptr j) {
    jucatori.push_back(j);
}

void BazaDeDate::adaugaEchipe(Echipeptr e) {
    echipe_disponibile.push_back(e);
}

void BazaDeDate::eliminaEchipa(Echipeptr e) {
    auto& lista = echipe_disponibile;

    lista.erase(
        std::remove(lista.begin(), lista.end(), e),
        lista.end()
    );
}

void BazaDeDate::afiseazaJucatori() const {
    cout << "\n=== Lista jucatorilor inregistrati ===\n";
    for (size_t i = 0; i < jucatori.size(); i++) {
        cout << i + 1 << ". ";
        jucatori[i]->afiseaza();
        cout<< endl;
    }
}


void BazaDeDate::afisare_jucatori_necontractati() {
    cout << "\n=== Lista jucatorilor necontractati valabili ===\n";
    for (size_t i = 0; i < jucatori.size(); i++) {
        if (jucatori[i]->get_transferabil() == true){
            cout << i + 1 << ". ";
            jucatori[i]->afiseaza();
            cout<< endl;
        }
    }
}

Echipeptr BazaDeDate::getEchipe(size_t index) const {
    return echipe_disponibile[index]; 
}

const vector<jucatorptr>& BazaDeDate::getLista() const {
    return jucatori; 
}

const vector<Echipeptr>& BazaDeDate::getListe() const {
    return echipe_disponibile; 
}

Echipeptr BazaDeDate::alege_echipa_random() {
    if (echipe_disponibile.empty()) return nullptr;

    int idx = rand() % echipe_disponibile.size();
    return echipe_disponibile[idx];
}

void BazaDeDate::afiseazaEchipe() const {
    for (size_t i = 0; i < echipe_disponibile.size(); i++) {
        cout<<"Echipa "<<i+1<<": \n"
        << echipe_disponibile[i];
    }
}

jucatorptr BazaDeDate::getJucator(int index) const {
    return jucatori[index];
}

int BazaDeDate::alege_echipa_random_index() {
    if (echipe_disponibile.empty()) return -1;
    return rand() % echipe_disponibile.size();
}