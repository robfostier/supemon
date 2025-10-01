#pragma once

typedef struct {
    char name[10];
    int damage; // positive for damage/buffs, negative for healing/debuffs
    char type; // 'O' for offensive moves (damage/healing), 'A' for attack, 'D' for defense, 'E' for evasion, 'P' for accuracy, 'S' for speed
} Move;

extern Move SCRATCH;
extern Move GRAWL;
extern Move POUND;
extern Move FOLIAGE;
extern Move SHELL;