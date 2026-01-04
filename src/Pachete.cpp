#include "Pachete.h"
#include <cstdlib>

Pachet::Pachet(std::string n, int c) : nume(n), cost(c) {}

int Pachet::getCost() const { return cost; }

const std::string& Pachet::getNume() const { return nume; }

jucatorptr Pachet::extrageDupaOvr(const std::vector<jucatorptr>& lista, int minOvr, int maxOvr) {
    std::vector<jucatorptr> filtrati;
    for (const auto& j : lista) {
        if (j->get_ovr() >= minOvr && j->get_ovr() <= maxOvr) {
            filtrati.push_back(j);
        }
    }
    if (filtrati.empty()) return nullptr;
    return filtrati[std::rand() % filtrati.size()];
}

PachetBronze::PachetBronze() : Pachet("Bronze Pack", 5000) {}

jucatorptr PachetBronze::deschide(const std::vector<jucatorptr>& baza) {
    float r = static_cast<float>(std::rand()) / RAND_MAX;
    if (r < 0.85f) return extrageDupaOvr(baza, 0, 70);
    return extrageDupaOvr(baza, 71, 78);
}

PachetGold::PachetGold() : Pachet("Gold Pack", 25000) {}

jucatorptr PachetGold::deschide(const std::vector<jucatorptr>& baza) {
    float r = static_cast<float>(std::rand()) / RAND_MAX;
    if (r < 0.70f) return extrageDupaOvr(baza, 75, 84);
    return extrageDupaOvr(baza, 85, 99);
}