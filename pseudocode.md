

void into_the_wild(Player* player)
{
    random_mon = select random mon

    foe = init_supemon(&random_mon, player.get_active_supemon()->level)

    item_count = 0

    while True
    {
        turn_result = battle_player_turn(player, &foe, item_count) // PLAYER TURN
        if (turn_result == 1)                                      // Player won
        {
            battle_rewards(player, &foe)
            return
        }
        elif (turn_result == 2)                                    // Player used item, gets another turn
        {
            if (battle_player_turn(player, &foe, 5) == 1)          // This time, he can't use items
            {
                battle_rewards(player, &foe)
                return
            }
        }

        if (battle_foe_turn(player, &foe) == 1) return             // FOE TURN
    }
}

void battle_rewards(Player* player, Supemon* foe)
{
    player->coins += rand(100, 500);
    gain_experience(player.get_active_supemon(), rand(100, 500) * foe->level)
}

void display_battle(Player* player, Supemon* foe)
{
    print foe
    print player->get_active_supemon
}

int battle_player_turn(Player* player, Supemon* foe, int item_count) // retourne 0 si combat continue, retourne 1 si combat fini, retourne 2 si item
{
    print(Your turn...)

    display_battle()
    
    print(What will you do ?)
    input action

    switch(action)
    {
        case move:
            c = 0
            while
            {
                if player.get_active_supemon()->moves[c] == NULL
                    print(c + 1, " - Cancel")
                    break

                print(c + 1, " - ", player.get_active_supemon()->moves[c])
                c++
            }

            input

            apply_move(player.get_active_supemon()->moves[input - 1], player.get_active_supemon(), &foe)

            if (foe->hp > 0) return 0;
            else return 1;

        case change:
            player.display_supemons() // NOT YET IMPLEMENTED

            input supemon
            player.set_active_supemon(input - 1)
            return 0

        case item:
            if (item_count < 4)
            {
                player.display_items() // NOT YET IMPLEMENTED

                input
                item = player->items[input]

                apply_item(&item, player.get_active_supemon()) // NOT YET IMPLEMENTED
                item_count++
            }
            else
            {
                print(Can't use item !)
            }

        case capture:
            rnd_val = rand(0, 1)
            if (rnd_val <= (foe->max_health - foe->health) / foe->max_health - .5f)
            {
                player.add_supemon(*foe)
                print(Captured !)
                return 1
            }

        case run:
            rnd_val = rand(0, 1)
            if (rnd_val <= player.get_active_supemon()->speed / (player.get_active_supemon()->speed + foe->speed))
            {
                print(Ran away !);
                return 1;
            }
            return 0;
    }
}

int battle_foe_turn(Player* player, Supemon* foe)
{
    get foe move count
    random value in count

    apply_move(foe->moves[rnd_val], &foe, player.get_active_supemon())

    if (player.get_active_supemon()->hp > 0) return 0;
    else return 1;
}


void apply_move(Move *move, Supemon *attacker, Supemon *target)
{
    switch(move->type)
        case 'O':
            if (move->damage > 0)
            {
                rnd_val = rand(0, 1)

                dmg = attacker->attack * move->damage / target->defense
                if (floor(dmg) != dmg)
                    if (rnd_val >= .5) dmg += rnd_val

                dodge = attacker->accuracy / (attacker->accuracy + target->evasion) + 0.1

                if (rnd_val > dodge) update_health(target, dmg)  // NOT YET IMPLEMENTED
                else print("dodged !")
            }
            else
            {
                update_health(attacker, -move->damage)  // NOT YET IMPLEMENTED
            }
            
        case 'A':
            if (move->damage > 0)
            {
                attacker->attack += move->damage;
            }
            else
            {
                target->attack += move->damage;
            }

        case 'D':
        if (move->damage > 0)
            {
                attacker->defense += move->damage;
            }
            else
            {
                target->defense += move->damage;
            }

        case 'E':
        if (move->damage > 0)
            {
                attacker->evasion += move->damage;
            }
            else
            {
                target->evasion += move->damage;
            }

        case 'P':
        if (move->damage > 0)
            {
                attacker->accuracy += move->damage;
            }
            else
            {
                target->accuracy += move->damage;
            }

        case 'S':
        if (move->damage > 0)
            {
                attacker->speed += move->damage;
            }
            else
            {
                target->speed += move->damage;
            }
    return;
}

update_health(Supemon* supemon, int value)