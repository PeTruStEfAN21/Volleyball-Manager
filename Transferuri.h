#pragma once


#include <vector>
#include "Echipe.h"
#include "BazaDeDate.h"
#include <string>
#include "jucator.h"

using jucatorptr = std::shared_ptr<jucator>;
using Echipeptr = std::shared_ptr<Echipe>;

class Transferuri {
    BazaDeDateptr baza;
    Echipeptr echipa;

    public:

    Transferuri();
    ~Transferuri();

    Transferuri(BazaDeDateptr baza, Echipeptr echipa);

    void transfer_manager() ;

    void transferuri_ai();

};