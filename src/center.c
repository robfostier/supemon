#include <stdio.h>

#include "center.h"
#include "player.h"
#include "utils.h"

void go_to_center(Player* player)
{
    clear_terminal();
    npc_dialog("\"Welcome to the Supemon Center ! We heal your Supemon for free !\" says nurse Ornella.\n", 20);

    // Boucle pour rester dans le centre jusqu'à ce que le joueur parte
    while (1)
    {
        printf("\n+------------------------------+\n");
        printf("| What do you wish to do ?     |\n");
        printf("|   1. Heal all Supemons       |\n");
        printf("|   2. Change active Supemon   |\n");
        printf("|   3. Leave the Center        |\n");
        printf("+------------------------------+\n");

        int choice = 0;
        while (choice < 1 || choice > 3)
        {
            printf("\nYOUR CHOICE (1, 2 or 3): ");
            scanf("%d", &choice);
        }

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
                npc_dialog("\"Your Supemons have been fully healed ! Come back anytime !\" says nurse Ornella.\n\n", 20);

                break;
            }
            case 2:  // Change active Supemon
            {
                // Afficher tous les Supémons disponibles
                int supemon_count = display_supemons(player);
                printf("%d - Cancel\n\n", supemon_count + 1);

                // Demander au joueur de choisir
                int chosen = 0;
                while (chosen < 1 || chosen > supemon_count + 1)
                {
                    printf("YOUR CHOICE: ");
                    scanf("%d", &chosen);
                }

                printf("\n");

                // Si le joueur annule
                if (chosen == supemon_count + 1) break;

                // Changer le Supémon actif (index - 1 car tableau commence à 0)
                set_active_supemon(player, chosen - 1);

                // Confirmation
                printf("Active Supemon changed to %s !\n\n", player->supemons[chosen - 1].name);

                break;
            }
            case 3:  // Leave the Center
            {
                npc_dialog("\"See you next time !\" says nurse Ornella.\n\n", 20);
                return;  // Quitter la fonction et la boucle
            }
            default: break;
        }
    }
}