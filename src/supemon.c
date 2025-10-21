#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "supemon.h"
#include "moves.h"
#include "colors.h"
#include "utils.h"

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
    // Déterminer la couleur du nom selon le joueur (vert pour joueur, rouge pour ennemi)
    const char* name_color = (strcmp(player_name, "enemy") == 0) ? COLOR_ENEMY : COLOR_PLAYER;

    // Calculer la couleur HP selon le pourcentage
    float hp_percent = (float)supemon->health / (float)supemon->max_health;
    const char* hp_color;
    if (hp_percent > 0.5)
        hp_color = COLOR_HP;
    else if (hp_percent > 0.2)
        hp_color = COLOR_HP_LOW;
    else
        hp_color = COLOR_HP_CRITICAL;

    // HP BAR (ligne du haut) - Bordures blanches
    int hp_len = 5 + snprintf(NULL, 0, "%d", supemon->health) + snprintf(NULL, 0, "%d", supemon->max_health);
    int hp_spaces = BOX_WIDTH - hp_len;
    if (hp_spaces < 0) hp_spaces = 0;

    printf("+");
    for (int i = 0; i < hp_spaces; i++) printf("-");
    printf("%s-[%d/%d]%s-+\n", hp_color, supemon->health, supemon->max_health, RESET);

    // NAME - Bordures blanches, nom coloré
    int name_len = strlen(supemon->name) + strlen(player_name) + 4;
    int name_spaces = BOX_WIDTH - name_len;
    if (name_spaces < 0) name_spaces = 0;

    printf("| ");
    printf("%s%s%s%s", name_color, BOLD, supemon->name, RESET);
    printf(" (%s)%*s|\n", player_name, name_spaces, "");

    // LVL + XP BAR - Bordures blanches
    int lvl_len = 10 + snprintf(NULL, 0, "%d", supemon->level);
    int xp_bar_len = BOX_WIDTH - lvl_len;
    if (xp_bar_len < 0) xp_bar_len = 0;

    float xp_ratio = (float)supemon->experience / xp_to_next_level(supemon->level);
    if (xp_ratio > 1.0f) xp_ratio = 1.0f;
    if (xp_ratio < 0.0f) xp_ratio = 0.0f;
    int progress = (int)(xp_ratio * xp_bar_len);

    printf("| Lvl. %s%d%s %s[", BOLD, supemon->level, RESET, COLOR_XP);
    for (int i = 0; i < xp_bar_len; i++)
    {
        if (i < progress) printf("#");
        else printf("-");
    }
    printf("]%s |\n", RESET);

    // Ligne de séparation
    printf("+------------------------------+\n");

    // STATS BOX - Stats colorées selon buffs/debuffs
    int total_atk = supemon->attack + supemon->base_attack;
    int total_def = supemon->defense + supemon->base_defense;
    int total_acc = supemon->accuracy + supemon->base_accuracy;
    int total_eva = supemon->evasion + supemon->base_evasion;

    // Déterminer les couleurs selon les buffs/debuffs
    const char* atk_color = (supemon->attack > 0) ? COLOR_STAT_UP : (supemon->attack < 0) ? COLOR_STAT_DOWN : "";
    const char* def_color = (supemon->defense > 0) ? COLOR_STAT_UP : (supemon->defense < 0) ? COLOR_STAT_DOWN : "";
    const char* acc_color = (supemon->accuracy > 0) ? COLOR_STAT_UP : (supemon->accuracy < 0) ? COLOR_STAT_DOWN : "";
    const char* eva_color = (supemon->evasion > 0) ? COLOR_STAT_UP : (supemon->evasion < 0) ? COLOR_STAT_DOWN : "";

    printf("| Atk: %s%-3d%s            Def: %s%-3d%s |\n",
           atk_color, total_atk, RESET,
           def_color, total_def, RESET);

    printf("| Acc: %s%-3d%s            Eva: %s%-3d%s |\n",
           acc_color, total_acc, RESET,
           eva_color, total_eva, RESET);

    printf("+------------------------------+\n\n");
}