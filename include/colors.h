#pragma once

#define RESET   "\033[0m"

#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#define FIRE  "RED"
#define GRASS "GREEN"
#define WATER "CYAN"

#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

#define BOLD      "\033[1m"
#define DIM       "\033[2m"
#define ITALIC    "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK     "\033[5m"

#define COLOR_HP          GREEN
#define COLOR_HP_LOW      YELLOW
#define COLOR_HP_CRITICAL RED
#define COLOR_DAMAGE      RED
#define COLOR_STAT_UP     BRIGHT_GREEN
#define COLOR_STAT_DOWN   BRIGHT_RED
#define COLOR_MONEY       YELLOW
#define COLOR_XP          BRIGHT_MAGENTA
#define COLOR_ITEM        BRIGHT_BLUE
#define COLOR_SUCCESS     BRIGHT_GREEN BOLD
#define COLOR_ERROR       BRIGHT_RED BOLD
#define COLOR_WARNING     YELLOW
#define COLOR_NPC         BRIGHT_YELLOW