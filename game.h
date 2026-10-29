#ifndef GAME_H
#define GAME_H

#include <QString>

struct Game {
    QString title;
    int players;
    int timeMinutes;
    QString difficulty;
    QString description;
};

#endif // GAME_H