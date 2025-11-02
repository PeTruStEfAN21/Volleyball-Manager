#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <ostream>

using namespace std;

class BazaDeDate;
class Echipe;

class jucator {
protected:
    string nume, pozitie;
    int ovr, spike_power, receive, spike_accuracy, serve_power, serve_accuracy, vertical_jump, mobility, speed, pret, height;
    bool ales;

    virtual void overall() {
        ovr = (spike_power + receive + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility + speed + height) / 9;
    }

    int set_val(int valoarea) const {
        if (valoarea < 0) valoarea = 0;
        if (valoarea > 99) valoarea = 99;
        return valoarea;
    }

    virtual void set_pret() {
        pret = ovr * 50;
    }

public:
    jucator(const string &nume, const string &pozitie, int ovr, int spike_power, int receive, int spike_accuracy,
            int serve_power, int serve_accuracy, int vertical_jump, int mobility, int speed, int pret, int height,
            bool ales)
        : nume(nume), pozitie(pozitie), ovr(ovr), spike_power(spike_power), receive(receive),
          spike_accuracy(spike_accuracy), serve_power(serve_power), serve_accuracy(serve_accuracy),
          vertical_jump(vertical_jump), mobility(mobility), speed(speed), pret(pret), height(height),
          ales(ales) {}

    virtual ~jucator() = default;

    jucator()
        : ovr(0), spike_power(0), receive(0), spike_accuracy(0), serve_power(0), serve_accuracy(0),
          vertical_jump(0), mobility(0), speed(0), pret(0), height(0), ales(false),
          nume("Necunoscut"), pozitie("necunoscut") {}

    void set_poz(const string &poz) { // Pass by const reference
        this->pozitie = poz;
    }

    virtual void valori(vector<int> a) {
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

    /*int get_pret() const { return pret; }*/
    //functie pentru transferuri...
    bool esteAles() const { return ales; }
    void setAles(bool val) { ales = val; }
    int get_ovr() const { return ovr; }

    virtual void afiseaza() const {
        cout << nume << " (" << pozitie << ") - OVR: " << ovr << ", Pret: " << pret << endl;
    }

    friend ostream &operator<<(ostream &out, const jucator &j) {
        out << "Nume: " << j.nume << ", Pozitie: " << j.pozitie << ", OVR: " << j.ovr;
        return out;
    }

    jucator &operator=(const jucator &j) {
        if (this != &j) {
            nume = j.nume;
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
        }
        return *this;
    }

    virtual void citeste(ifstream &in) {
        in >> nume;

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
    }

    /*string get_poz() {
        return pozitie;
    }*/
    //foarte util pentru depanare

    void set_nume(const string &nume) {
      this->nume = nume;
  }


    /*void get_nume() {
        cout<<nume<<endl;
    }*/
    //util pentru depanare

};


class OutsideHitter : public jucator {
    int explosiveness;


    public:
    explicit OutsideHitter(int explosiveness)
        : explosiveness(explosiveness) {
    }

    ~OutsideHitter() override = default;

    void valori(vector <int> a) override {
        jucator::valori(a);
        explosiveness = set_val(a[9]);
        overall();
    }

    OutsideHitter() {
        pozitie = "OutsideHitter";
        explosiveness = 0;
    }


    void overall() override{
        ovr = ((spike_power * 2 + receive * 2 + spike_accuracy * 2 + serve_power + serve_accuracy + vertical_jump * 2 +
                mobility + speed + explosiveness * 3 + height * 2) / 17);
    }

    void set_pret() override {
        pret = 10000 + ovr * 50 + spike_accuracy * 10 + spike_power * 10 + vertical_jump * 10 + explosiveness * 20 + receive * 10 + height * 10;;
    }

    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> explosiveness;
        explosiveness = set_val(explosiveness);
        overall();
        set_pret();
    }


};

class Setter : public jucator {
    int set_precision, set_iq;
    public:
    Setter(int set_precision, int set_iq)
        : set_precision(set_precision),
          set_iq(set_iq) {
    }

    Setter() {
        set_precision = set_iq = 0;
        pozitie = "Setter";
    }

    ~Setter() override = default;

    void valori(vector <int> a) override {
        jucator::valori(a);
        set_precision = set_val(a[9]);
        set_iq = set_val(a[10]);
        overall();
    }

    void overall() override {
        ovr = (spike_power + receive + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility * 2 +
               speed * 2 + set_precision * 3 + set_iq * 3 + height) / 17;
    }

    void set_pret() override {
        pret = 8000 + ovr * 50 + speed * 10 + set_precision * 20 + set_iq * 20 + mobility * 10;
    }

    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> set_iq;
        in>> set_precision;
        set_precision = set_val(set_precision);
        set_iq  = set_val(set_iq);
        overall();
        set_pret();
    }
};

class Libero : public jucator {
    int reflexes;
    public:
    explicit Libero(int reflexes)
        : reflexes(reflexes) {
    }

    Libero() {
        reflexes = 0;
        pozitie = "Libero";
    }

    ~Libero() override = default;


    void overall() override {
        ovr = (spike_power + receive * 3 + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility * 2 +
               speed * 3 + reflexes * 3 + height) / 17;
    }

    void set_pret() override {
        pret = 6000 + ovr * 50 + mobility * 10 + speed * 20 + reflexes * 20 + receive * 20;
    }

    void valori(vector <int> a) override {
        jucator::valori(a);
        reflexes = set_val(a[9]);
        overall();
    }
    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> reflexes;
        reflexes = set_val(reflexes);
        overall();
        set_pret();
    }
};

class MiddleBlocker : public jucator {
    int blocking;

    public:
    explicit MiddleBlocker(int blocking)
        : blocking(blocking) {
    }

    MiddleBlocker() {
        blocking = 0;
        pozitie = "MiddleBlocker";
    }

    ~MiddleBlocker() override = default;

    void overall() override {
        ovr = (spike_power + receive + spike_accuracy + serve_power + serve_accuracy + vertical_jump + mobility * 2 +
               speed * 2 + blocking * 3 + height * 4) / 17;
    }

    void set_pret() override {
        pret = 7000 + ovr * 50 + blocking * 20 + height * 30 + speed * 10 + mobility * 10;
    }

    void valori(vector <int> a) override {
        jucator::valori(a);
        blocking = set_val(a[9]);
        overall();
    }
    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> blocking;
        blocking = set_val(blocking);
        overall();
        set_pret();
    }

};

class OppositeHitter : public jucator {
    int explosiveness;

    public:
    explicit OppositeHitter(int explosiveness)
        : explosiveness(explosiveness) {
    }

    OppositeHitter() {
        explosiveness = 0;
        pozitie = "OppositeHitter";
    }

    // ADAUGAT: Destructor explicit
    ~OppositeHitter() override = default;

    void overall() override {
        ovr = ((spike_power * 2 + receive * 2 + spike_accuracy * 2 + serve_power + serve_accuracy + vertical_jump * 2 +
                mobility + speed + explosiveness * 3 + height * 2) / 17);
    }

    void set_pret() override {
        pret = 10000 + ovr * 50 + spike_accuracy * 10 + spike_power * 10 + vertical_jump * 10 + explosiveness * 20 + receive * 10 + height * 10;;
    }


    void valori(vector <int> a) override {
        jucator::valori(a);
        explosiveness = set_val(a[9]);
        overall();
    }
    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> explosiveness;
        explosiveness = set_val(explosiveness);
        overall();
        set_pret();
    }

};


class Echipe {
private:
    string nume;
   vector< jucator*> jucatori;
    float ovr;
    int punctaj, seturi;

    /*void citireNume() {
        cout << "Dati numele echipei: ";
        cin >> nume;
    }*/
    //posibil util mai tarziu

public:

    void set_overall() {
        float suma = 0;
        int j = 0;
        for (size_t i = 0; i < jucatori.size(); i++) {
            suma = suma + jucatori[i]->get_ovr();
            j++;
        }
        ovr = suma / j;
    }

    Echipe(const string &nume, const vector<jucator *> &jucatori, float ovr, int punctaj, int seturi)
        : nume(nume),
          jucatori(jucatori),
          ovr(ovr),
          punctaj(punctaj),
          seturi(seturi) {
    }

    Echipe(const Echipe &other)
        : nume(other.nume),
          jucatori(other.jucatori),
          ovr(other.ovr),
          punctaj(other.punctaj),
          seturi(other.seturi) {
    }

    Echipe & operator=(const Echipe &other) {
        if (this == &other)
            return *this;
        nume = other.nume;
        jucatori = other.jucatori;
        ovr = other.ovr;
        punctaj = other.punctaj;
        seturi = other.seturi;
        return *this;
    }

    explicit Echipe(const string& nume) {
        ovr = 0;
        this->nume = nume;
        punctaj = 0;
        seturi = 0;


    }

    Echipe() {
        ovr = 0;
        punctaj = 0;
        seturi = 0;
    }

    int get_overall() {
        float suma = 0;
        int j = 0;
        for (size_t i = 0; i < jucatori.size(); i++) {
            suma = suma + jucatori[i]->get_ovr();
            j++;
        }
        ovr = suma / j;
        return ovr;
    }

    void creare(const vector<jucator*>& lista) {
        int n = 0, index;
        cout << "\n--- Creare echipa " << nume << " ---\n";



        while (n < 6) {
            cout << "Alegeti jucatorul " << n+1 << " (1-" << lista.size() << "): ";
            cin >> index;

            if (index < 1 || index > (int)lista.size()) {
                cout << "Index invalid! Alegeti intre 1 si " << lista.size() << endl;
                continue;
            }

            jucator* jucatorAles = lista[index-1];

            if (jucatorAles->esteAles()) {
                cout << "Jucatorul a fost deja selectat! Alegeti altul.\n";
                continue;
            }

            jucatori.push_back(jucatorAles);
            jucatorAles->setAles(true); // marchez ca ales
            n++;
            cout << "Jucator adaugat cu succes!\n";
        }
    }

   /* void adaugare_jucator(vector<jucator*>& lista, int buget) {
        int index;
        cout << "\n--- Creare echipa " << nume << " ---\n";

            cout << "Alegeti un jucator din lista: ";
            cin >> index;

            if (index < 1 || index > (int)lista.size()) {
                cout << "Index invalid! Alegeti intre 1 si " << lista.size() << endl;
            }

            jucator* jucatorAles = lista[index-1];


            if (jucatorAles->esteAles()) {
                cout << "Jucatorul a fost deja selectat! Alegeti altul.\n";
            }

        if (buget < jucatorAles->get_pret())
            cout<< " Nu aveti destui bani pentru a contracta jucatorul ales";
        else {
            jucatori.push_back(jucatorAles);
            jucatorAles->setAles(true); // marchez ca ales
            cout << "Jucator adaugat cu succes!\n";
        }
        }
        */
    //functie pentru transferuri pe viitor

    friend ostream& operator<<(ostream& os, const Echipe& e) {
        os << "\n=== Echipa: " << e.nume << " ===\n";
        for (size_t i = 0; i < e.jucatori.size(); i++)
            os << "- " << *e.jucatori[i] << "\n";
        os << "Overall-ul echipei este: " << e.ovr << "\n";
        return os;
    }

    void adaugare_jucatori(const vector<jucator*>& lista) {
        for(auto j : lista){
            jucatori.push_back(j);
            j->setAles(true);
        }
    }

    const string& getNume() const { return nume; }


    int getPunctaj() {
        return punctaj;
    }

    int getSeturi() {
        return seturi;
    }

    void modifPunctaj() {
        punctaj++;
    }

    void modifSetur() {
        seturi++;
    }

    void resetPunctaj() {
        punctaj = 0;
    }

    void resetSetur() {
        seturi = 0;
    }

    void set_nume() {
        getline(cin, nume);
    }



};


class Meci {
protected:
    Echipe* echipa1;
    Echipe* echipa2;

    void tabelaScor() {
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

    char punct() {
        float sansa_a = pow((float)echipa1->get_overall(), 3.0f) /
                (pow((float)echipa1->get_overall(), 3.0f) + pow((float)echipa2->get_overall(), 3.0f));

        float r = (float)rand() / RAND_MAX;

        if (r < sansa_a)
            return 'a';
        else
            return 'b';
    }

    char set() {
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

                this_thread::sleep_for(chrono::seconds((1)));

        }

        if (echipa1->getPunctaj() >= 25 || echipa2->getPunctaj() >= 25) {
            while (abs(echipa1->getPunctaj() - echipa2->getPunctaj()) <= 1){
                char castigator = punct();

                if (castigator == 'a')
                    echipa1->modifPunctaj();
                else
                    echipa2->modifPunctaj();

                cout << string(50, '\n');
                tabelaScor();

                this_thread::sleep_for(chrono::seconds((1)));
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


public:
    Meci(Echipe* echipa1, Echipe* echipa2) : echipa1(echipa1), echipa2(echipa2) {
    }

    Meci() : echipa1(nullptr), echipa2(nullptr) {
    }


    Echipe* meci() {
        echipa1->resetSetur();
        echipa2->resetSetur();
        while (echipa1->getSeturi() < 3 && echipa2->getSeturi() < 3) {
            char castigator = set();
            if (castigator == 'a')
                echipa1->modifSetur();
            else
                echipa2->modifSetur();

            this_thread::sleep_for(chrono::seconds(2));
        }

        if (echipa1->getSeturi() == 3) {
            cout << "\nEchipa " << echipa1->getNume() << " a castigat meciul! Felicitari!\n";
            return echipa1;
        }
        else {
            cout << "\nEchipa " << echipa2->getNume() << " a castigat meciul! Felicitari!\n";
            return echipa2;
        }


    }
};


class BazaDeDate {
private:
    vector<jucator*> jucatori;
    vector<Echipe*> echipe_disponibile;

public:
    explicit BazaDeDate(const vector<jucator *> &jucatori)
        : jucatori(jucatori) {
    }

    BazaDeDate() = default;

    ~BazaDeDate() {
        for (auto j : jucatori)
            delete j;
    }

    void adaugaJucator(jucator* j) {
        jucatori.push_back(j);
    }

    void adaugaEchipe(Echipe* e) {
        echipe_disponibile.push_back(e);
    }

    void afiseazaJucatori() const {
        cout << "\n=== Lista jucatorilor inregistrati ===\n";
        for (size_t i = 0; i < jucatori.size(); i++) {
            cout << i + 1 << ". ";
            jucatori[i]->afiseaza();
        }
    }

    /*void afiseazaEchipe() const {
        cout << "\n=== Lista echipelor inregistrate ===\n";
        for (size_t i = 0; i < echipe_disponibile.size(); i++) {
            cout << i + 1 << ". ";
            cout<< *echipe_disponibile[i];
        }
    }*/
    //functie utila pentru depanare si pentru a vedea ce echipe au fost adaugate in baza intr un anumit punct

    /*size_t getNrJucatori() const { return jucatori.size(); }
    jucator* getJucator(size_t index) const { return jucatori[index]; } */
    //functii utile pentru depanare

    Echipe* getEchipe(size_t index) const { return echipe_disponibile[index]; }

    const vector<jucator*>& getLista() const { return jucatori; }

    const vector<Echipe*>& getListe() const { return echipe_disponibile; }

    Echipe* alege_echipa_random() {
        if (echipe_disponibile.empty()) return nullptr;

        int idx = rand() % echipe_disponibile.size();
        return echipe_disponibile[idx];
    }

    BazaDeDate(const BazaDeDate &other)
        : jucatori(other.jucatori),
          echipe_disponibile(other.echipe_disponibile) {
    }

    BazaDeDate & operator=(const BazaDeDate &other) {
        if (this == &other)
            return *this;
        jucatori = other.jucatori;
        echipe_disponibile = other.echipe_disponibile;
        return *this;
    }
};

class Liga : public Meci {
private:
    BazaDeDate* baza;
    vector<int> punctaje;
    int nrEchipe = 0;
    vector<vector<bool>> ales;
    vector<bool> etapa_jucat;



public:



    Echipe Liga::meci(Echipe* echipa) {
    auto lista = baza->getListe();
    int n = lista.size();
    int index = -1;
    int index_echipa_manager = n - 1; 

    while (true) {
       const Echipe* adversar = baza->alege_echipa_random();
        for (int i = 0; i < n; i++) {
            if (lista[i] == adversar) {
                index = i;
                break;
            }
        }

        if (index == -1 || index == index_echipa_manager)
            continue;

        if (ales[index_echipa_manager][index] == true || etapa_jucat[index] == true)
            continue;


        break;
    }

    ales[index_echipa_manager][index] = true;
    ales[index][index_echipa_manager] = true;
    etapa_jucat[index] = true;
    etapa_jucat[index_echipa_manager] = true;

    Meci meci_local(echipa, lista[index]);
    const Echipe* castigatoare = meci_local.meci();

    if (castigatoare == echipa) {
        cout << "Felicitari, echipa ta a castigat meciul, obtinand 3 puncte in clasament.\n";
        punctaje[index_echipa_manager] += 3;
    } else {
        cout << "Din pacate ati pierdut meciul, nu ati obtinut niciun punct in clasament...\n";
        punctaje[index] += 3;
    }

    return *castigatoare;

}


    Liga(BazaDeDate* baza, const vector<Echipe*>& echipe) : baza(baza) {
        int n = baza->getListe().size();
        ales = vector<vector<bool>>(n, vector<bool>(n, false));
        punctaje.resize(n, 0);
        for (int i = 0; i < n; i++) {
            ales[i][i] = true;
        }
    }


    void afisare_punctaje() {
        for (size_t i = 0; i < punctaje.size(); i++)
            cout << baza->getEchipe(i)->getNume() << ": " << punctaje[i] << "\n";
    }

   /* void afisare_punctaj() {
        cout << baza->getEchipe(baza->getListe().size() - 1)->getNume() << ": " << punctaje[baza->getListe().size() - 1] << "\n";
    }*/
    //functie pentru verificare punctaj

    void Liga::meciuri() {
    auto lista = baza->getListe();
    int n = lista.size();
    int index_echipa_manager = n - 1;

    if (punctaje.size() != n)
        punctaje.resize(n, 0);


    while (etapa_jucat != vector<bool>(n, true)) {
            bool progres_facut;


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

                    double scor1 = lista[i]->get_overall() + factor_aleator_1;
                    double scor2 = lista[index]->get_overall() + factor_aleator_2;

                    const Echipe* castigatoare;

                    if (scor1 > scor2)
                        castigatoare = lista[i];
                    else if (scor2 > scor1)
                        castigatoare = lista[index];
                    else
                        castigatoare = (rand() % 2 == 0) ? lista[i] : lista[index];

                    if (castigatoare == lista[i])
                        punctaje[i] += 3;
                    else
                        punctaje[index] += 3;

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
};


class manageri {
private:
    int buget;
    Echipe* echipa;
    string nume;
    vector<jucator*> lista;
    BazaDeDate* baza;

public:

    Echipe* get_echipa() const {
        return echipa;
    }

    manageri(int buget, Echipe *echipa, const string &nume)
        : buget(buget), echipa(echipa), nume(nume), baza(nullptr), lista({}) {
    }

    /*void alegere_jucator(int buget) {
        buget = this->buget;
        echipa->adaugare_jucator(lista,buget);
    }*/
    //functie utila pentru tranferuri in viitor

    void set_overall() {
        echipa->Echipe::set_overall();
    }

    manageri() : buget(0), echipa(new Echipe()), nume("Necunoscut"), lista({}), baza(nullptr) {
    }

    /*void cariera() {
        cout<< "Dati un nume pentru propria voastra echipa";
        cin>> nume;
        buget = 10000;

        while (true) {
            int k = 0;
            cout<<"Alegeti ce doriti sa faceti in continuare(Scrieti doar numarul optiunii alese):\n"
            <<"1. Jucati un meci amical.(Castigati o suma mica de bani)\n"
            <<"2. Jucati un meci in liga.(Trebuie sa platiti fiecare jucator o suma pentru meci, dar castigati o suma bazata pe performanta)\n"
            <<"3. Intrati intr-un turneu local.(Trebuie sa platiti atat pentru intrarea in turneu, cat si jucatorii pentru fiecare meci, dar aveti sansa sa castigati o suma mare daca castigati)\n"
            <<"4. Faceti un antrenament cu toata echipa(scade performanta pentru urmatorul joc, dar creste permanent overall-ul fiecarui jucator)\n"
            <<"5. Luati o zi libera.\n"
            <<"6. Angajati un jucator.\n"
            <<"7. Iesi la pensie.";

            cin>>k;
            if (k == 1) {
                Echipe* adversar = baza->alege_echipa_random();
                if (adversar != nullptr) {
                    Meci meci(echipa, adversar);
                    meci.meci();
                    buget += 5000;

                }


            }

            else if (k == 2) {

            }
        }

    }*/
    //functie pentru a simula cariera utila pentru viitor

    friend ostream & operator<<(ostream &os, const manageri &obj) {
        return os
               << "buget: " << obj.buget
               << " echipa: " << obj.echipa
               << " nume: " << obj.nume
               << " jucator: " << &obj.lista;
    }

    void alegere_echipa() {
        baza->afiseazaJucatori();
        echipa->creare(baza->getLista());
    }

    void numeEchipa() {
        cout<<"Dati numele echipei voastre:\n";
        echipa->set_nume();
    }

    string get_nume() const {
      return  echipa->getNume();
    }

    void adaugare_jucatori_valabili(BazaDeDate* baza) {
        this->baza = baza;
    }

    manageri(const manageri& other)
        : buget(other.buget), echipa(new Echipe(*other.echipa)), nume(other.nume), lista(other.lista), baza(other.baza) {}

    manageri& operator=(const manageri& other) {
        if (this != &other) {
            delete echipa;
            buget = other.buget; nume = other.nume; lista = other.lista; baza = other.baza;
            echipa = new Echipe(*other.echipa);
        }
        return *this;
    }

    ~manageri() { }
};




int main() {
    srand(time(0));
    BazaDeDate baza, baza_jucatori;

    manageri manager;

    manager.numeEchipa();

    vector<Echipe*> echipe;
    vector<string> numeEchipe = {"Dinamo","Rapid","CFR","UCluj","Farul",
                                 "Petrolul","UTA","FCSB","Voluntari","Botosani","Otelul"};


    for (const auto &nume : numeEchipe)
        echipe.push_back(new Echipe(nume));


    ifstream fin("text.txt");
    if (!fin) {
        cerr << "Nu s-a putut deschide fisierul text.txt\n";
        return 1;
    }

    for (auto& echipa : echipe) {
        vector<jucator*> jucatoriEchipa;

        string pozitie, nume;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
        jucator* j = nullptr;
        int i = 0;
        while(i < 6) {
            if (!(fin >> pozitie)) break;
            vector<int> a;
            fin>>nume;

            if (pozitie == "Libero") {
                j = new Libero();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie == "Setter") {
                j = new Setter();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
                j->valori(a);
            }
            else if (pozitie == "OutsideHitter") {
                j = new OutsideHitter();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie == "OppositeHitter") {
                j = new OppositeHitter();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie == "MiddleBlocker") {
                j = new MiddleBlocker();
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else continue;
            j->set_nume(nume);
            j->set_poz(pozitie);

            cout<<*j<<endl;

            jucatoriEchipa.push_back(j);
            baza.adaugaJucator(j);
            i++;
        }
        echipa->adaugare_jucatori(jucatoriEchipa);
        jucatoriEchipa.clear();
        echipa->get_overall();
        baza.adaugaEchipe(echipa);
    }
    fin.close();


    ifstream finn("tastatura.txt");
    if (!finn) {
        cerr << "Nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    while(true) {
        string pozitie, nume;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
        jucator* j = nullptr;
        if (!(finn >> pozitie)) break;
        vector<int> a;
        finn>>nume;

        if (pozitie == "Libero") {
            j = new Libero();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "Setter") {
            int a11;
            j = new Setter();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
            j->valori(a);
        }
        else if (pozitie == "OutsideHitter") {
            j = new OutsideHitter();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "OppositeHitter") {
            j = new OppositeHitter();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie == "MiddleBlocker") {
            j = new MiddleBlocker();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else continue;
        j->set_nume(nume);
        j->set_poz(pozitie);

       baza_jucatori.adaugaJucator(j);


    }
    finn.close();
    numeEchipe.push_back( manager.get_nume());

    cout << "S-au citit toate echipele cu jucatorii.\n";

    manager.adaugare_jucatori_valabili(&baza_jucatori);
    manager.alegere_echipa();
    manager.set_overall();
    baza.adaugaEchipe(manager.get_echipa());

    echipe.push_back( manager.get_echipa());

    for (const Echipe* e : echipe)
        cout << *e;

    Liga liga(&baza,echipe);

    for (int i = 0; i < 12; i++) {
        string s;

        int index_echipa_manager = echipe.size() - 1;

        liga.meci(echipe[index_echipa_manager]);

            liga.meciuri();

            liga.afisare_punctaje();

            cout<<"Doriti sa se contiune liga?(da/nu)";
            cin>> s;

            if (s == "da")
                continue;
            else
                break;
        }

    for (auto &e : echipe)
        delete e;

    return 0;
}
