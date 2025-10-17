#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "player.h"
#include "supemon.h"

void init_player(Player* player, const char* name, Supemon starter)
{
    snprintf(player->name, sizeof(player->name), "%s", name);

    player->supemons[0] = starter;
    set_active_supemon(player, 0);

    for (int i = 1; i < MAX_SUPEMONS; i++) {
        memset(&player->supemons[i], 0, sizeof(Supemon)); // We'll initialize the rest to empty, and check for empty slots by checking level == 0
    }


    player->coins = 0;
    for (int i = 0; i < MAX_ITEMS; i++) {
        player->items[i] = NULL;
    }
}

Supemon* get_active_supemon(Player* player)
{
    if (player->active_index < 0 || player->active_index >= MAX_SUPEMONS) {
        return NULL; // Invalid index
    }
    return &player->supemons[player->active_index];
}

void set_active_supemon(Player* player, int index)
{
    if (index >= 0 && index < MAX_SUPEMONS && player->supemons[index].level > 0) {
        player->active_index = index;
    }
}

int add_supemon(Player* player, Supemon new_supemon)
{
    for (int i = 0; i < MAX_SUPEMONS; i++) {
        if (player->supemons[i].level == 0) { // Check for empty slot
            player->supemons[i] = new_supemon;
            return 1;
        }
    }
    return 0;
}

int display_supemons(Player* player)
{
    int i;
    for (i = 0; i < MAX_SUPEMONS; i++)
    {
        Supemon* current = &player->supemons[i];
        if (current->level == 0) break; // Found an empty slot

        printf("%d - %s\n", i + 1, current->name);
    }

    return i;
}

int display_items(Player* player)
{
    int i;
    for (i = 0; i < MAX_ITEMS; i++)
    {
        Item* current = player->items[i];
        if (current == NULL) break; // Found an empty slot

        printf("%d - %s\n", i + 1, current->name);
    }

    return i;
}