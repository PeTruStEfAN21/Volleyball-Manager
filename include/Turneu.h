#include <vector>
#include "Meci.h"

using Echipeptr = std::shared_ptr<Echipe>;

class MeciTurneu : public Meci {
    Echipe* echipa1;
    Echipe* echipa2;
    Echipe* castigatoare;

    MeciTurneu* meci_anterior1;
    MeciTurneu* meci_anterior2;

    public:

    MeciTurneu(Echipe* echipa1, Echipe* echipa2, Echipe* castigatoare, MeciTurneu* meci_anterior1, MeciTurneu* meci_anterior2);

    ~MeciTurneu();

    void meci_turneu(Echipe* echipa1, Echipe* echipa2);
};

class Turneu : public Meci {
    Meci* finala;

    public:
    Turneu():
    finala(nullptr){}

    ~Turneu();



};