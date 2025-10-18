#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "player.h"
#include "supemon.h"
#include "items.h"

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

/**
 * Display player's items and return the count.
 */
int display_items(Player* player)
{
    int i;
    for (i = 0; i < MAX_ITEMS; i++)
    {
        const Item* current = player->items[i];
        if (current == NULL) break; // Found an empty slot

        printf("%d - %s\n", i + 1, current->name);
    }

    return i;
}

/**
 * Return the count of player's items.
 */
int get_item_count(Player* player)
{
    int i;
    for (i = 0; i < MAX_ITEMS; i++)
    {
        const Item* current = player->items[i];
        if (current == NULL) break; // Found an empty slot
    }

    return i;
}

int remove_item(Player* player, int index)
{
    if (index < 0 || index >= MAX_ITEMS) return 0;

    int max_index = get_item_count(player) - 1; // get the index of players's last item in the array
    if (max_index < 0 || max_index > MAX_ITEMS - 1) return 0;

    for (int i = index; i < max_index; i++)
    {
        player->items[i] = player->items[i + 1]; // starting at used item pos, move every following item one index to the left
    }

    player->items[max_index] = NULL; // set last pointer to NULL
    return 1;
}

int add_item(Player* player, const Item* item)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (player->items[i] == NULL)
        {
            player->items[i] = item;
            return 1;
        }
    }

    return 0;
}

int use_item(Player* player, Supemon* supemon, int index)
{
    const Item* item = player->items[index];

    switch (item->type)
    {
        case ITEM_POTION:
        {
            update_health(supemon, 5);
            break;
        }
        case ITEM_SUPER_POTION:
        {
            update_health(supemon, 10);
            break;
        }
        case ITEM_RARE_CANDY:
        {
            // Do this instead of calling level_up() to give a full level
            unsigned int gained_xp = xp_to_next_level(supemon->level);
            gain_experience(supemon, gained_xp);
            break;
        }
        default: return 0;
    }

    return remove_item(player, index);
}