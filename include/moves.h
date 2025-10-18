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

extern const Move SCRATCH;
extern const Move GRAWL;
extern const Move POUND;
extern const Move FOLIAGE;
extern const Move SHELL;