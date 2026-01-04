#pragma once
#include <vector>
#include <string>
#include <iostream>

template <typename T>
class Gestiune {
private:
    std::vector<T> elemente;
    std::string numeCategorie;

public:
    explicit Gestiune(const std::string& nume) : numeCategorie(nume) {}
    void adauga(const T& element) {
        elemente.push_back(element);
    }

    size_t getNrElemente() const {
        return elemente.size();
    }

    void afiseazaStatus() const {
        std::cout << "Gestiunea [" << numeCategorie << "] are in prezent " << elemente.size() << " elemente.\n";
    }

    const std::vector<T>& getToate() const {
        return elemente;
    }
};