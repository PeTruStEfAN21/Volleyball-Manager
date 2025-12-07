#pragma once

#include <stdexcept>
#include <string>


class JocLogicException : public std::runtime_error {
public:
    explicit JocLogicException(const std::string& msg)
        : std::runtime_error("Eroare Logică: " + msg) {}
};


class BugetDepasitException : public JocLogicException {
public:
    explicit BugetDepasitException(const std::string& msg)
        : JocLogicException("Buget depasit. " + msg) {}
};



class CompozitieInvalidaException : public JocLogicException {
public:
    explicit CompozitieInvalidaException(const std::string& msg)
        : JocLogicException("Compozitie invalida. " + msg) {}
};



class SchimbareInvalidaException : public JocLogicException {
public:
    explicit SchimbareInvalidaException(const std::string& msg)
        : JocLogicException("Schimbare invalida. " + msg) {}
};