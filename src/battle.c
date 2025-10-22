#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "battle.h"
#include "global.h"
#include "utils.h"
#include "colors.h"
#include "player.h"
#include "supemon.h"
#include "moves.h"
#include "items.h"

int go_to_battle(Player* player)
{
    Supemon* active = get_active_supemon(player);
    if (active->health <= 0)
    {
        print_colored(COLOR_ERROR, "Your active Supemon cannot fight in this condition. Please visit the Supemon Center.\n");
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
        printf("Your %s%s%s is faster and will attack first!\n\n", type_to_color(active->type), active->name, RESET);
    }
    else if (active->speed < foe.speed)
    {
        player_first = 0;
        printf("Enemy %s%s%s is faster and will attack first!\n\n", type_to_color(foe.type), foe.name, RESET);
    }
    else // Equal speed, random
    {
        player_first = (rand() % 2);
        if (player_first) printf("Your %s%s%s will attack first!\n\n", type_to_color(active->type), active->name, RESET);
        else printf("Enemy %s%s%s will attack first!\n\n", type_to_color(foe.type), foe.name, RESET);
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

    printf("%s%s%s gained ", type_to_color(active->type), active->name, RESET);
    printf("%s+%d XP%s", COLOR_XP, gained_xp, RESET);
    printf(" and ");
    printf("%s+%d coins%s!\n", COLOR_MONEY, gained_coins, RESET);
    npc_dialog("...", 1000);
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

    int choice = get_input_counted(1, 5);

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
            int move_choice = get_input_counted(1, move_count + 1);

            if (move_choice == move_count + 1) break; // Player cancelled move, needs to abort turn

            // Apply the move
            apply_move(active->moves[move_choice - 1], active, foe);

            if (foe->health > 0) return 0; // Battle can continue

            clear_terminal();
            printf("%s%s%s fainted. ", type_to_color(foe->type), foe->name, RESET);
            printf("%s%s%s won the fight!\n", type_to_color(active->type), active->name, RESET);
            battle_rewards(player, foe);
            return 1; // Battle needs to stop if foe fainted
        }
        case 2:
        {
            int supemon_count = display_supemons(player);
            printf("%d - Cancel\n\n", supemon_count + 1);

            int mon_choice = get_input_counted(1, supemon_count + 1);

            if (mon_choice == supemon_count + 1) break; // Player cancelled action, needs to abort turn

            set_active_supemon(player, mon_choice - 1);

            return 0; // Player switched Supemon, battle can continue
        }
        case 3:
        {
            if (used_item_count < BATTLE_ITEM_LIMIT) // Can only use item if hasn't already used 4 during battle
            {
                int item_count = display_items(player); // display_items() also returns the count, better than just get_item_count() here
                printf("%d - Cancel\n\n", item_count + 1);

                int item_choice = get_input_counted(1, item_count + 1);

                if (item_choice == item_count + 1) break; // Player cancelled action, needs to abort turn

                if (use_item(player, active, item_choice - 1) == 0) return 0; // Something went wrong, end turn

                return 2; // Player used item, can play one more turn
            }
            else
            {
                print_colored(COLOR_ERROR, "Can't use item !\n");
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

                printf("%sSuccessfully captured%s %s%s%s!\n", COLOR_SUCCESS, RESET, type_to_color(foe->type), foe->name, RESET);
                battle_rewards(player, foe);
                return 1; // Foe captured, battle needs to end
            }
            else{
                printf("%sCapture failed...%s (%.2f/%.2f)\n\n", COLOR_ERROR, RESET, rate, rnd);
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
                print_colored(COLOR_SUCCESS, "You ran away !\n\n");
                npc_dialog("...", 1000);
                return 1; // Ran away, battle needs to end
            }
            else
            {
                printf("%sYou failed to run away from the battle...%s (%.2f/%.2f)\n\n", COLOR_ERROR, RESET, rate, rnd);
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
    printf("%s%s%s fainted. ", type_to_color(active->type), active->name, RESET);
    printf("%s%s%s won the fight!\n", type_to_color(foe->type), foe->name, RESET);
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
                float a_atk = (float)get_battle_stat(attacker, 'A');
                float a_acc = (float)get_battle_stat(attacker, 'P');
                
                float t_def = (float)get_battle_stat(target, 'D');
                float t_eva = (float)get_battle_stat(target, 'E');

                if (t_def <= .0f) t_def = 1.0f; // Avoid div by 0 if target's def was reduced

                float dmg = (a_atk * move->damage) / t_def;
                
                // 1/2 chance to round up if not an integer
                if (floorf(dmg) != dmg)
                {
                    dmg = floorf(dmg);
                    if ((float)rand() / RAND_MAX >= .5f) dmg += 1.0f;
                }

                float success_rate = a_acc / (a_acc + t_eva) + .1f;

                //printf("%f a_acc / (%f a_acc + %f t_acc) + 0,1 = %f\n", a_acc, a_acc, t_eva, success_rate);

                 if ((float)rand() / RAND_MAX <= success_rate)
                {
                    update_health(target, -(int)dmg);
                    printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                    printf("%s takes ", target->name);
                    printf("%s-%d HP%s.\n", COLOR_DAMAGE, (int)dmg, RESET);
                }
                else {
                    printf("%s dodged ", target->name);
                    printf("%s's attack!\n", attacker->name);
                }
            }
            break;
        }
        case MOVE_ATTACK:
        {
            if (move->damage > 0)
            {
                attacker->attack += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sAttack +%d%s!\n", attacker->name, COLOR_STAT_UP, move->damage, RESET);
            }
            else
            {
                target->attack += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sAttack -%d%s!\n", target->name, COLOR_STAT_DOWN, -move->damage, RESET);
            }
            break;
        }
        case MOVE_DEFENSE:
        {
            if (move->damage > 0)
            {
                attacker->defense += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sDefense +%d%s!\n", attacker->name, COLOR_STAT_UP, move->damage, RESET);
            }
            else
            {
                target->defense += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sDefense -%d%s!\n", target->name, COLOR_STAT_DOWN, -move->damage, RESET);
            }
            break;
        }
        case MOVE_ACCURACY:
        {
            if (move->damage > 0)
            {
                attacker->accuracy += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sAccuracy +%d%s!\n", attacker->name, COLOR_STAT_UP, move->damage, RESET);
            }
            else
            {
                target->accuracy += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sAccuracy -%d%s!\n", target->name, COLOR_STAT_DOWN, -move->damage, RESET);
            }
            break;
        }
        case MOVE_EVASION:
        {
            if (move->damage > 0)
            {
                attacker->evasion += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sEvasion +%d%s!\n", attacker->name, COLOR_STAT_UP, move->damage, RESET);
            }
            else
            {
                target->evasion += move->damage;
                printf("%s used %s%s%s. ", attacker->name, BOLD, move->name, RESET);
                printf("%s's %sEvasion -%d%s!\n", target->name, COLOR_STAT_DOWN, -move->damage, RESET);
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