#include <stdio.h>
#include <string.h>

#include "shop.h"
#include "utils.h"
#include "colors.h"

void go_to_shop(Player* player)
{
    clear_terminal();

    const Item* catalog[] = {&POTION, &SUPER_POTION, &RARE_CANDY};
    int catalog_count = sizeof(catalog) / sizeof(catalog[0]);

    printf("%s", COLOR_NPC);
    npc_dialog("\"Welcome to the Supemart ! All your item needs fulfilled !\" says vendor Na.\n", 20);
    printf("%s", RESET);

    while (1) // Stay in shop so long as player doesn't Leave
    {
        int item_count = get_item_count(player);

        printf("\n+------------------------------+\n");
        printf("| What do you wish to do ?     |\n");
        printf("|   1. See what's in store     |\n");
        printf("|   2. Sell your items         |\n");
        printf("|   3. Leave the Mart          |\n");
        printf("+----------------%s[%4d Supcoins]%s\n", COLOR_MONEY, player->coins, RESET);

        int choice = get_input_counted(1, 3);

        switch (choice)
        {
            case 1:
            {
                // Avoid buying more than can carry
                if (item_count >= MAX_ITEMS)
                {
                    printf("%s", COLOR_NPC);
                    npc_dialog("\"Your pockets are already full ! Come back later !\" says vendor Na.\n", 20);
                    printf("%s", RESET);
                    break;
                }

                printf("+------------------------------+\n");
                printf("| What do you wish to buy ?    |\n");
                for (int i = 0; i < catalog_count; i++)
                {
                    char formatted_line[BOX_WIDTH + 1]; // buffer pour store la ligne à imprimer, de taille box_width + '\0'

                    snprintf(formatted_line, sizeof(formatted_line), "%3d. %-*s - %3d coins ",
                            i + 1, 12, catalog[i]->name, catalog[i]->price); // Should be exactly 30 chars : 3 + 2 + 12 + 3 + 3 + 7
                    
                    int len = strlen(formatted_line); 
                    for (int j = len; j < BOX_WIDTH; j++) formatted_line[j] = ' '; // In case we change box_width size

                    formatted_line[BOX_WIDTH] = '\0';
                    
                    printf("|%s|\n", formatted_line);
                }
                printf("|%3d. Cancel                   |\n", catalog_count + 1);
                printf("+----------------%s[%4d Supcoins]%s\n", COLOR_MONEY, player->coins, RESET);

                int buy_option = get_input_counted(1, catalog_count + 1);

                if (buy_option == catalog_count + 1) break; // Player choose to cancel
                
                if (player->coins >= catalog[buy_option - 1]->price) // Player has enough coins to buy
                {
                    if (add_item(player, catalog[buy_option - 1]) == 0) return; // Something went wrong, leave shop
                    player->coins -= catalog[buy_option - 1]->price; // Buy

                    printf("%sYou bought 1 %s for %s%d Supcoins%s.%s\n", COLOR_SUCCESS, catalog[buy_option - 1]->name, COLOR_MONEY, catalog[buy_option - 1]->price, RESET, RESET);

                    break;
                } 
                else
                {
                    printf("%s", COLOR_NPC);
                    npc_dialog("\"You don't have enough Supcoins for this. Come back when you have more !\" says vendor Na.\n", 20);
                    printf("%s", RESET);
                }

                return; // Something went wrong, leave shop
            }
            case 2:
            {
                if (item_count == 0)
                {
                    printf("%s", COLOR_NPC);
                    npc_dialog("\"You don't have any item to sell ! Come back later !\" says vendor Na.\n", 20);
                    printf("%s", RESET);
                    break;
                }

                printf("+------------------------------+\n");
                printf("| What do you wish to sell ?   |\n");
                for (int i = 0; i < item_count; i++)
                {
                    char formatted_line[BOX_WIDTH + 1]; // buffer pour store la ligne à imprimer, de taille box_width + '\0'

                    snprintf(formatted_line, sizeof(formatted_line), "%3d. %-*s - %3d coins ",
                            i + 1, 12, player->items[i]->name, player->items[i]->price / 2); // Should be exactly 30 chars : 3 + 2 + 12 + 3 + 3 + 7
                    
                    int len = strlen(formatted_line); 
                    for (int j = len; j < BOX_WIDTH; j++) formatted_line[j] = ' '; // In case we change box_width size

                    formatted_line[BOX_WIDTH] = '\0';
                    
                    printf("|%s|\n", formatted_line);
                }
                printf("|%3d. Cancel                   |\n", item_count + 1);
                printf("+----------------%s[%4d Supcoins]%s\n", COLOR_MONEY, player->coins, RESET);

                int sell_option = get_input_counted(1, item_count + 1);

                if (sell_option == item_count + 1) break; // Player choose to cancel

                // Sell
                int sell_price = player->items[sell_option - 1]->price / 2;
                printf("%sYou sold 1 %s for %s+%d Supcoins%s.%s\n", COLOR_SUCCESS, player->items[sell_option - 1]->name, COLOR_MONEY, sell_price, RESET, RESET);
                player->coins += sell_price;
                remove_item(player, sell_option - 1);

                break;
            }
            case 3: return;
            default: return; // Something went wrong, leave Shop
        }
    }
}