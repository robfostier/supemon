#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h> // pour usleep()
#endif

#include "utils.h"
#include "colors.h"

// Active le support des couleurs ANSI dans le terminal
void enable_colors(void)
{
    #ifdef _WIN32
        // Active le mode ANSI sur Windows 10+
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode))
            {
                dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
                SetConsoleMode(hOut, dwMode);
            }
        }
    #endif
    // Sur Linux/MacOS, les couleurs ANSI fonctionnent par défaut
}

void npc_dialog(char *message, int time_in_ms)
{
    int char_count = 0;

    while (message[char_count] != '\0')
    {
        printf("%c", message[char_count]);
        fflush(stdout);  // Forcer l'affichage immédiat

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

// Affiche du texte avec une couleur spécifique
void print_colored(const char* color, const char* text)
{
    printf("%s%s%s", color, text, RESET);
}

// Affiche une ligne de boîte avec une couleur
void print_box_colored(const char* color, const char* text)
{
    printf("%s%s%s\n", color, text, RESET);
}

// Affiche une barre de HP visuelle avec des couleurs
void print_hp_bar(int current_hp, int max_hp)
{
    // Calcule le pourcentage de HP
    float hp_percent = (float)current_hp / (float)max_hp;

    // Choisit la couleur en fonction du pourcentage
    const char* hp_color;
    if (hp_percent > 0.5)
        hp_color = COLOR_HP;           // Vert si > 50%
    else if (hp_percent > 0.2)
        hp_color = COLOR_HP_LOW;       // Jaune si entre 20% et 50%
    else
        hp_color = COLOR_HP_CRITICAL;  // Rouge si < 20%

    // Affiche la barre HP avec format: HP: [████████--] 8/10
    int bar_length = 10;
    int filled = (int)(hp_percent * bar_length);

    printf("HP: %s[", hp_color);
    for (int i = 0; i < bar_length; i++)
    {
        if (i < filled)
            printf("█");
        else
            printf("-");
    }
    printf("] %d/%d%s", current_hp, max_hp, RESET);
}