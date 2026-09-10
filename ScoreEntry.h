#ifndef SCOREENTRY_H
#define SCOREENTRY_H

#include <string>

// Un registro de puntuación / mejor tiempo al terminar (o fallar) una partida.
struct ScoreEntry {
    std::string playerName;
    int score = 0;
    double timeSeconds = 0.0;  // tiempo en el nivel / partida
    int level = 1;             // 1, 2 o 3
};

#endif
