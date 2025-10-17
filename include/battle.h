#include "player.h"
#include "supemon.h"

int go_to_battle(Player* player);
void display_battle(Player* player, Supemon* foe);
void battle_rewards(Player* player, Supemon* foe);

/**
 * Play player turn in battle.
 * Returns 1 if battle needs to end, 2 if player used item, 0 otherwise.
 */
int player_turn(Player* player, Supemon* foe, int item_count);

/**
 * Play foe turn in battle.
 * Returns 1 if battle needs to end, 0 otherwise.
 */
int foe_turn(Player* player, Supemon* foe);

void apply_move(Move* move, Supemon* attacker, Supemon* target);

/**
 * Reset all statistics
 */
void leave_battle(Player* player);