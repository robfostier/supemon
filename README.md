# Supémon - pCDEV1

*Robinson Fostier, Mouhamadou Sy*

---
## User documentation

### Getting started

#### Prerequisites

Make sure you have the following installed :
- `gcc`
- `make`

To check if you have them installed :

```bash
gcc --version
make --version
```

You will also need Linux, MacOS or Windows.

#### Build instructions

1. Clone the repository

```bash
git clone https://github.com/robfostier/supemon.git
cd supemon
```

2. Build the project

```bash
make
```

3. Run the executable
    - on Windows : `./bin/supemon.exe`
    - on Linux : `./bin/supemon`

#### Save files

Saves are stored under `bin/saves/supemon_save.txt`.

---
## Technical documentation

### Project architecture

#### Directory structure

This project is written in C. It follows a modular architecture that separates source code files, headers, compiled objects and executable into dedicated directories for maintainability.

```bash
supemon/
├── bin/
│   ├── supemon.exe
│   └── saves/
│       └── supemon_save.txt
├── src/
│   └── sources.c
├── include/
│   └── headers.h
├── obj/
│   └── objects.o
├── Makefile
└── README.md
```

- `bin/` contains compiled binaries and savefiles.
- `src/` contains source code files.
- `include/` contains header files.
- `obj/` contains compiled objects.
- `Makefile` is a custom made file that automates compilation on Linux and Windows.
- `README.md` contains User Doc and Technical Doc. It is the file you are currently looking at.

#### Makefile Targets

Three commands are defined within Makefile :
- `make` compiles the project and produces `bin/supemon.exe`
- `make run` builds and runs the program
- `make clean` cleans the build

#### Build Process

1. Compile Sources
    Each `.c` file in `src/` is compiled into an `.o` file in `obj/`, using `.h` files in `include/`
2. Link Objects
    `.o` files are linked together to produce an executable in `bin/`
3. Run
    Execute the program from `bin/supemon.exe`
4. Clean
    Use `make clean` to clean the build by removing all files in `obj/` and `bin/`

---
### Data structures

#### Player

```c
typedef struct {
    char name[MAX_NAME_LENGTH];
    Supemon supemons[MAX_SUPEMONS];
    int active_index;
    unsigned int coins;
    const Item* items[MAX_ITEMS];
} Player;
```

This structure contains all data associated with the Player:
1. `name` is a char array of size 12 + 1 for '\0'. It stores the name of the Player.
2. `supemons` is a Supemon array of size 6. This is where captured Supemons live in memory. The empty slots memory addresses are filled with 0s. Checking for empty slots is done by looking for any Supemon with level equal to 0.
3. `active_index` is an integer that allows us to keep track of the current active Supemon.
4. `coins` is a positive integer that keeps track of the Player's wallet. The Player cannot have debt.
5. `items` is an const Item pointer array of size 20. It stores one pointer per each item in the Player's collection. They point to one of the three const Item defined in `items.c`.

#### Supemon

```c
    typedef struct {
        char name[MAX_NAME_LENGTH];
        unsigned int level;
        unsigned int experience;
        int health;
        int max_health;
        int attack;
        int base_attack;
        // More stats
        int speed;
        const Move* moves[MAX_MOVES];
    } Supemon;
```

This structure contains all data associated with Supemons:
1. `name` is a char array of size 12 + 1 for '\0'. It stores the name of the Supemon.
2. `level` and `experience` are positive integers for keeping track of the Supemon current level.
3. 11 integers are used to store the Supemon statistics. `attack`, `defense`, `accuracy` and `evasion` are always equal to 0 outside battles. `health` cannot be greater than `max_health`.
4. `moves` is a const Move pointer array of size 4. It stores one pointer per move known by the Supemon. If the Supemon doesn't know 4 moves, the remaining slots store `NULL`.

#### Move

```c
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
```

This structure contains all data associated with Moves:
1. `name` is a char array of size 12 + 1 for '\0'. It stores the name of the Move.
2. `damage` is an integer that stores the damage/heal/buff/debuff value associated with the Move.
3. `type` is a MoveType that specifies the effect of the Move. 
    - `MOVE_OFFENSIVE` represents Moves that deal damage to the target - when positive -, or heal the launcher - when negative -.
    - `MOVE_ATTACK`, `MOVE_DEFENSE`, `MOVE_EVASION` and `MOVE_ACCURACY` represent Moves that buff the launcher - when positive -, or debuff the target - when negative -.

#### Item

```c
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
```

This structure contains all data associated with Items:
1. `name` is a char array of size 12 + 1 for '\0'. It stores the name of the Item.
2. `price` is a positive integer that stores the buying price of the Item in the shop.
3. `type` is an ItemType that specifies the effect of the Item. 

Three ItemTypes are implemented in the game : `ITEM_POTION`, `ITEM_SUPER_POTION` and `ITEM_RARE_CANDY`. 

---
### Constants

#### Supemons

Three Supemons are defined as constants in this game :
- `SUPMANDER`
- `SUPASAUR`
- `SUPIRTLE`

When initializing a random Supemon in battle, these level 1 templates are referenced and the program makes a deepcopy of these Supemons in the stack.
When adding a Supemon to the Player's team, these deepcopies are stored in heap.

#### Moves

Five Moves are defined as constants in this game :
- `SCRATCH`
- `GROWL`
- `POUND`
- `FOLIAGE`
- `SHELL`

Every Move pointer stored by Supemons points to the address of one of these reference. That way, we only define the Moves once and they can never be tempered with.

#### Items

Three Items are defined as constants in this game :
- `POTION`
- `SUPER_POTION`
- `RARE_CANDY`

Every Item pointer stored by Player points to the address of one of these reference. That way, we only define the Items once and they can never be tempered with.

#### Global constants

```c
#define MAX_NAME_LENGTH 13 // 12 + '\0'
#define MAX_SUPEMONS    6
#define MAX_ITEMS       20
#define MAX_MOVES       4
#define SAVE_FILE       "bin/saves/supemon_save.txt"
#define SAVE_DIR        "bin/saves"
#define BOX_WIDTH       30 // number of writable chars in print box
```

These macros are defined in `global.h` and used throughout the code to maintain coherence. They are self explanatory.

#### Static constants

```c
#define BATTLE_ITEM_LIMIT 4
```

This macro is defined in `battle.c` to ensure that the Player doesn't use more than 4 Items in a single battle.

---
### Program overview

#### Entry point

```c
int main(void)
{
    Player player = start_game();

    while(1)
    {
        if (chose_action(&player) == 0) break;
    }
}
```

`main()` is defined in main\.c. It acts as the entry point to our program. A Player object is created with `start_game()`, and the program then loops over `chose_action()` until the user asks to exit the game.

#### Creating a Player

```c
Player start_game(void)
{
    Player player;

    // Asks the user to chose between loading a save and starting a new game
    
    if (launch_choice == 2)
    {
        load_game(&player);
        return player;
    }
    
    // Asks the user to chose a name and a starter Supemon

    init_player(&player, name, starter);
    return player;
}
```

The role of `start_game()` is to create a Player object, either through loading saved data or creating a new game.
When creating a new game, a Player is initialized using `init_player()`.

A Player is composed of Supemons, themselves composed of Moves pointers, and Items pointers.
Those four structures and all functional logic associated with them are located into specific files : player\.c, supemon\.c, moves\.c and items\.c, and their respective header files.
The game only needs a properly initialized Player object and the defined constants to run.

#### Game loop

```c
int chose_action(Player* player)
{
    // Asks for 1 of 4 actions : start a random encounter, go to the Supemon Center, go to the Shop, or exit the game

    switch (action_choice)
    {
        case 1:
            go_to_battle(player);
            return 1;
        case 2:
            go_to_shop(player);
            return 1;
        case 3:
            go_to_center(player);
            return 1;
        case 4:
        {
            // Asks if the user wants to save and quit, quit without saving or keep playing

            if (save_choice == 1)
            {
                save_game(player);
                return 0;
            }
            else if (save_choice == 2) return 0;
            else return 1;
        }
    }
}
```

This functions asks as the main loop for our game. `main()` will keep calling it until it returns 0, at which point the user has asked to exit the game.
Three main areas are implemented for the Player to explore :
- `go_to_battle()` will start an encounter against a randomly selected Supemon.
- `go_to_shop()` will send the Player to the Supemart, where they can buy and sell items.
- `go_to_center()` will send the Player to the Supemon Center, where they can take a look at their Supemons and heal them and/or decide to switch their active Supemon.

Each of those areas functionalities are defined in functions located in area-specific files : battle\.c, shop\.c and center\.c, and their respective header files.

#### I/O utilities

```c
void npc_dialog(char *message, int time_in_ms); // Prints a line of dialog, char by char
void clear_terminal(void);                      // Clears the terminal for a clean look at the game
int get_input_counted(int min, int max);        // Asks the player for an integer, and ensures the validity of that input
```

I/O management is done using functions located in utils\.c. These functions help us maintain graphical coherence and ensure input safety. They are used throughout the code.

#### Saving / Loading