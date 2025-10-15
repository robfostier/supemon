#pragma once

#include "global.h"

typedef enum {
    MOVE_OFFENSIVE,
    MOVE_ATTACK,
    MOVE_DEFENSE,
    MOVE_EVASION,
    MOVE_ACCURACY
} MoveType;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int damage; // positive for damage/buffs, negative for healing/debuffs
    MoveType type;
} Move;

extern Move SCRATCH;
extern Move GRAWL;
extern Move POUND;
extern Move FOLIAGE;
extern Move SHELL;