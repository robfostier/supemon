#pragma once

#include "global.h"
#include "moves.h"

typedef struct {
    char name[MAX_NAME_LENGTH];
    unsigned int level;
    unsigned int experience;
    int health;
    int max_health;
    int attack;
    int base_attack;
    int defense;
    int base_defense;
    int evasion;
    int base_evasion;
    int accuracy;
    int base_accuracy;
    int speed;
    const Move* moves[MAX_MOVES];
} Supemon;

extern const Supemon SUPMANDER;
extern const Supemon SUPASAUR;
extern const Supemon SUPIRTLE;

/**
 * Initalize a Supemon.
 * Makes a copy from a template and assigns it a level.
 */
Supemon init_supemon(const Supemon* template, unsigned int level);

void level_up(Supemon* supemon);
void increase_stat(int* stat, float mult);
void gain_experience(Supemon* supemon, unsigned int xp);
unsigned int xp_to_next_level(unsigned int current_level);
void update_health(Supemon* supemon, int value);

/**
 * Return a random Supemon* template.
 * Returns NULL on failure.
 */
const Supemon* get_random_supemon_template();

void display_supemon(Supemon* supemon, char player_name[]);