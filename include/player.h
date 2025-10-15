#pragma once

#include "global.h"
#include "supemon.h"
#include "items.h"

typedef struct {
    char name[MAX_NAME_LENGTH];
    Supemon supemons[MAX_SUPEMONS];
    int active_index;
    unsigned int coins;
    Item* items[MAX_ITEMS];
} Player;

void init_player(Player* player, const char* name, Supemon starter);
Supemon* get_active_supemon(Player* player);
void set_active_supemon(Player* player, int index);
int add_supemon(Player* player, Supemon new_supemon);