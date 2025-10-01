#pragma once

#include "supemon.h"
#include "items.h"

typedef struct {
    char name[16];
    Supemon supemons[6];
    int active_index;
    unsigned int coins;
    Item* items[10];
} Player;

void init_player(Player* player, const char* name, Supemon starter);
Supemon* get_active_supemon(Player* player);
void set_active_supemon(Player* player, int index);
int add_supemon(Player* player, Supemon new_supemon);