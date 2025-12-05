#include <iostream>
#include <string>
#include <vector>
#include "Transferuri.h"
#include "Echipe.h"
#include "BazaDeDate.h"
#include "jucator.h"

using namespace std;



Transferuri::Transferuri():echipa(nullptr),
                            baza(nullptr)            
{
}

Transferuri::~Transferuri() {
}

Transferuri::Transferuri(BazaDeDateptr baza, Echipeptr echipa) : baza(baza),
                                                                echipa(echipa)
{
}



// cppcheck-suppress unusedFunction
void Transferuri::transfer_manager() {
        std::string s;
        int index = 0;
        while (1){
            cout<<"Alegeti daca doriti sa vindeti sau sa cumparati un jucator sau daca doriti sa iesiti. Scrieti v(vindeti), c(cumparati) sau i(iesire). Atentie!!! Jucatorii se vand la 90% din pretul lor real pe piata.\n";
            cin>> s;
            if (s != "v" && s != "c" && s != "i") {
                cout<<"Scrieti una din cele 3 litere mentionate\n";
                continue;
            }

            if (s == "i")
                break;

            if (s == "v") {
                if (echipa->get_nr_jucatori() < 7) {
                    cout<<"Nu poti vinde niciun jucator deoarece trebuie sa ai mai mult de 6 jucaatori pentru a completa aceasta actiune.\n";
                    continue;
                }
                cout<<"Alegeti ce jucator doriti sa vindeti din lista:\n";
                cout<<echipa;

                cout<<"Specificati indexul jucatorului dorit:\n";
                cin>>index;
                index--;

                if (index < 0 || index >= (int)echipa->get_nr_jucatori()) {
                    cout << "Index invalid! Jucatorul nu exista in echipa dumneavoastra.\n";
                    continue;
                }

                echipa->get_jucator(index)->transferabil(true);
                float pret = echipa->pret_jucator(index);
                echipa->adaugare_buget(pret - pret / 10);
                echipa->vinde(echipa->get_jucator(index));

                cout<<"Felicitari, ati vandut jucatorul ales, castigand "<< pret - pret / 10<<" bani.\n";


            }


           if (s == "c") {
                cout << "Alegeti ce jucator vreti sa cumparati din lista: \n";
                baza->afisare_jucatori_necontractati();

                cout << "Specificati indexul jucatorului dorit (din lista bazei de date): ";

                if (!(cin >> index)) {
                    cout << "Input invalid! Va rog introduceti un numar.\n";
                    cin.clear();
                    continue;
                }

               index--;


                jucatorptr jucatorAles = baza->getJucator(index);

                if (!jucatorAles) {
                    cout << "Index invalid! Jucatorul nu exista in baza de date.\n";
                    continue;
                }

                Echipeptr echipaSursa = jucatorAles->get_echipe();

                float pretBaza = jucatorAles->get_pret();
                float pretTranzactie = pretBaza * 1.5;

                if (echipaSursa == nullptr) {
                    pretTranzactie = pretBaza;
                    cout << "Jucatorul este liber. Pretul este " << pretTranzactie << " lei.\n";
                }
                else if (echipaSursa == echipa) {
                    cout << "Jucatorul este deja in echipa dumneavoastra!\n";
                    continue;
                }
                else {
                    cout << "Jucatorul provine de la echipa " << echipaSursa->getNume()
                         << ". Pretul de transfer este " << pretTranzactie << " lei.\n";
                }


                if (pretTranzactie > echipa->get_buget()) {
                    cout << "Nu aveti bugetul necesar (" << pretTranzactie << " lei) pentru a cumpara acest jucator. Aveti doar " << echipa->get_buget() << " lei.\n";
                    continue;
                }

               if (jucatorAles->get_transferabil() == false) {
                   cout<<"Jucatorul nu este disponibil.\n";
                   continue;
               }


                if (echipaSursa != nullptr) {

                    echipaSursa->adaugare_buget(pretTranzactie);


                    echipaSursa->eliminaJucator(jucatorAles);
                }

                echipa->adaugare_buget(-pretTranzactie);

                echipa->adaugaJucatorExistent(jucatorAles);

                jucatorAles->adaugare_echipe(echipa);
                jucatorAles->setAles(true);

                cout << "Jucatorul " << jucatorAles->get_nume() << " a fost adaugat cu succes!\n";
                cout << "Mai aveti " << echipa->get_buget() << " lei in buget.\n";
            }
        }
    }

// cppcheck-suppress unusedFunction
void Transferuri::transferuri_ai() {
    cout<<" a inceput!\n";
    bool valid = true;
    for (auto  e : baza->getListe()) {
        cout<<"echipa\n";
        if (e->get_nr_jucatori() < 7 )
            valid = false;
        int min = INT_MAX;
        jucatorptr veriga_slaba = nullptr;



        for (auto j : e->get_jucatori()) {
            if (j->get_ovr() < min) {
                min = j->get_ovr();
                veriga_slaba = j;
            }
        }
        cout<<" a mers partial\n";


        if (!veriga_slaba) continue;

        string pozitie_veriga_slaba = veriga_slaba->get_poz();
        jucatorptr jucatorForta = nullptr;


        for (auto j : baza->getLista()) {
            if ((j->get_echipe() == nullptr || jucatorForta == nullptr || j->get_ovr() > jucatorForta->get_ovr()) && j->get_pret() < e->get_buget() && j->get_transferabil() == true && j->get_poz() == pozitie_veriga_slaba)
                jucatorForta = j;
        }

        cout<<" a mers\n";

        if (!jucatorForta) continue;

        if (jucatorForta->get_echipe() != nullptr)
            jucatorForta->get_echipe()->adaugare_buget(jucatorForta->get_pret());
        jucatorForta->adaugare_echipe(e);
        jucatorForta->setAles(true);
       if (valid) e->adaugare_buget(veriga_slaba->get_pret() - veriga_slaba->get_pret() / 10);
        e->adaugare_buget(-jucatorForta->get_pret());
        if (valid)e->vinde(veriga_slaba);
        cout<<"Echipa "<< e<< " a vandut jucatorul "<< veriga_slaba->get_nume() << " pe piata de transferuri, pentru a cumpara jucatorul "<<jucatorForta->get_nume()<<"\n";


    }
}