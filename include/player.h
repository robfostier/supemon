#pragma once

#include "global.h"
#include "supemon.h"
#include "items.h"

typedef struct {
    char name[MAX_NAME_LENGTH];
    Supemon supemons[MAX_SUPEMONS];
    int active_index;
    unsigned int coins;
    const Item* items[MAX_ITEMS];
} Player;

void init_player(Player* player, const char* name, Supemon starter);
Supemon* get_active_supemon(Player* player);
void set_active_supemon(Player* player, int index);
int add_supemon(Player* player, Supemon new_supemon);

int display_supemons(Player* player);
int display_items(Player* player);

int get_item_count(Player* player);
void remove_item(Player* player, int index);

void use_item(Player* player, Supemon* supemon, int index);