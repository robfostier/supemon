#include <stdio.h>
#include <windows.h>
#include "utils.h"

void npc_dialog(char *message, int time_in_ms)
{
    int char_count = 0;

    while (message[char_count] != '\0')
    {
        printf("%c", message[char_count]);
        Sleep(time_in_ms);
        char_count++;
    }
}