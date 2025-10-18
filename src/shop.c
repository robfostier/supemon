#include <stdio.h>

#include "shop.h"
#include "utils.h"

void go_to_shop(Player* player)
{
    const Item* catalog[] = {&POTION, &SUPER_POTION, &RARE_CANDY};
    int catalog_count = 3;
    
    npc_dialog("\"Welcome to the Supemart ! All your item needs fulfilled !\" says vendor Na.\n", 20);

    while (1) // Stay in shop so long as player doesn't Leave
    {
        int item_count = get_item_count(player);

        printf("+-------------------------------+\n");
        printf("| What do you wish to do ?      |\n");
        printf("|   1. See what's in store      |\n");
        printf("|   2. Sell your items          |\n");
        printf("|   3. Leave the Mart           |\n");
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
                // Avoid buying more than can carry
                if (item_count >= MAX_ITEMS)
                {
                    npc_dialog("Your pockets are already full ! Come back later !\" says vendor Na.\n", 20);
                    break;
                }

                printf("+-------------------------------+\n");
                printf("| What do you wish to buy ?     |\n");
                for (int i = 0; i < catalog_count; i++)
                {
                    printf("|   %d. %s       - %d coins |\n", i + 1, catalog[i]->name, catalog[i]->price);
                }
                printf("|   %d. Cancel                  |\n", catalog_count + 1);
                printf("+-------------------------------+\n");

                int buy_option = 0;
                while (buy_option < 1 || buy_option > catalog_count + 1)
                {
                    printf("\nYOUR CHOICE: ");
                    scanf("%d", &buy_option);
                }

                printf("\n");

                if (buy_option == catalog_count + 1) break; // Player choose to cancel
                
                if (player->coins >= catalog[buy_option - 1]->price) // Player has enough coins to buy
                {
                    if (add_item(player, catalog[buy_option - 1]) == 0) return; // Something went wrong, leave shop
                    player->coins -= catalog[buy_option - 1]->price; // Buy

                    printf("You bought 1 %s for %d Supcoins.", catalog[buy_option - 1]->name, catalog[buy_option - 1]->price);

                    break;
                } else npc_dialog("You don't have enough Supcoins for this. Come back when you have more !\" says vendor Na.\n", 20); break; // Player doesn't have enough coins

                return; // Something went wrong, leave shop
            }
            case 2:
            {
                if (item_count == 0)
                {
                    npc_dialog("You don't have any item to sell ! Come back later !\" says vendor Na.\n", 20);
                    break;
                }

                printf("+-------------------------------+\n");
                printf("| What do you wish to sell ?    |\n");
                for (int i = 0; i < item_count; i++)
                {
                    printf("|   %d. %s       - %d coins |\n", i + 1, player->items[i]->name, player->items[i]->price / 2);
                }
                printf("|   %d. Cancel                  |\n", catalog_count + 1);
                printf("+-------------------------------+\n");

                int sell_option = 0;
                while (sell_option < 1 || sell_option > item_count + 1)
                {
                    printf("\nYOUR CHOICE: ");
                    scanf("%d", &sell_option);
                }

                printf("\n");

                if (sell_option == item_count + 1) break; // Player choose to cancel

                remove_item(player, sell_option - 1);
                player->coins += player->items[sell_option - 1]->price / 2; // Sell

                break;
            }
            case 3: return;
            default: return; // Something went wrong, leave Shop
        }
    }
}