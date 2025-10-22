#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include "utils.h"
#include "player.h"
#include "supemon.h"
#include "items.h"
#include "colors.h"

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

        printf("%d - %s%s%s\n", i + 1, type_to_color(current->type), current->name, RESET);
        display_supemon(current, player->name);
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

int save_game(Player* player)
{
    FILE* file = fopen(SAVE_FILE, "w");
    if (file == NULL)
    {
        printf("Error: Cannot create save file.\n");
        return 0;
    }

    // 1. Nom du joueur
    fprintf(file, "%s\n", player->name);

    // 2. Supcoins
    fprintf(file, "%u\n", player->coins);

    // 3. Index actif
    fprintf(file, "%d\n", player->active_index);

    // 4. Nombre de Supémons
    int supemon_count = 0;
    for (int i = 0; i < MAX_SUPEMONS; i++)
    {
        if (player->supemons[i].level > 0) supemon_count++;
        else break;
    }
    fprintf(file, "%d\n", supemon_count);

    // 5. Chaque Supémon
    for (int i = 0; i < supemon_count; i++)
    {
        Supemon* s = &player->supemons[i];
        fprintf(file, "%s %s %u %u %d %d %d %d %d %d %d ",
                s->name, s->type, s->level, s->experience, s->health, s->max_health,
                s->base_attack, s->base_defense, s->base_evasion, s->base_accuracy, s->speed);

        // Moves (sauvegarder les noms)
        for (int j = 0; j < MAX_MOVES; j++)
        {
            if (s->moves[j] != NULL)
                fprintf(file, "%s ", s->moves[j]->name);
            else
                fprintf(file, "NONE ");
        }
        fprintf(file, "\n");
    }

    // 6. Items (utiliser des codes numériques)
    int item_count = get_item_count(player);
    fprintf(file, "%d\n", item_count);

    for (int i = 0; i < item_count; i++)
    {
        // Sauvegarder un code : 1=Potion, 2=Super Potion, 3=Rare Candy
        if (player->items[i] == &POTION)
            fprintf(file, "1 ");
        else if (player->items[i] == &SUPER_POTION)
            fprintf(file, "2 ");
        else if (player->items[i] == &RARE_CANDY)
            fprintf(file, "3 ");
        else
            fprintf(file, "0 ");
    }
    fprintf(file, "\n");

    fclose(file);
    printf("\nGame saved successfully!\n");
    return 1;
}

int load_game(Player* player)
  {
    FILE* file = fopen(SAVE_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }

    // 1. Nom
    if (fscanf(file, "%12s\n", player->name) != 1)
    {
        fclose(file);
        return 0;
    }

    // 2. Coins
    if (fscanf(file, "%u\n", &player->coins) != 1)
    {
        fclose(file);
        return 0;
    }

    // 3. Index actif
    if (fscanf(file, "%d\n", &player->active_index) != 1)
    {
        fclose(file);
        return 0;
    }

    // 4. Nombre de Supémons
    int supemon_count;
    if (fscanf(file, "%d\n", &supemon_count) != 1)
    {
        fclose(file);
        return 0;
    }

    // 5. Charger chaque Supémon
    for (int i = 0; i < supemon_count; i++)
    {
        Supemon* s = &player->supemons[i];

        char type_buffer[16];

        unsigned int level;
        unsigned int experience; 
        int health;
        int max_health;
        int base_attack;
        int base_defense;
        int base_evasion;
        int base_accuracy;
        int speed;

        if (fscanf(file, "%12s %15s %u %u %d %d %d %d %d %d %d ",
                    s->name, type_buffer, &level, &experience, &health, &max_health,
                    &base_attack, &base_defense, &base_evasion, &base_accuracy, &speed) != 11)
        {
            fclose(file);
            return 0;
        }
    
        s->type = strdup(type_buffer);

        // Set statistics
        if (level <= 0) level = 1;
        s->level = level;

        if (max_health <= 0) max_health = 1;
        s->max_health = max_health;

        if (health < 0 ) health = 0;
        if (health > max_health) health = max_health;
        s->health = health;

        if (base_attack <= 0) base_attack = 1;
        s->base_attack = base_attack;
        s->attack = 0;
        if (base_defense <= 0) base_defense = 1;
        s->base_defense = base_defense;
        s->defense = 0;
        if (base_accuracy <= 0) base_accuracy = 1;
        s->base_accuracy = base_accuracy;
        s->accuracy = 0;
        if (base_evasion <= 0) base_evasion = 1;
        s->base_evasion = base_evasion;
        s->evasion = 0;
        if (speed<= 0) speed = 1;
        s->speed = speed;

        gain_experience(s, experience);

        // Charger les moves
        for (int j = 0; j < MAX_MOVES; j++)
        {
            char move_name[MAX_NAME_LENGTH];
            if (fscanf(file, "%12s ", move_name) != 1)
            {
                fclose(file);
                return 0;
            }

            if (strcmp(move_name, "NONE") == 0)
                s->moves[j] = NULL;
            else if (strcmp(move_name, "Scratch") == 0)
                s->moves[j] = &SCRATCH;
            else if (strcmp(move_name, "Pound") == 0)
                s->moves[j] = &POUND;
            else if (strcmp(move_name, "Growl") == 0)
                s->moves[j] = &GRAWL;
            else if (strcmp(move_name, "Foliage") == 0)
                s->moves[j] = &FOLIAGE;
            else if (strcmp(move_name, "Shell") == 0)
                s->moves[j] = &SHELL;
            else
                s->moves[j] = NULL;
        }
        fscanf(file, "\n");
    }

    // Initialiser les slots vides
    for (int i = supemon_count; i < MAX_SUPEMONS; i++)
    {
        memset(&player->supemons[i], 0, sizeof(Supemon));
    }

    // 6. Items
    int item_count;
    if (fscanf(file, "%d\n", &item_count) != 1)
    {
        fclose(file);
        return 0;
    }

    if (item_count < 0 || item_count > MAX_ITEMS)
    {
            fclose(file);
            return 0;
    }

    for (int i = 0; i < item_count; i++)
    {
        int item_code;
        if (fscanf(file, "%d ", &item_code) != 1)
        {
            fclose(file);
            return 0;
        }

        switch (item_code)
        {
            case 1: player->items[i] = &POTION; break;
            case 2: player->items[i] = &SUPER_POTION; break;
            case 3: player->items[i] = &RARE_CANDY; break;
            default: player->items[i] = NULL; break;
        }
    }

    // Initialiser les items restants
    for (int i = item_count; i < MAX_ITEMS; i++)
    {
        player->items[i] = NULL;
    }

    fclose(file);
    return 1;
}
