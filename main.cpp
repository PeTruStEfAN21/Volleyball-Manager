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
#include <memory>

using namespace std;

class BazaDeDate;
class Echipe;
class Meci; 


class jucator {
protected:
    string nume, pozitie;
    int ovr, spike_power, receive, spike_accuracy, serve_power, serve_accuracy, vertical_jump, mobility, speed, pret, height;
    bool ales;

    virtual void overall() {
        this->ovr = (this->spike_power + this->receive + this->spike_accuracy + this->serve_power + this->serve_accuracy + this->vertical_jump + this->mobility + this->speed + this->height) / 9;
    }

    int set_val(int valoarea) const {
        if (valoarea < 0) valoarea = 0;
        if (valoarea > 99) valoarea = 99;
        return valoarea;
    }

    virtual void set_pret() {
        this->pret = this->ovr * 50;
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
        :nume("Necunoscut"), pozitie("necunoscut"), ovr(0), spike_power(0), receive(0), spike_accuracy(0), serve_power(0), serve_accuracy(0),
          vertical_jump(0), mobility(0), speed(0), pret(0), height(0), ales(false) {}


    void set_poz(const string &poz) { 
        this->pozitie = poz;
    }

    virtual void valori(vector<int> a) {
        this->spike_power = set_val(a[0]);
        this->receive = set_val(a[1]);
        this->spike_accuracy = set_val(a[2]);
        this->serve_power = set_val(a[3]);
        this->serve_accuracy = set_val(a[4]);
        this->vertical_jump = set_val(a[5]);
        this->mobility = set_val(a[6]);
        this->height = set_val(a[7]);
        this->speed = set_val(a[8]);
    }

    bool esteAles() const { return this->ales; }
    void setAles(bool val) { this->ales = val; }
    int get_ovr() const { return this->ovr; }

    virtual void afiseaza() const {
        cout << this->nume << " (" << this->pozitie << ") - OVR: " << this->ovr << ", Pret: " << this->pret << endl;
    }

    friend ostream &operator<<(ostream &out, const jucator &j) {
        out << "Nume: " << j.nume << ", Pozitie: " << j.pozitie << ", OVR: " << j.ovr;
        return out;
    }

    jucator &operator=(const jucator &j) {
        if (this != &j) {
            this->nume = j.nume;
            this->ovr = j.ovr;
            this->spike_power = j.spike_power;
            this->height = j.height;
            this->receive = j.receive;
            this->spike_accuracy = j.spike_accuracy;
            this->serve_power = j.serve_power;
            this->serve_accuracy = j.serve_accuracy;
            this->vertical_jump = j.vertical_jump;
            this->mobility = j.mobility;
            this->speed = j.speed;
            this->pret = j.pret;
            this->pozitie = j.pozitie;
        }
        return *this;
    }

    virtual void citeste(ifstream &in) {
        in >> this->nume;

        in >> this->spike_power;
        in >> this->receive;
        in >> this->spike_accuracy;
        in >> this->serve_power;
        in >> this->serve_accuracy;
        in >> this->vertical_jump;
        in >> this->mobility;
        in >> this->speed;
        in >> this->height;

        this->spike_power = set_val(this->spike_power);
        this->receive = set_val(this->receive);
        this->spike_accuracy = set_val(this->spike_accuracy);
        this->serve_power = set_val(this->serve_power);
        this->serve_accuracy = set_val(this->serve_accuracy);
        this->vertical_jump = set_val(this->vertical_jump);
        this->mobility = set_val(this->mobility);
        this->speed = set_val(this->speed);
        this->height = set_val(this->height);

        overall();
        set_pret();
    }

    void set_nume(const string &nume) {
      this->nume = nume;
    }
};


using JucatorPtr = shared_ptr<jucator>;
using EchipaPtr = shared_ptr<Echipe>;


class OutsideHitter : public jucator {
    int explosiveness;


    public:
    explicit OutsideHitter(int explosiveness)
        : explosiveness(explosiveness) {
    }

    ~OutsideHitter() override = default;

    void valori(vector <int> a) override {
        jucator::valori(a);
        this->explosiveness = set_val(a[9]);
        overall();
    }

    OutsideHitter() {
        this->pozitie = "OutsideHitter";
        this->explosiveness = 0;
    }


    void overall() override{
        this->ovr = ((this->spike_power * 2 + this->receive * 2 + this->spike_accuracy * 2 + this->serve_power + this->serve_accuracy + this->vertical_jump * 2 +
                this->mobility + this->speed + this->explosiveness * 3 + this->height * 2) / 17);
    }

    void set_pret() override {
        this->pret = 10000 + this->ovr * 50 + this->spike_accuracy * 10 + this->spike_power * 10 + this->vertical_jump * 10 + this->explosiveness * 20 + this->receive * 10 + this->height * 10;;
    }

    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> this->explosiveness;
        this->explosiveness = set_val(this->explosiveness);
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
        this->set_precision = this->set_iq = 0;
        this->pozitie = "Setter";
    }

    ~Setter() override = default;

    void valori(vector <int> a) override {
        jucator::valori(a);
        this->set_precision = set_val(a[9]);
        this->set_iq = set_val(a[10]);
        overall();
    }

    void overall() override {
        this->ovr = (this->spike_power + this->receive + this->spike_accuracy + this->serve_power + this->serve_accuracy + this->vertical_jump + this->mobility * 2 +
                this->speed * 2 + this->set_precision * 3 + this->set_iq * 3 + this->height) / 17;
    }

    void set_pret() override {
        this->pret = 8000 + this->ovr * 50 + this->speed * 10 + this->set_precision * 20 + this->set_iq * 20 + this->mobility * 10;
    }

    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> this->set_iq;
        in>> this->set_precision;
        this->set_precision = set_val(this->set_precision);
        this->set_iq  = set_val(this->set_iq);
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
        this->reflexes = 0;
        this->pozitie = "Libero";
    }

    ~Libero() override = default;


    void overall() override {
        this->ovr = (this->spike_power + this->receive * 3 + this->spike_accuracy + this->serve_power + this->serve_accuracy + this->vertical_jump + this->mobility * 2 +
                this->speed * 3 + this->reflexes * 3 + this->height) / 17;
    }

    void set_pret() override {
        this->pret = 6000 + this->ovr * 50 + this->mobility * 10 + this->speed * 20 + this->reflexes * 20 + this->receive * 20;
    }

    void valori(vector <int> a) override {
        jucator::valori(a);
        this->reflexes = set_val(a[9]);
        overall();
    }
    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> this->reflexes;
        this->reflexes = set_val(this->reflexes);
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
        this->blocking = 0;
        this->pozitie = "MiddleBlocker";
    }

    ~MiddleBlocker() override = default;

    void overall() override {
        this->ovr = (this->spike_power + this->receive + this->spike_accuracy + this->serve_power + this->serve_accuracy + this->vertical_jump + this->mobility * 2 +
                this->speed * 2 + this->blocking * 3 + this->height * 4) / 17;
    }

    void set_pret() override {
        this->pret = 7000 + this->ovr * 50 + this->blocking * 20 + this->height * 30 + this->speed * 10 + this->mobility * 10;
    }

    void valori(vector <int> a) override {
        jucator::valori(a);
        this->blocking = set_val(a[9]);
        overall();
    }
    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> this->blocking;
        this->blocking = set_val(this->blocking);
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
        this->explosiveness = 0;
        this->pozitie = "OppositeHitter";
    }

    ~OppositeHitter() override = default;

    void overall() override {
        this->ovr = ((this->spike_power * 2 + this->receive * 2 + this->spike_accuracy * 2 + this->serve_power + this->serve_accuracy + this->vertical_jump * 2 +
                this->mobility + this->speed + this->explosiveness * 3 + this->height * 2) / 17);
    }

    void set_pret() override {
        this->pret = 10000 + this->ovr * 50 + this->spike_accuracy * 10 + this->spike_power * 10 + this->vertical_jump * 10 + this->explosiveness * 20 + this->receive * 10 + this->height * 10;;
    }


    void valori(vector <int> a) override {
        jucator::valori(a);
        this->explosiveness = set_val(a[9]);
        overall();
    }
    void citeste(std::ifstream &in) override {
        jucator::citeste(in);
        in >> this->explosiveness;
        this->explosiveness = set_val(this->explosiveness);
        overall();
        set_pret();
    }

};

class Echipe {
private:
    string nume;
    vector<JucatorPtr> jucatori; 
    float ovr;
    int punctaj, seturi;

public:

    void set_overall() {
        float suma = 0;
        int j = 0;
        for (size_t i = 0; i < this->jucatori.size(); i++) {
            suma = suma + this->jucatori[i]->get_ovr();
            j++;
        }
        this->ovr = suma / j;
    }

    Echipe(const string &nume, const vector<JucatorPtr> &jucatori, float ovr, int punctaj, int seturi)
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
        this->nume = other.nume;
        this->jucatori = other.jucatori;
        this->ovr = other.ovr;
        this->punctaj = other.punctaj;
        this->seturi = other.seturi;
        return *this;
    }

    explicit Echipe(const string& nume) {
        this->ovr = 0;
        this->nume = nume;
        this->punctaj = 0;
        this->seturi = 0;


    }

    Echipe() {
        this->ovr = 0;
        this->punctaj = 0;
        this->seturi = 0;
    }
    
    ~Echipe() = default;

    int get_overall() {
        float suma = 0;
        int j = 0;
        for (size_t i = 0; i < this->jucatori.size(); i++) {
            suma = suma + this->jucatori[i]->get_ovr();
            j++;
        }
        this->ovr = suma / j;
        return static_cast<int>(this->ovr);
    }

    void creare(const vector<JucatorPtr>& lista) {
        int n = 0, index;
        cout << "\n--- Creare echipa " << this->nume << " ---\n";



        while (n < 6) {
            cout << "Alegeti jucatorul " << n+1 << " (1-" << lista.size() << "): ";
            cin >> index;

            if (index < 1 || index > (int)lista.size()) {
                cout << "Index invalid! Alegeti intre 1 si " << lista.size() << endl;
                continue;
            }

            JucatorPtr jucatorAles = lista[index-1]; 

            if (jucatorAles->esteAles()) {
                cout << "Jucatorul a fost deja selectat! Alegeti altul.\n";
                continue;
            }

            this->jucatori.push_back(jucatorAles);
            jucatorAles->setAles(true); 
            n++;
            cout << "Jucator adaugat cu succes!\n";
        }
    }

    friend ostream& operator<<(ostream& os, const Echipe& e) {
        os << "\n=== Echipa: " << e.nume << " ===\n";
        for (size_t i = 0; i < e.jucatori.size(); i++)
            os << "- " << *e.jucatori[i] << "\n";
        os << "Overall-ul echipei este: " << e.ovr << "\n";
        return os;
    }

    void adaugare_jucatori(const vector<JucatorPtr>& lista) {
        for(auto j : lista){
            this->jucatori.push_back(j);
            j->setAles(true);
        }
    }

    const string& getNume() const { return this->nume; }


    int getPunctaj() {
        return this->punctaj;
    }

    int getSeturi() {
        return this->seturi;
    }

    void modifPunctaj() {
        this->punctaj++;
    }

    void modifSetur() {
        this->seturi++;
    }

    void resetPunctaj() {
        this->punctaj = 0;
    }

    void resetSetur() {
        this->seturi = 0;
    }

    void set_nume() {
        getline(cin, this->nume);
    }
};


class Meci {
protected:
    EchipaPtr echipa1; 
    EchipaPtr echipa2; 

    void tabelaScor() {
        cout << "_____________________________\n"
             << "|       Tabela de scor      |\n"
             << "-----------------------------\n"
             << "|" << this->echipa1->getNume() << " | " << this->echipa2->getNume() << "|\n"
             << "-----------------------------\n"
             << "| Punctaj | " << this->echipa1->getPunctaj() << " | " << this->echipa2->getPunctaj() << " |\n"
             << "-----------------------------\n"
             << "| Seturi  | " << this->echipa1->getSeturi()  << " | " << this->echipa2->getSeturi()  << " |\n"
             << "-----------------------------\n";
    }

    char punct() {
        float sansa_a = pow((float)this->echipa1->get_overall(), 3.0f) /
                 (pow((float)this->echipa1->get_overall(), 3.0f) + pow((float)this->echipa2->get_overall(), 3.0f));

        float r = (float)rand() / (float)RAND_MAX;
        
        if (r < sansa_a)
            return 'a';
        else
            return 'b';
    }

    char set() {
        this->echipa1->resetPunctaj();
        this->echipa2->resetPunctaj();

        while (this->echipa1->getPunctaj() < 25 && this->echipa2->getPunctaj() < 25) {
                char castigator = punct();

                if (castigator == 'a')
                    this->echipa1->modifPunctaj();
                else
                    this->echipa2->modifPunctaj();

                cout << string(50, '\n');
                tabelaScor();

                this_thread::sleep_for(chrono::seconds((1)));

        }

        if (this->echipa1->getPunctaj() >= 25 || this->echipa2->getPunctaj() >= 25) {
            while (abs(this->echipa1->getPunctaj() - this->echipa2->getPunctaj()) <= 1){
                char castigator = punct();

                if (castigator == 'a')
                    this->echipa1->modifPunctaj();
                else
                    this->echipa2->modifPunctaj();

                cout << string(50, '\n');
                tabelaScor();

                this_thread::sleep_for(chrono::seconds((1)));
            }
        }

        if (this->echipa1->getPunctaj() > this->echipa2->getPunctaj()) {
            cout << "\nSET CASTIGAT DE " << this->echipa1->getNume() << "!\n";
            return 'a';
        }
        else {
            cout << "\nSET CASTIGAT DE " << this->echipa2->getNume() << "!\n";
            return 'b';
        }
    }


public:
    Meci(EchipaPtr echipa1, EchipaPtr echipa2) : echipa1(echipa1), echipa2(echipa2) {
    }

    Meci() : echipa1(nullptr), echipa2(nullptr) {
    }


    EchipaPtr meci() { 
        this->echipa1->resetSetur();
        this->echipa2->resetSetur();
        while (this->echipa1->getSeturi() < 3 && this->echipa2->getSeturi() < 3) {
            char castigator = set();
            if (castigator == 'a')
                this->echipa1->modifSetur();
            else
                this->echipa2->modifSetur();

            this_thread::sleep_for(chrono::seconds(2));
        }

        if (this->echipa1->getSeturi() == 3) {
            cout << "\nEchipa " << this->echipa1->getNume() << " a castigat meciul! Felicitari!\n";
            return this->echipa1;
        }
        else {
            cout << "\nEchipa " << this->echipa2->getNume() << " a castigat meciul! Felicitari!\n";
            return this->echipa2;
        }


    }
};


class BazaDeDate {
private:
    vector<JucatorPtr> jucatori;
    vector<EchipaPtr> echipe_disponibile;

public:
    explicit BazaDeDate(const vector<JucatorPtr> &jucatori)
        : jucatori(jucatori) {
    }

    BazaDeDate() = default;

    ~BazaDeDate() = default;

    void adaugaJucator(JucatorPtr j) {
        this->jucatori.push_back(j);
    }

    void adaugaEchipe(EchipaPtr e) {
        this->echipe_disponibile.push_back(e);
    }

    void afiseazaJucatori() const {
        cout << "\n=== Lista jucatorilor inregistrati ===\n";
        for (size_t i = 0; i < this->jucatori.size(); i++) {
            cout << i + 1 << ". ";
            this->jucatori[i]->afiseaza();
        }
    }

    EchipaPtr getEchipe(size_t index) const { return this->echipe_disponibile[index]; }

    const vector<JucatorPtr>& getLista() const { return this->jucatori; }

    const vector<EchipaPtr>& getListe() const { return this->echipe_disponibile; }

    EchipaPtr alege_echipa_random() {
        if (this->echipe_disponibile.empty()) return nullptr;

        int idx = rand() % this->echipe_disponibile.size();
        return this->echipe_disponibile[idx];
    }

    BazaDeDate(const BazaDeDate &other)
        : jucatori(other.jucatori),
          echipe_disponibile(other.echipe_disponibile) {
    }

    BazaDeDate & operator=(const BazaDeDate &other) {
        if (this == &other)
            return *this;
        this->jucatori = other.jucatori;
        this->echipe_disponibile = other.echipe_disponibile;
        return *this;
    }
};

class Liga : public Meci {
private:
    BazaDeDate* baza;
    vector<int> punctaje;
    vector<vector<bool>> ales;
    vector<bool> etapa_jucat;

public:

    EchipaPtr meci(EchipaPtr echipa) {
    auto lista = this->baza->getListe();
    size_t n = lista.size(); 
    size_t index = (size_t)-1; 
    size_t index_echipa_manager = n - 1;

    while (true) {
        EchipaPtr adversar = this->baza->alege_echipa_random();
        for (size_t i = 0; i < n; i++) {
            if (lista[i] == adversar) {
                index = i;
                break;
            }
        }

        if (index == (size_t)-1 || index == index_echipa_manager)
            continue;

        if (this->ales[index_echipa_manager][index] == true || this->etapa_jucat[index] == true)
            continue;


        break;
    }

    this->ales[index_echipa_manager][index] = true;
    this->ales[index][index_echipa_manager] = true;
    this->etapa_jucat[index] = true;
    this->etapa_jucat[index_echipa_manager] = true;

    Meci meci_local(echipa, lista[index]);
    EchipaPtr castigatoare = meci_local.meci();

    if (castigatoare == echipa) {
        cout << "Felicitari, echipa ta a castigat meciul, obtinand 3 puncte in clasament.\n";
        this->punctaje[index_echipa_manager] += 3;
    } else {
        cout << "Din pacate ati pierdut meciul, nu ati obtinut niciun punct in clasament...\n";
        this->punctaje[index] += 3;
    }

    return castigatoare; 

}


    Liga(BazaDeDate* baza, const vector<EchipaPtr>&) : baza(baza) {
        size_t n = this->baza->getListe().size(); 
        this->ales = vector<vector<bool>>(n, vector<bool>(n, false));
        this->etapa_jucat.resize(n, false);
        this->punctaje.resize(n, 0);
        for (size_t i = 0; i < n; i++) { 
            this->ales[i][i] = true;
        }
    }


    void afisare_punctaje() {
        for (size_t i = 0; i < this->punctaje.size(); i++)
            cout << this->baza->getEchipe(i)->getNume() << ": " << this->punctaje[i] << "\n";
    }

    void meciuri() {
    auto lista = this->baza->getListe();
    size_t n = lista.size();
    size_t index_echipa_manager = n - 1;

    if (this->punctaje.size() != n)
        this->punctaje.resize(n, 0);


    while (this->etapa_jucat != vector<bool>(n, true)) {
            bool progres_facut;


        progres_facut = false;

        for (size_t i = 0; i < n; i++) { 

            if (i == index_echipa_manager || this->etapa_jucat[i])
                continue;

            for (size_t index = 0; index < n; index++) { 

                if (index == index_echipa_manager || index == i || this->etapa_jucat[index])
                    continue;

                if (!this->ales[i][index]) {


                    this->ales[i][index] = true;
                    this->ales[index][i] = true;

                    this->etapa_jucat[i] = true;
                    this->etapa_jucat[index] = true;
                    progres_facut = true;

                    double factor_aleator_1 = (rand() % 11 - 5) * 0.1;
                    double factor_aleator_2 = (rand() % 11 - 5) * 0.1;

                    double scor1 = lista[i]->get_overall() + factor_aleator_1;
                    double scor2 = lista[index]->get_overall() + factor_aleator_2;

                    EchipaPtr castigatoare;

                    if (scor1 > scor2)
                        castigatoare = lista[i];
                    else if (scor2 > scor1)
                        castigatoare = lista[index];
                    else
                        castigatoare = (rand() % 2 == 0) ? lista[i] : lista[index];

                    if (castigatoare == lista[i])
                        this->punctaje[i] += 3;
                    else
                        this->punctaje[index] += 3;

                    break;
                }
            }
        }


        if (!progres_facut && this->etapa_jucat != vector<bool>(n, true)) {
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
    unique_ptr<Echipe> echipa; 
    string nume;
    vector<JucatorPtr> lista; 
    BazaDeDate* baza;

public:

    EchipaPtr get_echipa() const { 
        return EchipaPtr(this->echipa.get());
    }

    manageri(int buget, EchipaPtr echipa, const string &nume) = delete;

    void set_overall() {
        this->echipa->Echipe::set_overall();
    }

    manageri() : buget(0), echipa(make_unique<Echipe>()), nume("Necunoscut"), lista({}), baza(nullptr) { 
    }

    friend ostream & operator<<(ostream &os, const manageri &obj) {
        return os
               << "buget: " << obj.buget
               << " echipa: " << obj.echipa
               << " nume: " << obj.nume
               << " jucator: " << &obj.lista;
    }

    void alegere_echipa() {
        this->baza->afiseazaJucatori();
        this->echipa->creare(this->baza->getLista());
    }

    void numeEchipa() {
        cout<<"Dati numele echipei voastre:\n";
        this->echipa->set_nume();
    }

    string get_nume() const {
      return  this->echipa->getNume();
    }

    void adaugare_jucatori_valabili(BazaDeDate* baza_ptr) {
        this->baza = baza_ptr;
    }

    manageri(const manageri& other) = delete; 
    manageri& operator=(const manageri& other) = delete; 

~manageri() = default; 
};


int main() {
    srand(static_cast<unsigned int>(time(0))); 
    BazaDeDate baza_echipe, baza_jucatori_valabili;

    manageri manager;

    manager.numeEchipa();

    vector<EchipaPtr> echipe;
    vector<string> numeEchipe = {"Dinamo","Rapid","CFR","UCluj","Farul",
                                 "Petrolul","UTA","FCSB","Voluntari","Botosani","Otelul"};


    for (const auto &nume : numeEchipe)
        echipe.push_back(make_shared<Echipe>(nume));


    ifstream fin("text.txt");
    if (!fin) {
        cerr << "Nu s-a putut deschide fisierul text.txt\n";
        return 1;
    }

    for (auto& echipa : echipe) {
        vector<JucatorPtr> jucatoriEchipa;

        string pozitie_local, nume_local;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
        JucatorPtr j = nullptr; 
        int i = 0;
        while(i < 6) {
            if (!(fin >> pozitie_local)) break; 
            vector<int> a;
            fin>>nume_local; 

            if (pozitie_local == "Libero") {
                j = make_shared<Libero>(); 
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie_local == "Setter") {
                j = make_shared<Setter>(); 
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
                j->valori(a);
            }
            else if (pozitie_local == "OutsideHitter") {
                j = make_shared<OutsideHitter>(); 
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie_local == "OppositeHitter") {
                j = make_shared<OppositeHitter>(); 
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else if (pozitie_local == "MiddleBlocker") {
                j = make_shared<MiddleBlocker>(); 
                fin>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
                a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
                a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
                j->valori(a);
            }
            else continue;
            j->set_nume(nume_local);
            j->set_poz(pozitie_local); 

            cout<<*j<<endl;

            jucatoriEchipa.push_back(j);
            i++;
        }
        echipa->adaugare_jucatori(jucatoriEchipa);
        jucatoriEchipa.clear();
        echipa->get_overall();
        baza_echipe.adaugaEchipe(echipa);
    }
    fin.close();


    ifstream finn("tastatura.txt");
    if (!finn) {
        cerr << "Nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    while(true) {
        string pozitie_local, nume_local;
        int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10;
        JucatorPtr j = nullptr; 
        if (!(finn >> pozitie_local)) break; 
        vector<int> a;
        finn>>nume_local; 

        if (pozitie_local == "Libero") {
            j = make_shared<Libero>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie_local == "Setter") {
            int a11;
            j = make_shared<Setter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10>>a11;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10); a.push_back(a11);
            j->valori(a);
        }
        else if (pozitie_local == "OutsideHitter") {
            j = make_shared<OutsideHitter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie_local == "OppositeHitter") {
            j = make_shared<OppositeHitter>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else if (pozitie_local == "MiddleBlocker") {
            j = make_shared<MiddleBlocker>();
            finn>>a1>>a2>>a3>>a4>>a5>>a6>>a7>>a8>>a9>>a10;
            a.push_back(a1); a.push_back(a2); a.push_back(a3); a.push_back(a4); a.push_back(a5);
            a.push_back(a6); a.push_back(a7); a.push_back(a8); a.push_back(a9); a.push_back(a10);
            j->valori(a);
        }
        else continue;
        j->set_nume(nume_local);
        j->set_poz(pozitie_local);

        baza_jucatori_valabili.adaugaJucator(j);


    }
    finn.close();
    numeEchipe.push_back( manager.get_nume());

    cout << "S-au citit toate echipele cu jucatorii.\n";

    manager.adaugare_jucatori_valabili(&baza_jucatori_valabili); 
    manager.alegere_echipa(); 
    manager.set_overall();
    baza_echipe.adaugaEchipe(manager.get_echipa());

    echipe.push_back( manager.get_echipa());

    for (const auto& e : echipe)
        cout << *e;

    Liga liga(&baza_echipe, echipe);

    for (int i = 0; i < 12; i++) {
        string s;

        size_t index_echipa_manager = echipe.size() - 1; 

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

    return 0;
}
