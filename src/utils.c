#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h> // pour usleep()
#endif

#include "utils.h"

void npc_dialog(char *message, int time_in_ms)
{
    int char_count = 0;

    while (message[char_count] != '\0')
    {
        printf("%c", message[char_count]);

        #ifdef _WIN32
            Sleep(time_in_ms);              // Windows -> millisecondes
        #else
            usleep(time_in_ms * 1000);      // Linux/Unix -> microsecondes
        #endif

        char_count++;
    }
}

void clear_terminal(void)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    printf("+------------------------------+\n");
    printf("|         Supemon -CLI         |\n");
    printf("+------------------------------+\n\n");
}

int get_input(int min, int max)
{
    int input = 0;
    while (input < min || input > max)
    {
        printf("\nYOUR CHOICE: ");
        if (scanf("%d", &input) != 1)
        {
            while (getchar() != '\n');
            input = 0;
        }
    }
    printf("\n");

    return input;
}

int get_input_counted(int min, int max)
{
    int input = 0;
    while (input < min || input > max)
    {
        printf("\nYOUR CHOICE (");

        for (int i = 0; i < max - 2; i++)
        {
            printf("%d, ", i + 1);
        }
        printf("%d ", max - 1);
        printf("or %d): ", max);

        if (scanf("%d", &input) != 1)
        {
            while (getchar() != '\n');
            input = 0;
        }
    }
    printf("\n");

    return input;
}