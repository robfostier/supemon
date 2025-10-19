#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "global.h"
#include "utils.h"
#include "battle.h"
#include "shop.h"
#include "center.h"
#include "player.h"
#include "supemon.h"
#include "moves.h"

int main(void)
{
    // SET SEED
    srand(time(NULL));
    clear_terminal();

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
      printf("|      SUPEMON WORLD           |\n");
      printf("+------------------------------+\n");
      printf("| 1. New Game                  |\n");
      printf("| 2. Load Game                 |\n");
      printf("+------------------------------+\n");

      int choice = 0;
      while (choice < 1 || choice > 2)
      {
          printf("\nYOUR CHOICE (1 or 2): ");
          if (scanf("%d", &choice) != 1)
          {
              while (getchar() != '\n');
              choice = 0;
          }
      }

      if (choice == 2)
      {
          if (load_game(&player) == 1)
          {
              printf("\nGame loaded successfully!\n");
              npc_dialog("Welcome back, ", 20);
              npc_dialog(player.name, 20);
              npc_dialog("!\n\n", 20);
              return player;
          }
          else
          {
              printf("\nNo save file found or error loading. Starting new game...\n\n");
              // Continue vers New Game
          }
      }

      // NEW GAME
      char name[MAX_NAME_LENGTH];

      npc_dialog("\"...", 500);
      npc_dialog(" Hello", 30);
      npc_dialog("...", 500);
      npc_dialog(" Erm", 30);
      npc_dialog("...", 500);
      npc_dialog(" What's your name again ?\" asks an old man.\n", 30);

      npc_dialog("\nYOUR NAME: ", 10);
      scanf("%12s", name);
      printf("\n");

      npc_dialog("\"Oh yes, I remember now. You are ", 30);
      npc_dialog(name, 30);
      npc_dialog("...", 500);
      npc_dialog("\nAnd, erm, what's my name again ?", 30);
      npc_dialog("..", 500);
      npc_dialog("\nAh yes ! I'm professor Ionis", 20);
      npc_dialog("...", 500);
      npc_dialog(" Here, I have a gift for you.\nTake one of my Supeballs ! Each contains a Supemon. Choose well !\" says the very old man.\n", 20);

      npc_dialog("\nYou are presented with a strange box.\nIt appears to contain 3 strange balls.\n\n", 10);

      printf("+------------------------------+\n");
      printf("| Choose your starter Supemon: |\n");
      printf("|   1. Supmander               |\n");
      printf("|   2. Supasaur                |\n");
      printf("|   3. Supirtle                |\n");
      printf("+------------------------------+\n");

      choice = 0;
      while (choice < 1 || choice > 3)
      {
          printf("\nYOUR CHOICE (1, 2 or 3): ");
          if (scanf("%d", &choice) != 1)
          {
              while (getchar() != '\n');
              choice = 0;
          }
      }

      Supemon starter;

      switch (choice)
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

      npc_dialog("\n\"You chose ", 20);
      npc_dialog(starter.name, 20);
      npc_dialog(" ! A wise choice. Right on time for my hourly nap. See you soon, ", 20);
      npc_dialog(player.name, 20);
      npc_dialog(" !\" says professor Ionis as he urges you to leave his home.\n\n", 20);

      npc_dialog("You step outside the house, ready to start your adventure!\n\n", 20);

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

    int choice = 0;
    while (choice < 1 || choice > 4)
    {
        printf("\nYOUR CHOICE (1, 2, 3 or 4): ");
        scanf("%d", &choice);
    }

    printf("\n");

    switch (choice)
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
                printf("\n+------------------------------+\n");
                printf("| Do you want to save ?        |\n");
                printf("|   1. Save and quit           |\n");
                printf("|   2. Quit without saving     |\n");
                printf("|   3. Cancel                  |\n");
                printf("+------------------------------+\n");

                int save_choice = 0;
                while (save_choice < 1 || save_choice > 3)
                {
                    printf("\nYOUR CHOICE (1, 2 or 3): ");
                    if (scanf("%d", &save_choice) != 1)
                    {
                        while (getchar() != '\n');
                        save_choice = 0;
                    }
                }

                if (save_choice == 1)
                {
                    save_game(player);
                    return 0;
                }
                else if (save_choice == 2)
                {
                    return 0;
                }
                else
                {
                    return 1; // Continue playing
                }
            }
        default: break;
    }

    return 0; // Something went wrong, leave game
}