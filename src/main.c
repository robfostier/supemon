#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include "global.h"
#include "utils.h"
#include "colors.h"
#include "battle.h"
#include "shop.h"
#include "center.h"
#include "player.h"
#include "supemon.h"

Player start_game(void);
int chose_action(Player* player);

int main(void)
{
    // SET SEED
    srand(time(NULL));
    enable_colors();

    clear_terminal();

    // Create saves directory if it doesn't exist
    #ifdef _WIN32
        system("if not exist bin\\saves mkdir bin\\saves");
    #else
        mkdir("bin", 0755);
        mkdir(SAVE_DIR, 0755);
    #endif

    Player player = start_game();

    while(1)
    {
        clear_terminal();
        if (chose_action(&player) == 0) break;
    }

    return 0;
}

Player start_game(void)
{
    Player player;

    printf("+------------------------------+\n");
    printf("|     Welcome to Supemon !     |\n");
    printf("+------------------------------+\n");
    printf("| 1. New Game                  |\n");
    printf("| 2. Load Game                 |\n");
    printf("+------------------------------+\n");

    int launch_choice = get_input_counted(1, 2);

    if (launch_choice == 2)
    {
        if (load_game(&player) == 1)
        {
            print_colored(COLOR_SUCCESS, "Game loaded successfully!\n");
            printf("%s", COLOR_NPC);
            npc_dialog("Welcome back, ", 20);
            npc_dialog(player.name, 20);
            npc_dialog("!\n\n", 20);
            printf("%s", RESET);
            npc_dialog("...", 500);
            printf("\n");
            return player;
        }
        else
        {
            print_colored(COLOR_WARNING, "No save file found or error loading. Starting new game...\n\n");
            npc_dialog("...", 500);
            // Continue vers New Game
        }
    }

    // NEW GAME
    clear_terminal();

    char name[MAX_NAME_LENGTH];

    printf("%s", COLOR_NPC);
    npc_dialog("\"...", 500);
    npc_dialog(" Hello", 30);
    npc_dialog("...", 500);
    npc_dialog(" Erm", 30);
    npc_dialog("...", 500);
    npc_dialog(" What's your name again ?\" asks an old man.\n", 30);
    printf("%s", RESET);

    npc_dialog("\nYOUR NAME: ", 10);

    int c;
    while ((c = getchar()) != '\n' && c != EOF); // empty buffer
    if (fgets(name, sizeof(name), stdin) != NULL)
    {
        size_t len = strcspn(name, "\n"); // remove '\n'
        if (name[len] == '\n') name[len] = '\0'; // expected input
        else // buffer overflow, empty buffer
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }

    printf("\n");

    printf("%s", COLOR_NPC);
    npc_dialog("\"Oh yes, I remember now. You are ", 30);
    npc_dialog(name, 30);
    npc_dialog("...", 500);
    npc_dialog("\nAnd, erm, what's my name again ?", 30);
    npc_dialog("..", 500);
    npc_dialog("\nAh yes ! I'm professor Ionis", 20);
    npc_dialog("...", 500);
    npc_dialog(" Here, I have a gift for you.\nTake one of my Supeballs ! Each contains a Supemon. Choose well !\" says the very old man.\n", 20);
    printf("%s", RESET);

    npc_dialog("\nYou are presented with a strange box.\nIt appears to contain 3 strange balls.\n\n", 10);

    printf("+------------------------------+\n");
    printf("| Choose your starter Supemon: |\n");
    printf("|   1. %sSupmander%s               |\n", RED, RESET);
    printf("|   2. %sSupasaur%s                |\n", GREEN, RESET);
    printf("|   3. %sSupirtle%s                |\n", CYAN, RESET);
    printf("+------------------------------+\n");

    int starter_choice = get_input_counted(1, 3);

    Supemon starter;

    switch (starter_choice)
    {
        case 1:
            starter = init_supemon(&SUPMANDER, 1);
            break;
        case 2:
            starter = init_supemon(&SUPASAUR, 1);
            break;
        case 3:
            starter = init_supemon(&SUPIRTLE, 1);
            break;
    }

    init_player(&player, name, starter);

    printf("%s", COLOR_NPC);
    npc_dialog("\"You chose ", 20);
    printf("%s", type_to_color(starter.type));
    npc_dialog(starter.name, 20);
    printf("%s", COLOR_NPC);
    npc_dialog(" ! A wise choice. Right on time for my hourly nap. See you soon, ", 20);
    npc_dialog(player.name, 20);
    npc_dialog(" !\" says professor Ionis as he urges you to leave his home.\n\n", 20);
    printf("%s", RESET);

    printf("%s", COLOR_SUCCESS);
    npc_dialog("You step outside the house, ready to start your adventure!\n\n", 20);
    printf("%s", RESET);
    
    npc_dialog("...", 500);

    return player;
}

int chose_action(Player* player)
{
    printf("+------------------------------+\n");
    printf("| Where do you want to go ?    |\n");
    printf("|   1. Into the wild           |\n");
    printf("|   2. Supemart                |\n");
    printf("|   3. Supemon Center          |\n");
    printf("|   4. Leave the Game          |\n");
    printf("+------------------------------+\n");

    int action_choice = get_input_counted(1, 4);

    switch (action_choice)
    {
        case 1:
            go_to_battle(player);
            return 1;
        case 2:
            go_to_shop(player);
            return 1;
        case 3:
            go_to_center(player);
            return 1;
        case 4:
            {
                printf("+------------------------------+\n");
                printf("| Do you want to save ?        |\n");
                printf("|   1. Save and quit           |\n");
                printf("|   2. Quit without saving     |\n");
                printf("|   3. Cancel                  |\n");
                printf("+------------------------------+\n");

                int save_choice = get_input_counted(1, 3);

                if (save_choice == 1)
                {
                    save_game(player);
                    return 0;
                }
                else if (save_choice == 2) return 0;
                else return 1; // Continue playing
            }
        default: break;
    }

    return 0; // Something went wrong, leave game
}