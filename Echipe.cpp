#include "Echipe.h"
#include "jucator.h"
#include "BazaDeDate.h"
#include "manageri.h"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "Exceptii.h"

using namespace std;

//FUNCTII PT GUI

const float Echipe::MOMENTUM_POINT_WIN = 0.02f;
const float Echipe::MOMENTUM_POINT_LOSS = -0.01f;
const float Echipe::MOMENTUM_SET_WIN = 0.15f;
const float Echipe::MOMENTUM_SET_LOSS = -0.10f;
const float Echipe::MOMENTUM_MIN = 0.80f;
const float Echipe::MOMENTUM_MAX = 1.30f;


float Echipe::getMomentum() const {
    return momentumFactor;
}
// cppcheck-suppress unusedFunction
void Echipe::resetMomentum() {
    momentumFactor = 1.0f;
}

void Echipe::updateMomentum(float change) {
    momentumFactor += change;

    if (momentumFactor < MOMENTUM_MIN) {
        momentumFactor = MOMENTUM_MIN;
    } else if (momentumFactor > MOMENTUM_MAX) {
        momentumFactor = MOMENTUM_MAX;
    }
}

const std::map<std::string, int> Echipe::REGULI_START_6 = {
    {"Setter", 1},
    {"OutsideHitter", 2},
    {"MiddleBlocker", 1},
    {"OppositeHitter", 1},
    {"Libero", 1}
};


bool Echipe::adaugare_jucatorGUI(jucatorptr jucator) {

    string input;




        if (jucator->esteAles()) {
            throw std::runtime_error("Eroare: JUCATORUL ESTE DEJA CONTRACTAT DE O ALTA ECHIPA, DE ACEEA ESTE MARCAT CU ROSU.");
        }

        if (get_buget() < jucator->get_pret()) {
            throw BugetDepasitException("Nu puteti achizitiona acest jucator deoarece este prea scump.");
        }


        jucatori.push_back(jucator);
        set_buget(get_buget() - jucator->get_pret());
        jucator->setAles(true);

        BazaDeDate::applyInflation(BazaDeDate::getStep());

        return true;
    }




float Echipe::valideaza_si_set_start_6(const std::vector<jucatorptr>& selection) {

    if (selection.size() != 6) {
        throw CompozitieInvalidaException("Trebuie selectati exact 6 jucatori.");
    }

    float suma_ovr = 0;
    std::map<std::string, int> counts;

    for (const auto& jucatorAles : selection) {
        counts[jucatorAles->get_poz()]++;
        suma_ovr += jucatorAles->get_ovr();
    }

    for (const auto& pair : REGULI_START_6) {
        const std::string& pozitie = pair.first;
        const int necesar = pair.second;
        const int actual = counts[pozitie];

        if (actual != necesar) {
            std::string msg = "Sunt necesari exact " + std::to_string(necesar) + " " + pozitie +
                              "i. Detectat: " + std::to_string(actual) + ".";
            throw CompozitieInvalidaException(msg);
        }
    }

    jucatori_fixi = std::array<jucatorptr, 6>();

    for (size_t i = 0; i < 6; ++i) {
        jucatori_fixi[i] = selection[i];
    }

    ovr_primii_6 = suma_ovr / 6;

    return ovr_primii_6;
}
[[nodiscard]] bool Echipe::poate_achizitiona_si_finaliza(
    jucatorptr jucatorNou,
    const std::vector<jucatorptr>& totiJucatoriiDisponibili
) const {

    if (jucatorNou == nullptr || jucatorNou->get_echipe() != nullptr) {
        return false;
    }
    if (buget < jucatorNou->get_pret()) {
        return false;
    }

    std::string pozitieNoua = jucatorNou->get_poz();

    int nr_outside = 0, nr_setter = 0, nr_libero = 0, nr_oppo = 0, nr_middle = 0;

    for (const auto& j : jucatori) {
        std::string poz = j->get_poz();
        if (poz == "OutsideHitter") nr_outside++;
        else if (poz == "Setter") nr_setter++;
        else if (poz == "Libero") nr_libero++;
        else if (poz == "OppositeHitter") nr_oppo++;
        else if (poz == "MiddleBlocker") nr_middle++;
    }

    if (pozitieNoua == "OutsideHitter") nr_outside++;
    else if (pozitieNoua == "Setter") nr_setter++;
    else if (pozitieNoua == "Libero") nr_libero++;
    else if (pozitieNoua == "OppositeHitter") nr_oppo++;
    else if (pozitieNoua == "MiddleBlocker") nr_middle++;

    if (nr_outside > 2 || nr_setter > 1 || nr_libero > 1 || nr_oppo > 1 || nr_middle > 1) {
        return false;
    }

    int locuri_ramase = 6 - (int)jucatori.size() - 1;

    if (locuri_ramase <= 0) {
        return true;
    }

    std::vector<jucatorptr> min_preturi;
    for (const auto& j : totiJucatoriiDisponibili) {
        if (j->get_echipe() == nullptr && std::find(jucatori.begin(), jucatori.end(), j) == jucatori.end()) {
            min_preturi.push_back(j);
        }
    }

    std::sort(min_preturi.begin(), min_preturi.end(),
        [](const jucatorptr& a, const jucatorptr& b) {
            return a->get_pret() < b->get_pret();
        });

    long long suma_min_necesara = 0;

    for (int i = 0; i < locuri_ramase; i++) {
        if (i < (int)min_preturi.size()) {
             suma_min_necesara += min_preturi[i]->get_pret();
        } else {
             return false;
        }
    }

    if (buget - jucatorNou->get_pret() < suma_min_necesara) {
        return false;
    }

    return true;
}

void Echipe::creare_in_gui(const std::vector<jucatorptr>& selectie_finala) {

    if (selectie_finala.size() != 6) {
        throw CompozitieInvalidaException("Echipa trebuie sa contina exact 6 jucatori in faza initiala de creare.");    }

    long long cost_total = 0;
    int nr_outside = 0, nr_setter = 0, nr_libero = 0, nr_oppo = 0, nr_middle = 0;

    for (const auto& jucatorAles : selectie_finala) {

        if (jucatorAles == nullptr) {
            throw CompozitieInvalidaException("Un jucator selectat este invalid.");
        }

        cost_total += jucatorAles->get_pret();
        std::string pozitie = jucatorAles->get_poz();

        if (pozitie == "OutsideHitter") nr_outside++;
        else if (pozitie == "Setter") nr_setter++;
        else if (pozitie == "Libero") nr_libero++;
        else if (pozitie == "OppositeHitter") nr_oppo++;
        else if (pozitie == "MiddleBlocker") nr_middle++;
    }

    if (nr_outside != 2 || nr_setter != 1 || nr_libero != 1 || nr_oppo != 1 || nr_middle != 1) {
        std::string msg = "Eroare de compozitie. Structura necesara: 2 OH, 1 S, 1 L, 1 Opp, 1 MB.\n";
        msg += "Detectat: OH=" + std::to_string(nr_outside) + ", S=" + std::to_string(nr_setter) + ", L=" + std::to_string(nr_libero) +
               ", Opp=" + std::to_string(nr_oppo) + ", MB=" + std::to_string(nr_middle) + ".";
        throw CompozitieInvalidaException(msg);
    }

    if (cost_total > buget) {
        std::string err_msg = "Buget depasit! Costul total este " + std::to_string(cost_total) +
                              " lei, dar aveti doar " + std::to_string(buget) + " lei.";
        throw BugetDepasitException(err_msg);
    }

    jucatori.clear();
    for (const auto& j : selectie_finala) {
        jucatori.push_back(j);
        j->adaugare_echipe(shared_from_this());
        j->transferabil(false);
        j->setAles(true);
    }

    set_buget(buget - (int)cost_total);
    set_overall();
}

//FUNCTII PENTRU CONSOLA
Echipe::Echipe(const string &nume, const vector<jucatorptr> &jucatori, float ovr, int punctaj, int seturi,int buget, array<jucatorptr, 6> jucatori_fixi)
    :ovr_primii_6(0.0f), 
      serve(false),         
      momentumFactor(1.0f), 
      nume(nume),
      jucatori(jucatori),
      jucatori_fixi(jucatori_fixi),
      ovr_tot(ovr),
      punctaj(punctaj),
      seturi(seturi),
      buget(buget){
    momentumFactor = 1.0f;
}

void Echipe::construire_echipa_primii_6() {
    int index;

    float suma = 0;

    int nr_outside = 0, nr_setter = 0, nr_libero = 0, nr_oppo = 0, nr_middle = 0;

    vector<jucatorptr> echipa_start;

    cout << "Alegeti va primii 6 jucatori ce vor intra in teren din cei contractati in clubul dumneavoastra.\n"
         << "Aceasta este lista cu toti jucatorii dumneavoastra:\n";

    for (size_t j = 0; j < jucatori.size(); ++j) {
        cout << "Jucatorul cu numarul " << j + 1 << " " << *jucatori[j] << endl;
    }

    cout << "ATENTIE!! Va trebui sa selectati 6 jucatori dintre care:\n"
         << "Un libero\n"
         << "Un setter\n"
         << "Doi outside hitter\n"
         << "Un opposite hitter\n"
         << "Un middle blocker\n";

    while (echipa_start.size() < 6) {
        cout << "\nSelectati jucatorul " << echipa_start.size() + 1 << ": ";

        if (!(cin >> index)) {
            cout << "Input invalid. Va rog introduceti un numar.\n";
            cin.clear();
            continue;
        }

        if (index < 1 || index > (int)jucatori.size()) {
            cout << "Index invalid! Alegeti intre 1 si " << jucatori.size() << ".\n";
            continue;
        }

        jucatorptr jucatorAles = jucatori[index - 1];
        string pozitie = jucatorAles->get_poz();

        bool deja_ales = false;
        for (auto j : echipa_start) {
            if (j == jucatorAles) {
                deja_ales = true;
                break;
            }
        }
        if (deja_ales) {
            cout << "Jucatorul a fost deja selectat in echipa de start! Alegeti altul.\n";
            continue;
        }


        bool limita_depasita = false;

        if (pozitie == "OutsideHitter" && nr_outside >= 2) {
            limita_depasita = true;
        } else if (pozitie == "Setter" && nr_setter >= 1) {
            limita_depasita = true;
        } else if (pozitie == "Libero" && nr_libero >= 1) {
            limita_depasita = true;
        } else if (pozitie == "OppositeHitter" && nr_oppo >= 1) {
            limita_depasita = true;
        } else if (pozitie == "MiddleBlocker" && nr_middle >= 1) {
            limita_depasita = true;
        }

        if (limita_depasita) {
            cout << "Limita de jucatori pentru pozitia " << pozitie << " a fost atinsa. Alegeti alta pozitie.\n";
            continue;
        }


        if (pozitie == "OutsideHitter") nr_outside++;
        else if (pozitie == "Setter") nr_setter++;
        else if (pozitie == "Libero") nr_libero++;
        else if (pozitie == "OppositeHitter") nr_oppo++;
        else if (pozitie == "MiddleBlocker") nr_middle++;

        suma = suma + jucatorAles->get_ovr();

        echipa_start.push_back(jucatorAles);
        cout << "Jucator (" << pozitie << ") adaugat cu succes!\n";

    }


    ovr_primii_6 = suma / 6;
    cout << "\nEchipa dumneavoastra de start a fost formata:\n";
    for (size_t k = 0; k < echipa_start.size(); ++k) {
        cout << k + 1 << ". " << *echipa_start[k] << endl;
    }
    cout << "\n Overall-ul acesteia este " << ovr_primii_6 << endl;
}
// cppcheck-suppress unusedFunction
void Echipe::vinde(jucatorptr j) {
    auto poz = find(jucatori.begin(), jucatori.end(), j);
    j->setAles(false);
    j->adaugare_echipe(nullptr);
    jucatori.erase(poz);

}


void Echipe::set_nume(string& nume) {
    this->nume = nume;
}
// cppcheck-suppress unusedFunction
jucatorptr Echipe::get_jucator(int index) const {
    return jucatori[index];
}


void Echipe::adaugare_buget(int n) {
    this->buget = this->buget + n;
}

Echipe::Echipe(const Echipe &other)
    :std::enable_shared_from_this<Echipe>(other),
    ovr_primii_6(other.ovr_primii_6),
      serve(other.serve),
      momentumFactor(other.momentumFactor),
     nume(other.nume),
      jucatori(other.jucatori),
      jucatori_fixi(other.jucatori_fixi),
      ovr_tot(other.ovr_tot),
      punctaj(other.punctaj),
      seturi(other.seturi),
      buget(other.buget){
}
// cppcheck-suppress unusedFunction
int Echipe::pret_jucator(int index) const {
    return jucatori[index]->get_pret();
}

// cppcheck-suppress unusedFunction
int Echipe::get_nr_jucatori() const {
    return jucatori.size();
}

Echipe & Echipe::operator=(const Echipe &other) {
    if (this == &other)
        return *this;
    nume = other.nume;
    jucatori = other.jucatori;
    jucatori_fixi = other.jucatori_fixi;
    ovr_tot = other.ovr_tot;
    punctaj = other.punctaj;
    seturi = other.seturi;
    buget = other.buget;
    ovr_primii_6 = other.ovr_primii_6;  
    momentumFactor = other.momentumFactor;
    serve = other.serve;
    return *this;
}

void Echipe::serve_on() {
    serve = true;
}

void Echipe::serve_off() {
    serve = false;
}

bool Echipe::get_serve() const {
    return serve;
}

Echipe::Echipe(const string& nume) {
    ovr_tot = 0;
    this->nume = nume;
    punctaj = 0;
    seturi = 0;
    buget = 200000;
    ovr_primii_6 = 0;
    momentumFactor = 1.0f;
}

Echipe::Echipe() {
    ovr_tot = 0;
    punctaj = 0;
    seturi = 0;
    buget = 200000;
    ovr_primii_6 = 0;
    momentumFactor = 1.0f;
}


void Echipe::set_overall() {
    float suma = 0;
    int j = 0;
    for (size_t i = 0; i < jucatori.size(); i++) {
        suma = suma + jucatori[i]->get_ovr();
        j++;
    }
    if (j > 0) {
        ovr_tot = suma / j;
    } else {
        ovr_tot = 0;
    }

    ovr_primii_6 = ovr_tot;
}

const vector<jucatorptr>& Echipe::get_jucatori() const {
    return jucatori;
}


std::vector<jucatorptr>& Echipe::get_jucatori_pe_teren() const {
    std::vector<jucatorptr> teren;
    for (const auto& j : jucatori_fixi) {
        if (j != nullptr) {
            teren.push_back(j);
        }
    }
    return teren;
}

std::vector<jucatorptr>& Echipe::get_jucatori_de_pe_banca() const {
    std::vector<jucatorptr> banca;

    const auto& toti_jucatorii = get_jucatori();

    for (const auto& j_contractat : toti_jucatorii) {

        bool este_pe_teren = false;
        for (const auto& j_teren : jucatori_fixi) {
            if (j_contractat == j_teren) {
                este_pe_teren = true;
                break;
            }
        }

        if (!este_pe_teren) {
            banca.push_back(j_contractat);
        }
    }
    return banca;
}

void Echipe::schimba_jucator_pe_teren(jucatorptr out, jucatorptr in) {

    if (!out || !in) {
        throw SchimbareInvalidaException("Jucatorii OUT sau IN sunt invalizi.");
    }

    if (out->get_poz() != in->get_poz()) {
        throw SchimbareInvalidaException("Pozitiile jucatorilor nu se potrivesc (" + out->get_poz() + " vs " + in->get_poz() + ").");
    }

    size_t index_out = 6;
    for (size_t i = 0; i < jucatori_fixi.size(); ++i) {
        if (jucatori_fixi[i] == out) {
            index_out = i;
            break;
        }
    }

    if (index_out >= 6) {
        throw SchimbareInvalidaException("Jucatorul OUT (" + out->get_nume() + ") nu a fost gasit in echipa de start.");    }

    jucatori_fixi[index_out] = in;

    float suma_ovr = 0;
    for (const auto& j : jucatori_fixi) {
        suma_ovr += j->get_ovr();
    }
    ovr_primii_6 = suma_ovr / 6;

}

int Echipe::get_overall() {

    if (ovr_primii_6 !=0)
        return ovr_primii_6;

    float suma = 0;
    int j = 0;
    for (size_t i = 0; i < jucatori.size(); i++) {
        suma = suma + jucatori[i]->get_ovr();
        j++;
    }
    if (j > 0) {
        ovr_tot = suma / j;
    }
    return (int)ovr_tot;
}

void Echipe::creare(const vector<jucatorptr>& lista) {
    int n = 0, index;
    vector<jucatorptr> min;

    int nr_outside = 0;
    int nr_setter = 0;
    int nr_libero = 0;
    int nr_oppo = 0;
    int nr_middle = 0;

    cout << "\n--- Creare echipa " << nume << " ---\nAveti un buget de "<<get_buget()<<" lei.\n";

    cout << "ATENTIE! Trebuie sa selectati exact 6 jucatori, respectand urmatoarea structura:\n"
         << "1 Libero, 1 Setter, 2 Outside Hitter, 1 Opposite Hitter, 1 Middle Blocker.\n";


    while (n < 6) {
        min.clear();
        long long suma_min = 0;

        cout << "\nAlegeti jucatorul " << n+1 << " (1-" << lista.size() << "): ";

        if (!(cin >> index)) {
            cout << "Input invalid! Va rog introduceti un numar.\n";
            cin.clear();
            continue;
        }

        for (size_t i = 0; i < lista.size(); i++) {
            if (!lista[i]->jucator::esteAles())
                min.push_back(lista[i]);
        }

        for (size_t i = 0; i < min.size(); i++) {
            for (size_t j = i; j < min.size(); j++) {
                if (min[i]->jucator::get_pret() > min[j]->jucator::get_pret())
                    swap(min[i], min[j]);

            }
        }

        for (int i = 0; i < 6 - n - 1; i++) {
            if (i < (int)min.size()) {
                suma_min = suma_min + min[i]->get_pret();
            }
        }


        if (index < 1 || index > (int)lista.size()) {
            cout << "Index invalid! Alegeti intre 1 si " << lista.size() << endl;
            continue;
        }

        jucatorptr jucatorAles = lista[index-1];
        string pozitie = jucatorAles->get_poz();


        if (jucatorAles->esteAles()) {
            cout << "Jucatorul a fost deja selectat! Alegeti altul.\n";
            continue;
        }

        bool limita_depasita = false;

        if (pozitie == "Libero" && nr_libero >= 1) {
            limita_depasita = true;
        } else if (pozitie == "Setter" && nr_setter >= 1) {
            limita_depasita = true;
        } else if (pozitie == "OutsideHitter" && nr_outside >= 2) {
            limita_depasita = true;
        } else if (pozitie == "OppositeHitter" && nr_oppo >= 1) {
            limita_depasita = true;
        } else if (pozitie == "MiddleBlocker" && nr_middle >= 1) {
            limita_depasita = true;
        }

        if (limita_depasita) {
            cout << "Limita de jucatori pentru pozitia " << pozitie << " a fost atinsa. Alegeti alt jucator.\n";
            continue;
        }

        if (get_buget() - jucatorAles->get_pret() < suma_min || get_buget() < jucatorAles->get_pret()) {
            cout << "Jucatorul este prea scump pentru bugetul dumneavoastra sau va lasa insuficienti bani pentru restul echipei ("<<suma_min<<" lei minim necesar). Alegeti altul!\n";
            continue;
        }

        cout << jucatorAles->get_pret() << " lei\n";

        if (pozitie == "Libero") nr_libero++;
        else if (pozitie == "Setter") nr_setter++;
        else if (pozitie == "OutsideHitter") nr_outside++;
        else if (pozitie == "OppositeHitter") nr_oppo++;
        else if (pozitie == "MiddleBlocker") nr_middle++;

        jucatorAles->adaugare_echipe(shared_from_this());
        jucatorAles->transferabil(false);
        jucatori.push_back(jucatorAles);
        set_buget(get_buget() - jucatorAles->get_pret());
        jucatorAles->setAles(true);
        n++;
        cout << "Jucator (" << pozitie << ") adaugat cu succes!\nMai aveti "<< get_buget()<<" lei in buget.\n";
    }
}
int Echipe::get_buget() const {
    return buget;
}

void Echipe::set_buget(const int buget) {
    this->buget = buget;
}
ostream& operator<<(ostream& os, const Echipe& e) {
    os << "\n=== Echipa: " << e.nume << " ===\n";
    for (size_t i = 0; i < e.jucatori.size(); i++)
        os << "- " << *e.jucatori[i] << "\n";
    os << "Overall-ul echipei este: " << e.ovr_tot << "\n";
    return os;
}
// cppcheck-suppress unusedFunction
void Echipe::eliminaJucator(jucatorptr j) {
    jucatori.erase(
        std::remove(jucatori.begin(), jucatori.end(), j),
        jucatori.end()
    );
}
// cppcheck-suppress unusedFunction
void Echipe::adaugare_jucatori(const vector<jucatorptr>& lista) {
    for(auto j : lista){
        jucatori.push_back(j);
        j->setAles(true);
    }
}

const string& Echipe::getNume() const { return nume; }

int Echipe::getPunctaj() { return punctaj; }

int Echipe::getSeturi() { return seturi; }

void Echipe::modifPunctaj() { punctaj++; }

void Echipe::modifSetur() { seturi++; }

void Echipe::resetPunctaj() { punctaj = 0; }

void Echipe::resetSetur() { seturi = 0; }

void Echipe::set_nume() {cin>>nume; }

void Echipe::set_overall(int overall) {
    ovr_tot = overall;
    ovr_primii_6 = overall;
}

void Echipe::adaugaJucatorExistent(jucatorptr j) {
    if (j != nullptr) {
        jucatori.push_back(j);

        j->adaugare_echipe(shared_ptr<Echipe>(shared_from_this()));

        this->set_overall();
    }
}

// cppcheck-suppress unusedFunction
void Echipe::adaugare_jucator(BazaDeDateptr baza) {
    cout << "Alegeti jucatorul dorit din urmatoarea lista:\n";
    baza->afiseazaJucatori();

    string input;
    int index;

    while (true) {
        cout << "Alegeti un jucator (1-" << baza->getLista().size() << "): \n"
             << "Daca doriti sa reveniti din acest menu scrieti back in terminal.\n";

        cin >> input;

        if (input == "back" || input == "Back") {
            cout << "Revenire la meniul anterior.\n";
            return;
        }

        try {

            index = stoi(input);

        } catch (const std::invalid_argument& e) {
            cout << "Input invalid! Va rog introduceti un numar sau 'back'.\n";
            continue;
        } catch (const std::out_of_range& e) {
            cout << "Numar prea mare. Va rog introduceti un index valid.\n";
            continue;
        }


        if (index < 1 || index > (int)baza->getLista().size()) {
            cout << "Index invalid! Alegeti intre 1 si " << baza->getLista().size() << endl;
            continue;
        }

        jucatorptr jucatorAles = baza->getLista()[index - 1];

        if (jucatorAles->esteAles()) {
            cout << "Jucatorul a fost deja selectat! Alegeti altul.\n";
            continue;
        }

        if (get_buget() < jucatorAles->get_pret()) {
            cout << "Nu aveti bugetul adecvat pentru acest jucator. Alegeti altul!\n";
            continue;
        }

        cout << jucatorAles->get_pret() << endl;

        jucatori.push_back(jucatorAles);
        set_buget(get_buget() - jucatorAles->get_pret());
        jucatorAles->setAles(true);
        cout << "Jucator adaugat cu succes!\nMai aveti " << get_buget() << " lei in buget.\n";

        break;
    }
}