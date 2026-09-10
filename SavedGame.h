#ifndef SAVEDGAME_H
#define SAVEDGAME_H

#include <string>

// Estado mínimo para reanudar una partida
struct SavedGame {
    std::string playerName;
    int level = 1;
    int score = 0;
    int health = 100;
    int fuel = 100;
    int rescued = 0;
    double timeElapsed = 0.0;
    double posX = 0.0;
    double posY = 0.0;
};

#endif // SAVEDGAME_H
