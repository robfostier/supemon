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

/**
 * Remove Item* from player's item list.
 * Returns 1 on success, 0 on failure.
 */
int remove_item(Player* player, int index);

/**
 * Add Item* to player's item list.
 * Returns 1 on success, 0 on failure.
 */
int add_item(Player* player, const Item* item);

/**
 * Use Item on player's Supemon.
 * Returns 1 on success, 0 on failure.
 */
int use_item(Player* player, Supemon* supemon, int index);

/**
 * Save player data to file.
 * Returns 1 on success, 0 on failure.
 */
int save_game(Player* player);

/**
 * Load player data from file.
 * Returns 1 on success, 0 on failure.
 */
int load_game(Player* player);