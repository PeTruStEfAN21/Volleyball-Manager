#include "Liga.h"
#include "Meci.h"     
#include "Echipe.h"   
#include "BazaDeDate.h"
#include "jucator.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

#include "manageri.h"

using namespace std;




Liga::Liga(BazaDeDateptr baza) : baza(baza)  {
    ales = vector<vector<bool>>(12, vector<bool>(12, false));
    etapa_jucat = vector<bool>(12, false);
    punctaje.resize(12, 0);
    for (int i = 0; i < 12; i++) {
        ales[i][i] = true;
    }
}

void Liga::adaugare_echipa(Echipeptr echipa) {
    echipe.push_back(echipa);
}

void Liga::creare_liga() {
    for (size_t i = 0; i < 11; i++) {

        Echipeptr echipa_aleasa = baza->alege_echipa_random();

        if (echipa_aleasa == nullptr) {
            break;
        }

        echipe.push_back(echipa_aleasa);

        baza->eliminaEchipa(echipa_aleasa);

    }
}
Liga::Liga() {
    ales = vector<vector<bool>>(12, vector<bool>(12, false));
    etapa_jucat = vector<bool>(12, false);
    punctaje.resize(12, 0);
    for (int i = 0; i < 12; i++) {
        ales[i][i] = true;

    }
}

void Liga::reset_etapa() {
    auto lista = baza->getListe();
    int n = lista.size();
    etapa_jucat = vector<bool>(n, false);
}

Echipeptr Liga::meciliga() {
    int n = echipe.size();

    int index_echipa_manager = n - 1;

    int index_adversar = -1;

    while (true) {

        index_adversar = rand() % (n - 1);

        if (etapa_jucat[index_adversar] == true)
            continue;

        if (ales[index_echipa_manager][index_adversar] == true)
            continue;

        break;
    }

    ales[index_echipa_manager][index_adversar] = true;
    ales[index_adversar][index_echipa_manager] = true;
    etapa_jucat[index_adversar] = true;
    etapa_jucat[index_echipa_manager] = true;

    Meci meci_local(echipe[index_echipa_manager], echipe[index_adversar]);
    const Echipeptr castigatoare = meci_local.meci();


    if (castigatoare == echipe[index_echipa_manager]) {
        cout << "Felicitari, echipa ta a castigat meciul, obtinand 3 puncte in clasament.\n";
        punctaje[index_echipa_manager] += 3;
        echipe[index_echipa_manager]->adaugare_buget(5000);
        echipe[index_adversar]->adaugare_buget(2000);
    } else {
        cout << "Din pacate ati pierdut meciul, nu ati obtinut niciun punct in clasament...\n";
        punctaje[index_adversar] += 3;
        echipe[index_echipa_manager]->adaugare_buget(5000);
        echipe[index_adversar]->adaugare_buget(2000);
    }

    return castigatoare;
}


const std::vector<Echipeptr>& Liga::getEchipe() const { return echipe; }


Liga::~Liga() {
}


bool Liga::isSeasonFinished() const {
    int n = echipe.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (!ales[i][j]) {
                return false;
            }
        }
    }
    return true;
}


Echipeptr Liga::getNextAdversarPentruManager() const {
    int n = echipe.size();
    int index_echipa_manager = n - 1;

    if (n < 2) return nullptr;

    for (int tentativa = 0; tentativa < 100; tentativa++) {
        int index_adversar = rand() % (n - 1);



        if (!etapa_jucat[index_adversar] && !ales[index_echipa_manager][index_adversar]) {
            return echipe[index_adversar];
        }
    }
    return nullptr;
}


std::vector<TeamScore> Liga::getSortedClasament() const {
    std::vector<TeamScore> clasament;
    int n = echipe.size();

    for (int i = 0; i < n; ++i) {
        if (echipe[i]) {
            clasament.push_back({
                echipe[i]->getNume(),
                punctaje[i]
            });
        }
    }

    std::sort(clasament.begin(), clasament.end(), [](const TeamScore& a, const TeamScore& b) {
        return a.puncte > b.puncte;
    });

    return clasament;
}


void Liga::afisare_punctaje() {
    for (size_t i = 0; i < punctaje.size(); i++)
        cout << echipe[i]->getNume() << ": " << punctaje[i] << "\n";
}

void Liga::meciuri() {
    int n = echipe.size();
    int index_echipa_manager = n - 1;

    if (punctaje.size() != n)
        punctaje.resize(n, 0);

    bool progres_facut;

    while (etapa_jucat != vector<bool>(n, true)) {

        progres_facut = false;

        for (int i = 0; i < n; i++) {

            if (i == index_echipa_manager || etapa_jucat[i])
                continue;

            for (int index = 0; index < n; index++) {

                if (index == index_echipa_manager || index == i || etapa_jucat[index])
                    continue;

                if (!ales[i][index]) {


                    ales[i][index] = true;
                    ales[index][i] = true;

                    etapa_jucat[i] = true;
                    etapa_jucat[index] = true;
                    progres_facut = true;

                    double factor_aleator_1 = (rand() % 11 - 5) * 0.1;
                    double factor_aleator_2 = (rand() % 11 - 5) * 0.1;

                    double scor1 = echipe[i]->get_overall() + factor_aleator_1;
                    double scor2 = echipe[index]->get_overall() + factor_aleator_2;

                    Echipeptr castigatoare = nullptr;

                    if (scor1 > scor2)
                        castigatoare = echipe[i];
                    else if (scor2 > scor1)
                        castigatoare = echipe[index];
                    else
                        castigatoare = (rand() % 2 == 0) ? echipe[i] : echipe[index];

                    if (castigatoare == echipe[i]) {
                        punctaje[i] += 3;
                        echipe[i]->adaugare_buget(5000);
                        echipe[index]->adaugare_buget(2000);
                    }
                    else{
                        punctaje[index] += 3;
                        echipe[i]->adaugare_buget(2000);
                        echipe[index]->adaugare_buget(5000);
                    }

                    break;
                }
            }
        }


        if (!progres_facut && etapa_jucat != vector<bool>(n, true)) {
             cout << "\nATENȚIE: S-au blocat meciurile AI. Etapa este incompletă. Nu se pot forma toate perechile. S-a jucat cu un număr impar de echipe AI.\n";
             break;
        }
    }


    cout << "S-a terminat o serie de partide";
}


void Liga::meciGUI() {
    int n = echipe.size();

    int index_echipa_manager = n - 1;

    int index_adversar = -1;

    while (true) {

        index_adversar = rand() % (n - 1);

        if (etapa_jucat[index_adversar] == true)
            continue;

        if (ales[index_echipa_manager][index_adversar] == true)
            continue;

        break;
    }

    ales[index_echipa_manager][index_adversar] = true;
    ales[index_adversar][index_echipa_manager] = true;
    etapa_jucat[index_adversar] = true;
    etapa_jucat[index_echipa_manager] = true;
}


void Liga::resetSeason() {
    int n = echipe.size();

    std::fill(punctaje.begin(), punctaje.end(), 0);

    for (int i = 0; i < n; ++i) {
        std::fill(ales[i].begin(), ales[i].end(), false);
        ales[i][i] = true;
    }

    std::fill(etapa_jucat.begin(), etapa_jucat.end(), false);

    cout << "\n============================================\n";
    cout << "SEZON NOU: Liga a fost resetata complet. Succes!\n";
    cout << "============================================\n";
}

void Liga::finalizeSeason(Echipeptr echipaMea) {
    if (!isSeasonFinished()) {
        cerr << "EROARE: Sezonul nu este terminat. Nu se poate finaliza liga.\n";
        return;
    }

    std::vector<long long> prizePool = {100000, 70000, 40000, 20000, 10000, 5000, 2000, 1000, 0, 0, 0, 0};

    std::vector<TeamScore> clasament = getSortedClasament();
    int managerRank = -1;
    long long finalPrize = 0;

    for (size_t i = 0; i < clasament.size(); ++i) {
        if (clasament[i].nume == echipaMea->getNume()) {
            managerRank = i + 1;

            if (i < prizePool.size()) {
                finalPrize = prizePool[i];
                echipaMea->adaugare_buget(finalPrize);
            }
            break;
        }
    }

    cout << "\n============================================\n";
    cout << "🎉 FINAL SEZON! 🎉\n";
    if (managerRank != -1) {
        cout << "Echipa ta (" << echipaMea->getNume() << ") a terminat pe locul " << managerRank << "!\n";
        cout << "Ai castigat un bonus de " << finalPrize << " lei!\n";
    } else {
         cerr << "Eroare: Echipa ta nu a fost gasita in clasament.\n";
    }
    cout << "============================================\n\n";

    resetSeason();
}


void Liga::registerMatchResult(Echipeptr echipaAdversa, Echipeptr castigator) {
    int n = echipe.size();
    int index_echipa_manager = n - 1;

    int index_adversar = -1;
    for (int i = 0; i < n; ++i) {
        if (echipe[i] == echipaAdversa) {
            index_adversar = i;
            break;
        }
    }

    if (index_adversar == -1 || index_adversar == index_echipa_manager) {
        std::cerr << "EROARE: Adversar invalid in registerMatchResult." << std::endl;
        return;
    }

    ales[index_echipa_manager][index_adversar] = true;
    ales[index_adversar][index_echipa_manager] = true;

    if (castigator == echipe[index_echipa_manager]) {
        std::cout << "Felicitari, echipa ta a castigat meciul, obtinand 3 puncte in clasament.\n";
        punctaje[index_echipa_manager] += 3;
        echipe[index_echipa_manager]->adaugare_buget(5000);
        echipe[index_adversar]->adaugare_buget(2000);
    } else {
        std::cout << "Din pacate ati pierdut meciul, nu ati obtinut niciun punct in clasament...\n";
        punctaje[index_adversar] += 3;
        echipe[index_echipa_manager]->adaugare_buget(5000);
        echipe[index_adversar]->adaugare_buget(2000);
    }
}



int Liga::liga() {



    for (const Echipeptr e : echipe)
        cout << *e;




    for (int i = 0; i < 11; i++) {
        string s;


        int index_echipa_manager = echipe.size() - 1;
        this->reset_etapa();


        echipe[index_echipa_manager]->construire_echipa_primii_6();

        Echipeptr castigator = this->meci();

        if (castigator->getNume() == echipe[index_echipa_manager]->getNume()) {
            cout<<"Felicitari, echipa dumneavoastra a castigat suma de 5000 de lei si 3 puncte in clasament.";



        }
        else {
            cout<<"Din pacate, echipa dumneavoastra a pierdut, insa tot ati castigat 2000 de lei";
        }

        this->meciuri();

        this->afisare_punctaje();

        while (s != "da") {
            cout<<"Cand doriti sa se contiune liga scrieti da.";
            cin>> s;
        }
    }


    vector<int> punctaje2;

    for (size_t i = 0; i < punctaje.size(); i++) {
        punctaje2.push_back(punctaje[i]);
    }


    for (size_t i = 0; i < punctaje2.size(); i++) {
        for (size_t j = i; j < punctaje2.size(); j++) {
            if (punctaje2[i] < punctaje2[j]) {
                swap(punctaje2[i], punctaje2[j]);
            }
        }
    }

    for (size_t i = 0; i < punctaje2.size(); i++) {
        if (punctaje2[i] == punctaje[11]) return i;
    }


}