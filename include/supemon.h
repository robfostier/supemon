#pragma once

#include "moves.h"

typedef struct {
    char name[16];
    unsigned int level;
    unsigned int experience;
    unsigned int health;
    unsigned int max_health;
    unsigned int attack;
    unsigned int base_attack;
    unsigned int defense;
    unsigned int base_defense;
    unsigned int evasion;
    unsigned int base_evasion;
    unsigned int accuracy;
    unsigned int base_accuracy;
    unsigned int speed;
    Move* moves[4];
} Supemon;

extern Supemon SUPMANDER;
extern Supemon SUPASAUR;
extern Supemon SUPIRTLE;

Supemon init_supemon(Supemon* template, unsigned int level);
void level_up(Supemon* supemon);
void increase_stat(unsigned int* stat, double mult);
void gain_experience(Supemon* supemon, unsigned int xp);
unsigned int xp_to_next_level(unsigned int current_level);