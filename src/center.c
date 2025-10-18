#include <stdio.h>

#include "center.h"
#include "player.h"
#include "utils.h"

void go_to_center(Player* player)
{
    npc_dialog("\"Welcome to the Supemon Center ! We heal your Supemon for free !\" says nurse Ornella.\n", 20);

    printf("+-------------------------------+\n");
    printf("| What do you wish to do ?      |\n");
    printf("|   1. Heal all Supemons        |\n");
    printf("|   2. Change active Supemon    |\n");
    printf("|   3. Leave the Center         |\n");
    printf("+-------------------------------+\n");

    int choice = 0;
    while (choice < 1 || choice > 3)
    {
        printf("\nYOUR CHOICE (1, 2 or 3): ");
        scanf("%d", &choice);
    }

    printf("\n");

    switch (choice)
    {
        case 1:
        {
            break;
        }
        case 2:
        {
            break;
        }
        case 3:
        {
            break;
        }
        default: break;
    }
}