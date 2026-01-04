#include "manageri.h"

#include <chrono>
#include "Liga.h"
#include "Echipe.h"
#include "BazaDeDate.h"
#include "jucator.h"
#include "Meci.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>

#include "Transferuri.h"

using namespace std;

//Design Pattern

manageri& manageri::getInstance() {
    static manageri instance;
    return instance;
}


manageri::manageri()
    :  nume("Necunoscut"),
       lista({}),
       baza(make_shared<BazaDeDate>()),
       echipa(make_shared<Echipe>()){
    this->echipa->set_buget(250000);
    transferuri_manager = make_shared<Transferuri>(baza, this->echipa);

    this->jucatorgratis2 = make_shared<Setter>();

    this->jucatorgratis1 = make_shared<OutsideHitter>();

    jucatorgratis1->setOvr(78);
    jucatorgratis2->setOvr(75);

    this->echipa->get_jucatori().push_back(jucatorgratis1);
    this->echipa->get_jucatori().push_back(jucatorgratis2);
}


manageri::manageri(const manageri& other)
    :  nume(other.nume),
       lista(other.lista),
       baza(other.baza),
       echipa(make_shared<Echipe>(*other.echipa))
{

}



int manageri::get_buget() const {
    if (!echipa) return 0;
    return echipa->get_buget();
}

void manageri::set_buget(int buget) {
    if (echipa) echipa->set_buget(buget);
}

manageri& manageri::operator=(const manageri& other) {
    if (this != &other) {

        nume = other.nume;
        lista = other.lista;
        baza = other.baza;

        echipa = make_shared<Echipe>(*other.echipa);
        transferuri_manager = other.transferuri_manager;
    }
    return *this;
}

manageri::~manageri() {
}

// cppcheck-suppress unusedFunction
Echipeptr manageri::get_echipa() const {
    return echipa;
}

void manageri::set_overall() {
    if (echipa) echipa->set_overall();
}

void manageri::alegere_echipa() {
    if (!baza || !echipa) return;
    baza->afiseazaJucatori();
    echipa->creare(baza->getLista());
}

// cppcheck-suppress unusedFunction
void manageri::numeEchipa() {
    if (!echipa) return;
    cout<<"Dati numele echipei voastre:\n";
    echipa->set_nume();
}
    

string manageri::get_nume() const {
    if (!echipa) return "Echipa Invalida";
    return echipa->getNume();
}


// cppcheck-suppress unusedFunction
void manageri::adaugare_jucatori_valabili(BazaDeDateptr bazalocal) {
    this->baza = bazalocal;
}



ostream & operator<<(ostream &os, const manageri &obj) {
    os << "buget: " << obj.get_buget() << "\n";
    if (obj.echipa) os << "echipa: " << *obj.echipa << "\n";
    os << "nume manager: " << obj.nume << "\n";
    os << "adresa lista: " << &obj.lista;
    return os;
}


// cppcheck-suppress unusedFunction
void manageri::schimba_ovr(float ovr) {
    if (echipa) this->echipa->set_overall(ovr);
}




int manageri::citire_toti_jucatorii_si_echipe(){

    vector<Echipeptr> echipe_ai;
    vector<string> numeEchipe = {"Dinamo","Rapid","CFR","UCluj","Farul",
                                 "Petrolul","UTA","FCSB","Voluntari","Botosani","Otelul", "Arcada", "Barca","Zenit Kazan", "Cucine Lube Civitanova",
                                    "Sir Safety Conad Perugia", "Sada Cruzeiro", "Halkbank Ankara", "ZAKSA", "HakifBank", "Unibuc", "Ase"};

    for (const auto& nume_echipa_curenta : numeEchipe) {
        cout<<nume_echipa_curenta<<endl;
        echipe_ai.push_back(make_shared<Echipe>(nume_echipa_curenta));
    }

     ifstream fin("text.txt");
    if (!fin) {
        cerr << "Nu s-a putut deschide fisierul text.txt\n";
        return 1;
    }

    for (auto& e : echipe_ai) {
        string pozitie, numeCitit;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
        jucatorptr j = nullptr;
        int i = 0;
        while(i < 6) {
            if (!(fin >> pozitie)) {
                cout<<"s a blocat aici\n";
                break;
            }
            vector<int> a;
            fin>>numeCitit;

            if (pozitie == "Libero") {
                j = make_shared<Libero>();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie == "Setter") {
                j = make_shared<Setter>() ;
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
                j->valori(a);
            }
            else if (pozitie == "OutsideHitter") {
                j = make_shared<OutsideHitter>() ;
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie == "OppositeHitter") {
                j = make_shared<OppositeHitter>();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie == "MiddleBlocker") {
                j = make_shared<MiddleBlocker>() ;
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else {
                if (fin.fail()) {
                    fin.clear();
                }
                break;
            }

            j->set_nume(numeCitit);
            j->set_poz(pozitie);
            j->transferabil(false);
            j->adaugare_echipe(e);

            e->adaugaJucatorExistent(j);
            baza->adaugaJucator(j);
          //  baza->adaugaJucator(std::dynamic_pointer_cast<jucator>(j->clone()));
            i++;
        }
        e->get_overall();
        baza->adaugaEchipe(e);

    }
    fin.close();


    for (const Echipeptr& e : echipe_ai)
        cout << *e;

    return 0;
}



// cppcheck-suppress unusedFunction
BazaDeDateptr manageri::get_baza_manager() {
    return baza;
}




int manageri::citire_baza_managerGUI() {
     ifstream finn("tastatura.txt");
    if (!finn) {
        cerr << "Nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    while(true) {
        string pozitie, numeCitit;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
        jucatorptr j = nullptr;
        if (!(finn >> pozitie)) break;
        vector<int> a;
        finn>>numeCitit;

        if (pozitie == "Libero") {
            j = make_shared <Libero>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "Setter") {
            int a11;
            j = make_shared <Setter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
            j->valori(a);
        }
        else if (pozitie == "OutsideHitter") {
            j = make_shared <OutsideHitter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "OppositeHitter") {
            j = make_shared <OppositeHitter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "MiddleBlocker") {
            j = make_shared <MiddleBlocker>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else continue;
        j->set_nume(numeCitit);
        j->set_poz(pozitie);

       baza->adaugaJucator(j);


    }
    finn.close();
    return 0;
}

// cppcheck-suppress unusedFunction
int manageri::citire_baza_manager() {
     ifstream finn("tastatura.txt");
    if (!finn) {
        cerr << "Nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    while(true) {
        string pozitie, numeCitit;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
        jucatorptr j = nullptr;
        if (!(finn >> pozitie)) break;
        vector<int> a;
        finn>>numeCitit;

        if (pozitie == "Libero") {
            j = make_shared <Libero>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "Setter") {
            int a11;
            j = make_shared <Setter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
            j->valori(a);
        }
        else if (pozitie == "OutsideHitter") {
            j = make_shared <OutsideHitter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "OppositeHitter") {
            j = make_shared <OppositeHitter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "MiddleBlocker") {
            j = make_shared <MiddleBlocker>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else continue;
        j->set_nume(numeCitit);
        j->set_poz(pozitie);

       baza->adaugaJucator(j);
        cout<<*j<<endl;



    }
    finn.close();

    alegere_echipa();
    manageri::set_overall();

    cout<<"Aceasta este echipa pe care ati ales-o:\n"<< *echipa<<endl;

    return 0;

}


// cppcheck-suppress unusedFunction
int manageri::citire_jucatori_liberi() {
    ifstream fin("jucatori.txt");
    if (!fin) {
        cerr << "Nu s-a putut deschide fisierul jucatori.txt\n";
        return 1;
    }

     while(true) {
        string pozitie, numeCitit;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
        jucatorptr j = nullptr;
        if (!(fin >> pozitie)) break;
        vector<int> a;
        fin>>numeCitit;

        if (pozitie == "Libero") {
            j = make_shared <Libero>();
            fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "Setter") {
            int a11;
            j = make_shared <Setter>();
            fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a5); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
            j->valori(a);
        }
        else if (pozitie == "OutsideHitter") {
            j = make_shared <OutsideHitter>();
            fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a5); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "OppositeHitter") {
            j = make_shared <OppositeHitter>();
            fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a5); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "MiddleBlocker") {
            j = make_shared <MiddleBlocker>();
            fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a5); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else continue;
        j->set_nume(numeCitit);
        j->set_poz(pozitie);

       baza->adaugaJucator(j);



    }
    fin.close();

    cout<<"S-au citit jucatorii fara contract.\n";
    return 0;

}



BazaDeDateptr manageri::getBazaDeDate() {
    return baza;
}


Echipeptr manageri::getEchipaManager() {
    return echipa;
}


void manageri::meci_amical() {
    if (!echipa || !baza) return;

    Echipeptr adversar = baza->alege_echipa_random();
    if (!adversar) {
         cout<<"Nu sunt echipe adverse disponibile.\n";
         return;
    }

    Meci meci_amical(echipa, adversar);
    meci_amical.meci();

    cout<<"Felicitari, ati primit suma de 20000 de lei.";
    echipa->adaugare_buget(20000);
}


// cppcheck-suppress unusedFunction
int manageri::cariera() {
    string pref;
    if (!echipa || !baza) {
         cerr << "Eroare: Echipa sau Baza de Date nu este initializata corect.\n";
         return 1;
    }

    cout<<"Felicitari! A inceput cariera dumneavoastra de manager al unei echipe de volei!\n"
    <<"Primul pas este sa imi spuneti numele dumneavoastra:\n";
    cin>> this->nume;
    cout<<"Acum va rog sa imi spuneti cum vreti sa se cheme echipa dumneavoastra:\n";
    echipa->set_nume();
    cout<<"Urmatorul pas este sa-mi spuneti echipa preferata a dumneavoastra:\n";
    cin>> pref;

    if (pref == "dinamo" || pref == "Dinamo") {
        cout<<"Of, tocmai ati facut jocul mai greu...\n";
        echipa->set_buget(150000);
    }
    else if (pref == "steaua" || pref == "Steaua") {
        cout<<"Felicitari! Ati ales cea mai buna echipa!\n";
        echipa->set_buget(350000);
    }
    else {
        cout<<"Buna alegere...\n";
        echipa->set_buget(250000);
    }

    this_thread::sleep_for(chrono::seconds(5));

    cout<<"Acum este timpul sa va alegeti echipa! La inceput aceasta va fi formata din 6 jucatori pentru a putea juca primele meciuri!";

    this_thread::sleep_for(chrono::seconds(1));

    citire_baza_manager();
    citire_toti_jucatorii_si_echipe();
    citire_jucatori_liberi();

    this_thread::sleep_for(chrono::seconds(10));


    cout<<"Acum veti vedea cum decurge un meci, urmarind scorul echipei dumneavoastra.\n";


    meci_amical();


    this_thread::sleep_for(chrono::seconds(5));


    cout<<"Pentru fiecare meci amical veti primii o suma de bani, pe care ii puteti folosi pentru a alege alti jucatori mai buni din lista de jucatori.\n"
    <<"Acum aveti 3 optiuni, puteti alege un jucator nou din lista daca aveti destui bani, puteti juca alt meci amical sau puteti intra intr-o liga(daca vreti sa intrati in liga nu mai puteti lua alt jucator sau juca meciuri amicale.\n";


    while (1) {
        cout<<endl
            <<"1. Meci amical.\n"
            <<"2. Liga.\n"
            <<"3. Deschideti lista de transferuri.\n"
            <<"4. Terminare joc.\n";

        cin>>pref;
        if (pref == "1") {
            echipa->construire_echipa_primii_6();
            meci_amical();
            transferuri_manager->transferuri_ai();
            this_thread::sleep_for(chrono::seconds(15));
        }
        else if (pref == "2") {
            Liga liga(baza);
            liga.creare_liga();
            liga.adaugare_echipa(echipa);
            int n = liga.liga();
            cout<<"Echipa dumneavoastra a iesit pe locul "<< n + 1<< "in liga, castigand astfel "<<(12 - n) * 1000<< " lei. Felicitari!\n";
            echipa->adaugare_buget(( 12 - n ) * 1000 );
            transferuri_manager->transferuri_ai();
            this_thread::sleep_for(chrono::seconds(15));

        }
        else if (pref == "3") {

            transferuri_manager->transfer_manager();
            this_thread::sleep_for(chrono::seconds(15));
        }
        else if (pref == "4") {
            break;
        }
        else {
            continue;
        }

    }

return 0;
}




