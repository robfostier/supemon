#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "global.h"
#include "player.h"
#include "supemon.h"
#include "battle.h"

void go_to_battle(Player* player)
{
    Supemon foe;

    Supemon* random_mon = get_random_supemon_template();
    if (random_mon != NULL) foe = init_supemon(random_mon, get_active_supemon(player)->level);
    else return 0; // Error on random init
    
    int item_count = 0;

    while (1)
    {
        int turn_result = player_turn(player, &foe, item_count); // 0 if battle still ongoing, 1 if turn ends battle, 2 if player used an item
        if (turn_result == 1)
        {
            battle_rewards(player, &foe);
            return;
        }
        else if (turn_result == 2) // If an item was used, player gets another turn
        {
            if (player_turn(player, &foe, 5) == 1) // Player can't use 2 items in a single turn, so we pass 5 for item_count
            {
                battle_rewards(player, &foe);
                return;
            }
        }

        if (foe_turn(player, &foe) == 1) return; // 0 if battle still ongoing, 1 if turn ends battle
    }
}

void display_battle(Player* player, Supemon* foe); // TO DO

void battle_rewards(Player* player, Supemon* foe)
{
    srand(time(NULL));
    int gained_coins = rand() % 401 + 100; // rand % 401 -> (0, ..., 400), + 100 = (100, ..., 500)
    int gained_xp = (rand() % 401 + 100) * foe->level; // (100, ..., 500) * enemy level

    player->coins += gained_coins;
    gain_experience(get_active_supemon(player), gained_xp);
}

int player_turn(Player* player, Supemon* foe, int item_count)
{
    printf("Your turn...\n");
    
    display_battle(player, foe);

    printf("+-------------------------------+\n");
    printf("| What will you do ?            |\n");
    printf("|   1. Move                     |\n");
    printf("|   2. Change active Supemon    |\n");
    printf("|   3. Use item                 |\n");
    printf("|   4. Capture                  |\n");
    printf("|   5. Run away                 |\n");
    printf("+-------------------------------+\n");

    int choice = 0;
    while (choice < 1 || choice > 5)
    {
        printf("YOUR CHOICE (1, 2, 3, 4 or 5): ");
        scanf("%d", &choice);
    }

    printf("\n");

    switch (choice)
    {
        case 1:
            // Print all moves
            int move_count;
            for (move_count = 0; move_count < MAX_MOVES; move_count++)
            {
                Move* move = get_active_supemon(player)->moves[move_count];
                if (move == NULL) break;
                printf(move_count + 1, " - ", move->name, "\n");
            }

            print(move_count + 1, " - Cancel\n");

            // Player selects a move
            int chosen_move = 0;
            while (chosen_move < 1 || chosen_move > move_count + 1)
            {
                printf("YOUR CHOICE: ");
                scanf("%d", &chosen_move);
            }
            printf("\n");

            if (chosen_move == move_count + 1) break; // Player cancelled move, needs to abort turn

            // Apply the move
            apply_move(get_active_supemon(player)->moves[chosen_move], get_active_supemon(player), foe);

            if (foe->health > 0) return 0; // Battle can continue
            return 1; // Battle needs to stop if foe fainted

        case 2:
            int supemon_count = display_supemons(player);
            print(supemon_count + 1, " - Cancel\n");

            int chosen_mon = 0;
            while (chosen_mon < 1 || chosen_mon > supemon_count + 1)
            {
                printf("YOUR CHOICE: ");
                scanf("%d", &chosen_mon);
            }
            printf("\n");

            if (chosen_mon == supemon_count + 1) break; // Player cancelled action, needs to abort turn

            set_active_supemon(player, chosen_mon - 1);

            return 0; // Player switched Supemon, battle can continue

        case 3:
            if (item_count < 4) // Can only use item if hasn't already used 4 during battle
            {
                int item_count = display_items(player);
                print(item_count + 1, " - Cancel\n");

                int chosen_item = 0;
                while (chosen_item < 1 || chosen_item > item_count + 1)
                {
                    printf("YOUR CHOICE: ");
                    scanf("%d", &chosen_item);
                }
                printf("\n");

                if (chosen_item == item_count + 1) break; // Player cancelled action, needs to abort turn

                Item* item = player->items[chosen_item - 1];

                // TO DO : IMPLEMENT ITEMS

                item_count++;
                return 2; // Player used item, can play one more turn
            }
            else
            {
                printf("Can't use item !\n");
                break; // Player can't use item, abort turn
            }

        case 4:
            srand(time(NULL));
            float rnd = (float)rand() / RAND_MAX;

            if (rnd <= (foe->max_health - foe->health) / foe->max_health - .5f)
            {
                add_supemon(player, *foe);
                printf("Successfully captured ", foe->name, " !\n");
                return 1; // Foe captured, battle needs to end
            }
            else{
                printf("Capture failed...");
                return 0; // Battle can continue
            }

        case 5:
            srand(time(NULL));
            float rnd = (float)rand() / RAND_MAX;

            if (rnd <= get_active_supemon(player)->speed / (get_active_supemon(player)->speed + foe->speed))
            {
                printf("You ran away !\n");
                return 1; // Ran away, battle needs to end
            }
            else
            {
                printf("You failed to run away from the battle...");
                return 0; // Battle can continue
            }

        default: break;
    }

    return player_turn(player, foe, item_count); // If we reach here, turn was aborted, start another one
}

int foe_turn(Player* player, Supemon* foe)
{
    // Get foe move count
    int move_count;
    for (move_count = 0; move_count < MAX_MOVES; move_count++)
    {
        Move* move = foe->moves[move_count];
        if (move == NULL) break;
    }
    if (move_count == 0) return 1; // Foe doesn't have any move, leave battle

    // Pick a random move
    srand(time(NULL));
    int rnd = rand() % move_count;
    Move* chosen_move = foe->moves[rnd];

    apply_move(chosen_move, foe, get_active_supemon(player));

    if (get_active_supemon(player)->health > 0) return 0; // Battle can continue
    return 1; // Battle needs to stop if player's active supemon has fainted
}

void apply_move(Move* move, Supemon* attacker, Supemon* target);