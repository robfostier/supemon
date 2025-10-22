#include <stdio.h>

#include "center.h"
#include "player.h"
#include "utils.h"
#include "colors.h"

void go_to_center(Player* player)
{
    clear_terminal();
    printf("%s", COLOR_NPC);
    npc_dialog("\"Welcome to the Supemon Center ! We heal your Supemon for free !\" says nurse Ornella.\n\n", 20);
    printf("%s", RESET);

    // Boucle pour rester dans le centre jusqu'à ce que le joueur parte
    while (1)
    {
        printf("+------------------------------+\n");
        printf("| What do you wish to do ?     |\n");
        printf("|   1. Heal all Supemons       |\n");
        printf("|   2. Change active Supemon   |\n");
        printf("|   3. Leave the Center        |\n");
        printf("+------------------------------+\n");

        int choice = get_input_counted(1, 3);

        printf("\n");

        switch (choice)
        {
            case 1:  // Heal all Supemons
            {
                // Parcourir tous les Supémons du joueur
                for (int i = 0; i < MAX_SUPEMONS; i++)
                {
                    // Si le slot est vide (level == 0), arrêter la boucle
                    if (player->supemons[i].level == 0) break;

                    // Restaurer les HP au maximum
                    player->supemons[i].health = player->supemons[i].max_health;
                }

                // Message de confirmation
                npc_dialog("...", 500);
                printf("\n\n");
                printf("%s", COLOR_NPC);
                npc_dialog("\"Your Supemons have been fully healed ! Come back anytime !\" says nurse Ornella.\n\n", 20);
                printf("%s", RESET);

                break;
            }
            case 2:  // Change active Supemon
            {
                // Afficher tous les Supémons disponibles
                int supemon_count = display_supemons(player);
                printf("%d - Cancel\n", supemon_count + 1);

                int active_choice = get_input_counted(1, supemon_count + 1);

                if (active_choice == supemon_count + 1) break; // On cancel

                if (player->active_index != active_choice - 1)
                {
                    set_active_supemon(player, active_choice - 1);
                    printf("%sActive Supemon changed to %s%s%s!%s\n\n", COLOR_SUCCESS, type_to_color(get_active_supemon(player)->type), get_active_supemon(player)->name, COLOR_SUCCESS, RESET);
                }
                else
                {
                    printf("%s%s%s is already your active Supemon.%s\n\n", type_to_color(get_active_supemon(player)->type), get_active_supemon(player)->name, COLOR_WARNING, RESET);
                }

                break;
            }
            case 3: return;
            default: break;
        }
    }
}