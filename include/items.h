#pragma once

#include "global.h"

typedef enum {
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_RARE_CANDY
} ItemType;

typedef struct {
    char name[MAX_NAME_LENGTH];
    unsigned int price;
    ItemType type;
} Item;

extern const Item POTION;
extern const Item SUPER_POTION;
extern const Item RARE_CANDY;
