#include <stdio.h>

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
    printf("\e[1;1H\e[2J");
}