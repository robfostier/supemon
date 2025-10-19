#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "battle.h"
#include "global.h"
#include "utils.h"
#include "player.h"
#include "supemon.h"
#include "moves.h"
#include "items.h"

int go_to_battle(Player* player)
{
    Supemon* active = get_active_supemon(player);
    if (active->health <= 0)
    {
        printf("Your active Supemon cannot fight in this condition. Please visit the Supemon Center.\n");
        npc_dialog("...", 1000);
        printf("\n");
        return 0;
    }

    clear_terminal();
    Supemon foe;

    const Supemon* random_mon = get_random_supemon_template();
    if (random_mon != NULL) foe = init_supemon(random_mon, active->level);
    else return 0; // Error on random init
    
    // Determine who goes first based on Speed
    int player_first;
    if (active->speed > foe.speed)
    {
        player_first = 1;
        printf("Your %s is faster and will attack first!\n\n", active->name);
    }
    else if (active->speed < foe.speed)
    {
        player_first = 0;
        printf("Enemy %s is faster and will attack first!\n\n", foe.name);
    }
    else // Equal speed, random
    {
        player_first = (rand() % 2);
        if (player_first) printf("Your %s will attack first!\n\n", active->name);
        else printf("Enemy %s will attack first!\n\n", foe.name);
    }

    int used_item_count = 0;

    while (1)
    {
        if (player_first)
        {
            // Player goes first
            int turn_result = player_turn(player, &foe, used_item_count); // 0 if battle still ongoing, 1 if turn ends battle, 2 if player used an item
            if (turn_result == 1)
            {
                leave_battle(player);
                return 1;
            }
            else if (turn_result == 2) // If an item was used, player gets another turn
            {
                used_item_count++;
                if (player_turn(player, &foe, BATTLE_ITEM_LIMIT + 1) == 1) // Player can't use 2 items in a single turn, so we pass 5 for item_count
                {
                    leave_battle(player);
                    return 1;
                }
            }

            if (foe_turn(player, &foe) == 1) // 0 if battle still ongoing, 1 if turn ends battle
            {
                leave_battle(player);
                return 1;
            }
        }
        else
        {
            // Foe goes first
            if (foe_turn(player, &foe) == 1) // 0 if battle still ongoing, 1 if turn ends battle
            {
                leave_battle(player);
                return 1;
            }
            
            int turn_result = player_turn(player, &foe, used_item_count); // 0 if battle still ongoing, 1 if turn ends battle, 2 if player used an item
            if (turn_result == 1)
            {
                leave_battle(player);
                return 1;
            }
            else if (turn_result == 2) // If an item was used, player gets another turn
            {
                used_item_count++;
                if (player_turn(player, &foe, BATTLE_ITEM_LIMIT + 1) == 1) // Player can't use 2 items in a single turn, so we pass 5 for item_count
                {
                    leave_battle(player);
                    return 1;
                }
            }
        }     
    }
}

void display_battle(Player* player, Supemon* foe)
{
    Supemon* active = get_active_supemon(player);
    
    display_supemon(foe, "enemy");
    display_supemon(active, player->name);
}

void battle_rewards(Player* player, Supemon* foe)
{
    Supemon* active = get_active_supemon(player);
    int gained_coins = rand() % 401 + 100; // rand % 401 -> (0, ..., 400), + 100 = (100, ..., 500)
    int gained_xp = (rand() % 401 + 100) * foe->level; // (100, ..., 500) * enemy level

    player->coins += gained_coins;
    gain_experience(active, gained_xp);

    printf("%s gained %d experience points.\n", active->name, gained_xp);
    npc_dialog("\n....", 1000);
}

int player_turn(Player* player, Supemon* foe, int used_item_count)
{
    printf("\nYour turn...\n\n");
    
    display_battle(player, foe);

    printf("+------------------------------+\n");
    printf("| What will you do ?           |\n");
    printf("|   1. Move                    |\n");
    printf("|   2. Change active Supemon   |\n");
    printf("|   3. Use item                |\n");
    printf("|   4. Capture                 |\n");
    printf("|   5. Run away                |\n");
    printf("+------------------------------+\n\n");

    int choice = 0;
    while (choice < 1 || choice > 5)
    {
        printf("YOUR CHOICE (1, 2, 3, 4 or 5): ");
        scanf("%d", &choice);
    }

    printf("\n");

    Supemon* active = get_active_supemon(player);

    switch (choice)
    {
        case 1:
        {
            // Print all moves
            int move_count;
            for (move_count = 0; move_count < MAX_MOVES; move_count++)
            {
                const Move* move = active->moves[move_count];
                if (move == NULL) break;
                printf("%d - %s\n", move_count + 1, move->name);
            }

            printf("%d - Cancel\n\n", move_count + 1);

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
            apply_move(active->moves[chosen_move - 1], active, foe);

            if (foe->health > 0) return 0; // Battle can continue

            clear_terminal();
            printf("%s fainted. %s won the fight !\n", foe->name, active->name);
            battle_rewards(player, foe);
            return 1; // Battle needs to stop if foe fainted
        }
        case 2:
        {
            int supemon_count = display_supemons(player);
            printf("%d - Cancel\n\n", supemon_count + 1);

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
        }
        case 3:
        {
            if (used_item_count < BATTLE_ITEM_LIMIT) // Can only use item if hasn't already used 4 during battle
            {
                int item_count = display_items(player); // display_items() also returns the count, better than just get_item_count() here
                printf("%d - Cancel\n\n", item_count + 1);

                int chosen_item = 0;
                while (chosen_item < 1 || chosen_item > item_count + 1)
                {
                    printf("YOUR CHOICE: ");
                    scanf("%d", &chosen_item);
                }
                printf("\n");

                if (chosen_item == item_count + 1) break; // Player cancelled action, needs to abort turn

                if (use_item(player, active, chosen_item - 1) == 0) return 0; // Something went wrong, end turn

                return 2; // Player used item, can play one more turn
            }
            else
            {
                printf("Can't use item !\n");
                break; // Player can't use item, abort turn
            }
        }
        case 4:
        {
            float rnd = (float)rand() / RAND_MAX;
            float health_ratio = (float)(foe->max_health - foe->health) / foe->max_health;
            float rate = health_ratio - 0.5f;

            npc_dialog("...", 1000);
            printf("\n\n");

            if (rnd <= rate)
            {
                add_supemon(player, *foe);
                
                clear_terminal();

                printf("Successfully captured %s !\n", foe->name);
                battle_rewards(player, foe);
                return 1; // Foe captured, battle needs to end
            }
            else{
                printf("Capture failed... (%.2f/%.2f)\n\n", rate, rnd);
                return 0; // Battle can continue
            }
        }
        case 5:
        {
            float rnd = (float)rand() / RAND_MAX;
            float rate = (float)active->speed / (active->speed + foe->speed); // Don't forget to cast ints to floats

            if (rnd <= rate)
            {
                clear_terminal();
                printf("You ran away !\n\n");
                npc_dialog("...", 1000);
                return 1; // Ran away, battle needs to end
            }
            else
            {
                printf("You failed to run away from the battle... (%.2f/%.2f)\n\n", rate, rnd);
                return 0; // Battle can continue
            }
        }
        default: break;
    }

    return player_turn(player, foe, used_item_count); // If we reach here, turn was aborted, start another one
}

int foe_turn(Player* player, Supemon* foe)
{
    printf("\nEnemy turn...\n\n");

    Supemon* active = get_active_supemon(player);

    // Get foe move count
    int move_count;
    for (move_count = 0; move_count < MAX_MOVES; move_count++)
    {
        const Move* move = foe->moves[move_count];
        if (move == NULL) break;
    }
    if (move_count == 0) return 1; // Foe doesn't have any move, leave battle

    // Pick a random move
    int rnd = rand() % move_count;
    const Move* chosen_move = foe->moves[rnd];

    apply_move(chosen_move, foe, active);

    if (active->health > 0) return 0; // Battle can continue
    clear_terminal();
    printf("%s fainted. %s won the fight !\n", active->name, foe->name);
    npc_dialog("...", 1000);
    return 1; // Battle needs to stop if player's active supemon has fainted
}

void apply_move(const Move* move, Supemon* attacker, Supemon* target)
{
    switch(move->type)
    {
        case MOVE_OFFENSIVE:
        {
            if (move->damage > 0)
            {
                float a_atk = (float)attacker->attack + attacker->base_attack;
                float a_acc = (float)attacker->accuracy + attacker->base_accuracy;
                
                float t_def = (float)target->defense + target->base_defense;
                float t_eva = (float)target->evasion + target->base_evasion;

                if (t_def <= .0f) t_def = 1.0f; // Avoid div by 0 if target's def was reduced

                float dmg = (a_atk * move->damage) / t_def;
                
                // 1/2 chance to round up if not an integer
                if (floorf(dmg) != dmg)
                {
                    dmg = floorf(dmg);
                    if ((float)rand() / RAND_MAX >= .5f) dmg += 1.0f;
                }

                float dodge_rate = a_acc / (a_acc + t_eva) + .1f;

                //printf("%f atk, %f def, %f acc, %f eva, %f dmg", a_atk, t_def, a_acc, t_eva, dmg);

                if ((float)rand() / RAND_MAX > dodge_rate)
                {
                    update_health(target, -(int)dmg);
                    printf("%s used %s. %s takes %d damage.\n", attacker->name, move->name, target->name, (int)dmg);
                }
                else printf("%s dodged %s's attack.\n", target->name, attacker->name);
            }
            break;
        }
        case MOVE_ATTACK:
        {
            if (move->damage > 0)
            {
                attacker->attack += move->damage;
                printf("%s used %s. %s's attack was increased by %d.\n", attacker->name, move->name, attacker->name, move->damage);
            }
            else
            {
                target->attack += move->damage;
                printf("%s used %s. %s's attack was reduced by %d.\n", attacker->name, move->name, target->name, - move->damage);
            }
            break;
        }
        case MOVE_DEFENSE:
        {
            if (move->damage > 0)
            {
                attacker->defense += move->damage;
                printf("%s used %s. %s's defense was increased by %d.\n", attacker->name, move->name, attacker->name, move->damage);
            }
            else
            {
                target->defense += move->damage;
                printf("%s used %s. %s's defense was reduced by %d.\n", attacker->name, move->name, target->name, - move->damage);
            }
            break;
        }
        case MOVE_ACCURACY:
        {
            if (move->damage > 0)
            {
                attacker->accuracy += move->damage;
                printf("%s used %s. %s's accuracy was increased by %d.\n", attacker->name, move->name, attacker->name, move->damage);
            }
            else
            {
                target->accuracy += move->damage;
                printf("%s used %s. %s's accuracy was reduced by %d.\n", attacker->name, move->name, target->name, - move->damage);
            }
            break;
        }
        case MOVE_EVASION:
        {
            if (move->damage > 0)
            {
                attacker->evasion += move->damage;
                printf("%s used %s. %s's evasion was increased by %d.\n", attacker->name, move->name, attacker->name, move->damage);
            }
            else
            {
                target->evasion += move->damage;
                printf("%s used %s. %s's evasion was reduced by %d.\n", attacker->name, move->name, target->name, - move->damage);
            }
            break;
        }
        default: break;
    }
}

void leave_battle(Player* player)
{
    for (int i = 0; i < MAX_SUPEMONS; i++)
    {
        if (player->supemons[i].level == 0) break;

        player->supemons[i].attack = 0;
        player->supemons[i].defense = 0;
        player->supemons[i].evasion = 0;
        player->supemons[i].accuracy = 0;
    }
}