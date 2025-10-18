#pragma once

#include "player.h"

Player start_game(void);

/**
 * Chose an action.
 * Returns 0 if player leaves the game, 1 otherwise.
 */
int chose_action(Player* player);