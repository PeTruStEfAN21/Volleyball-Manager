#include "Meci.h"
#include "Echipe.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;


Meci::Meci(Echipeptr echipa1, Echipeptr echipa2) : echipa1(echipa1), echipa2(echipa2) {
}

Meci::Meci() : echipa1(nullptr), echipa2(nullptr) {
}


void Meci::tabelaScor() {
    cout << "_____________________________\n"
         << "|       Tabela de scor       |\n"
         << "-----------------------------\n"
         << "|" << echipa1->getNume() << "  |  " << echipa2->getNume() << "|\n"
         << "-----------------------------\n"
         << "| Punctaj | " << echipa1->getPunctaj() << "  |  " << echipa2->getPunctaj() << " |\n"
         << "-----------------------------\n"
         << "| Seturi  | " << echipa1->getSeturi()  << "  |  " << echipa2->getSeturi()  << " |\n"
         << "-----------------------------\n";
}


float Meci::calculeaza_scor_echipa_ponderea_activa(Echipeptr echipa) const {
    if (!echipa) return 0.0f;

    float impact_total = 0.0f;
    const auto& titulari = echipa->get_jucatori_pe_teren();

    if (titulari.size() != 6) {
        return (float)echipa->get_overall();
    }

    for (const auto& titular : titulari) {
        if (titular) {
            impact_total += titular->obtine_scor_tactica_final();
        }
    }

    return impact_total / 6.0f;
}


char Meci::punct() {

    float scor_ponderat_a = calculeaza_scor_echipa_ponderea_activa(echipa1);
    float scor_ponderat_b = calculeaza_scor_echipa_ponderea_activa(echipa2);

    if (scor_ponderat_a < 10.0f) scor_ponderat_a = 10.0f;
    if (scor_ponderat_b < 10.0f) scor_ponderat_b = 10.0f;


    float sansa_a = pow(scor_ponderat_a, 2.5f) /
            (pow(scor_ponderat_a, 2.5f) + pow(scor_ponderat_b, 2.5f));

    float r = (float)rand() / RAND_MAX;

    if (r < sansa_a)
        return 'a';
    else
        return 'b';
}

char Meci::set() {
    echipa1->resetPunctaj();
    echipa2->resetPunctaj();

    while (echipa1->getPunctaj() < 25 && echipa2->getPunctaj() < 25) {
            char castigator = punct();

            if (castigator == 'a')
                echipa1->modifPunctaj();
            else
                echipa2->modifPunctaj();

            cout << string(50, '\n');
            tabelaScor();

         //   this_thread::sleep_for(chrono::seconds((1)));

    }

    if (echipa1->getPunctaj() >= 25 || echipa2->getPunctaj() >= 25) {
        // Tie-break (diferență de 2 puncte)
        while (abs(echipa1->getPunctaj() - echipa2->getPunctaj()) <= 1){
            char castigator = punct();

            if (castigator == 'a')
                echipa1->modifPunctaj();
            else
                echipa2->modifPunctaj();

            cout << string(50, '\n');
            tabelaScor();

          //  this_thread::sleep_for(chrono::seconds((1)));
        }
    }

    if (echipa1->getPunctaj() > echipa2->getPunctaj()) {
        cout << "\nSET CASTIGAT DE " << echipa1->getNume() << "!\n";
        return 'a';
    } else
    {
        cout << "\nSET CASTIGAT DE " << echipa2->getNume() << "!\n";
        return 'b';
    }
}


Echipeptr Meci::meci() {
    cout<<"Incepe meciul intre echipa "<<echipa1->getNume()<<" si echipa "<<echipa2->getNume()<<endl;
    echipa1->resetSetur();
    echipa2->resetSetur();
    this_thread::sleep_for(chrono::seconds((5)));
    while (echipa1->getSeturi() < 1 && echipa2->getSeturi() < 1) {
        char castigator = set();
        if (castigator == 'a')
            echipa1->modifSetur();
        else
            echipa2->modifSetur();

      //  this_thread::sleep_for(chrono::seconds(2));
    }

    if (echipa1->getSeturi() == 1) {
        cout << "\nEchipa " << echipa1->getNume() << " a castigat meciul! Felicitari!\n";
        return echipa1;
    }
    else {
        cout << "\nEchipa " << echipa2->getNume() << " a castigat meciul! Felicitari!\n";
        return echipa2;
    }
}


void Meci::simuleazaUrmatoareActiune() {
    while (echipa1->getPunctaj() < 25 && echipa2->getPunctaj() < 25) {
        char castigator = punct();

        if (castigator == 'a')
            echipa1->modifPunctaj();
        else
            echipa2->modifPunctaj();


       // this_thread::sleep_for(chrono::seconds((1)));

    }

    if (echipa1->getPunctaj() >= 25 || echipa2->getPunctaj() >= 25) {
        // Tie-break (diferență de 2 puncte)
        while (abs(echipa1->getPunctaj() - echipa2->getPunctaj()) <= 1){
            char castigator = punct();

            if (castigator == 'a')
                echipa1->modifPunctaj();
            else
                echipa2->modifPunctaj();


           // this_thread::sleep_for(chrono::seconds((1)));
        }
    }


}