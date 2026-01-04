#pragma once

#include "Screen.h"
#include "Echipe.h"
#include "Button.h"
#include "TableScore.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <optional>
#include <sstream>
#include "Gestiune.h"
class MatchLeague : public Screen {
private:
    Echipeptr echipaMea;
    Echipeptr echipaAdversa;

    TableScore tablaScor;
    sf::Text statusText;

    Button timeoutButton;
    Button continueButton;

    sf::Clock actionClock;
    enum MatchState {
        SIMULATION_IN_PROGRESS,
        TIMEOUT_CALLED,
        MATCH_END_DELAY,
        MATCH_FINISHED
    };
    MatchState currentState = SIMULATION_IN_PROGRESS;

    std::string finalMessage;
    bool isMyTeamServing = true;
    Gestiune<std::string>& istoricMeciuri;

public:
    MatchLeague(Echipeptr myTeam, Echipeptr opponent, sf::Font& font, Gestiune<std::string>& istoric);

    int run(sf::RenderWindow& window) override;

    void setAdversar(Echipeptr newOpponent);

    void resetare_scoruri();

    Echipeptr getEchipaAdversa() const;
    Echipeptr getMatchWinner() const;

    Echipeptr getAdversar() const;


private:
    void handleInput(const sf::Event& event, sf::RenderWindow& ) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
    void simulatePoint();
    void finalizeMatch();
    void checkSetEnd();
    void resetForNewSet();
    void toggleServe();

    float getWeightedTeamScore(Echipeptr echipa) const ;



};