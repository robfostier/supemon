#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "supemon.h"
#include "moves.h"

const Supemon SUPMANDER = {
    .name = "Supmander",
    .level = 1,
    .experience = 0,
    .health = 10,
    .max_health = 10,
    .attack = 0,
    .base_attack = 1,
    .defense = 0,
    .base_defense = 1,
    .evasion = 0,
    .base_evasion = 1,
    .accuracy = 0,
    .base_accuracy = 2,
    .speed = 1,
    .moves = {&SCRATCH, &GRAWL, NULL, NULL}
};

const Supemon SUPASAUR = {
    .name = "Supasaur",
    .level = 1,
    .experience = 0,
    .health = 9,
    .max_health = 9,
    .attack = 0,
    .base_attack = 1,
    .defense = 0,
    .base_defense = 1,
    .evasion = 0,
    .base_evasion = 3,
    .accuracy = 0,
    .base_accuracy = 2,
    .speed = 2,
    .moves = {&POUND, &FOLIAGE, NULL, NULL}
};

const Supemon SUPIRTLE = {
    .name = "Supirtle",
    .level = 1,
    .experience = 0,
    .health = 11,
    .max_health = 11,
    .attack = 0,
    .base_attack = 1,
    .defense = 0,
    .base_defense = 2,
    .evasion = 0,
    .base_evasion = 2,
    .accuracy = 0,
    .base_accuracy = 1,
    .speed = 2,
    .moves = {&POUND, &SHELL, NULL, NULL}
};

Supemon init_supemon(const Supemon* template, unsigned int level)
{
    Supemon new_supemon = *template;

    if (level < 1) return new_supemon; // Invalid level, return as is
    else if (level > 1)
    {
        for (unsigned int i = 1; i < level; i++)
        {
            level_up(&new_supemon);
        }
    }

    return new_supemon;
}

void level_up(Supemon* supemon){
    supemon->level++;
    supemon->experience = 0;

    increase_stat(&supemon->max_health, 1.3);

    // Full heal on level up
    supemon->health = supemon->max_health;

    increase_stat(&supemon->base_attack, 1.3);
    increase_stat(&supemon->base_defense, 1.3);
    increase_stat(&supemon->base_evasion, 1.3);
    increase_stat(&supemon->base_accuracy, 1.3);
    increase_stat(&supemon->speed, 1.3);
}

void increase_stat(int* stat, float mult)
{
    float tmp = (*stat) * mult;
    if (fabsf(tmp - floorf(tmp)) < 1e-6f) *stat = (int)tmp;
    else
    {
        *stat = (int)tmp;

        // 50% chance to round up, since half of random numbers are odd
        if (rand() % 2 == 1) (*stat)++; // *stat in parenthesis to increase the value, not the pointer itself
    }
}

void gain_experience(Supemon* supemon, unsigned int xp)
{
    supemon->experience += xp;

    unsigned int needed_xp = xp_to_next_level(supemon->level);

    if (supemon->experience >= needed_xp)
    {
        unsigned int bonus_xp = supemon->experience - needed_xp;
        level_up(supemon);
        gain_experience(supemon, bonus_xp); // If xp gain was more than needed for a level up, make sure to apply the bonus xp as well
    }
}

unsigned int xp_to_next_level(unsigned int current_level)
{
    if (current_level < 1) return 0; // Invalid level
    else if (current_level == 1) return 500;
    else return xp_to_next_level(current_level - 1) + 1000;
}

void update_health(Supemon* supemon, int value)
{
    supemon->health += value;
    if (supemon->health < 0) supemon->health = 0;
    else if (supemon->health > supemon->max_health) supemon->health = supemon->max_health; 
}

const Supemon* get_random_supemon_template()
{
    int rnd = rand() % 3;
    
    switch(rnd)
    {
        case 0: return &SUPMANDER;
        case 1: return &SUPASAUR;
        case 2: return &SUPIRTLE;
        default: return NULL;
    }
}

void display_supemon(Supemon* supemon, char player_name[])
{
    // HP
    int hp_len = 5 + snprintf(NULL, 0, "%d", supemon->health) + snprintf(NULL, 0, "%d", supemon->max_health); // " [" + "/" + "] "
    int hp_spaces = BOX_WIDTH - hp_len;

    if (hp_spaces < 0) hp_spaces = 0;

    printf("+");
    for (int i = 0; i < hp_spaces; i++) printf("-");
    printf("-[%d/%d]-+\n", supemon->health, supemon->max_health);

    // NAME
    int name_len = strlen(supemon->name) + strlen(player_name) + 4; // " Supemon (player)"
    int name_spaces = BOX_WIDTH - name_len;

    if (name_spaces < 0) name_spaces = 0;
    printf("| %s (%s)%*s|\n", supemon->name, player_name, name_spaces, "");
    
    // LVL + XP BAR
    int lvl_len = 10 + snprintf(NULL, 0, "%d", supemon->level); // " [" + progress bar + "] Lvl. {lvl} "
    int xp_bar_len = BOX_WIDTH - lvl_len;

    if (xp_bar_len < 0) xp_bar_len = 0; // Avoid bugs

    float xp_ratio = (float)supemon->experience / xp_to_next_level(supemon->level);
    if (xp_ratio > 1.0f) xp_ratio = 1.0f;
    if (xp_ratio < 0.0f) xp_ratio = 0.0f;
    int progress = (int)(xp_ratio * xp_bar_len);

    char xp_bar[BOX_WIDTH - 10]; // maximum size

    for (int i = 0; i < xp_bar_len; i++)
    {
        if (i < progress) xp_bar[i] = '#';
        else xp_bar[i] = '-';
    }

    xp_bar[xp_bar_len] = '\0';

    printf("| Lvl. %d [%s] |\n", supemon->level, xp_bar);
    
    printf("+------------------------------+\n");

    // STATS BOX
    printf("| Atk: %-3d            Def: %-3d |\n", get_battle_stat(supemon, 'A'), get_battle_stat(supemon, 'D'));
    printf("| Acc: %-3d            Eva: %-3d |\n", get_battle_stat(supemon, 'P'), get_battle_stat(supemon, 'E'));
    printf("+------------------------------+\n\n");
}

int get_battle_stat(Supemon* supemon, char stat)
{
    int battle_stat;

    switch(stat)
    {
        case 'A':
            battle_stat = supemon->base_attack + supemon->attack;
            if (battle_stat >= 0) return battle_stat;
            else return 0;

        case 'D':
            battle_stat = supemon->base_defense + supemon->defense;
            if (battle_stat >= 0) return battle_stat;
            else return 0;

        case 'P':
            battle_stat = supemon->base_accuracy + supemon->accuracy;
            if (battle_stat >= 0) return battle_stat;
            else return 0;

        case 'E':
            battle_stat = supemon->base_evasion + supemon->evasion;
            if (battle_stat >= 0) return battle_stat;
            else return 0;

        default: return 0;
    }
}