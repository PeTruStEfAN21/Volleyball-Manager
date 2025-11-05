#pragma once

#include <iostream>
#include <string>
#include <memory>

class Echipe;


using Echipeptr = std::shared_ptr<Echipe>;

class Meci {
protected:
    Echipeptr echipa1;
    Echipeptr echipa2;

    void tabelaScor();
    char punct();
    char set();

public:
    Meci(Echipeptr echipa1, Echipeptr echipa2);
    Meci();
    

    Echipeptr meci();
};