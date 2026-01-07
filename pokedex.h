#ifndef POKEDEX_H
#define POKEDEX_H

#include <stdint.h>

#define MAX_NAME_LENGTH 20
#define MAX_TYPE_LENGTH 15
#define MAX_POKEMON 151
#define STATE_FILE "pokedex_state.dat"

// Pokemon data structure
typedef struct {
    uint16_t id;
    char name[MAX_NAME_LENGTH];
    char type1[MAX_TYPE_LENGTH];
    char type2[MAX_TYPE_LENGTH];
    uint8_t hp;
    uint8_t attack;
    uint8_t defense;
    uint8_t speed;
} Pokemon;

// User state for tracking encounters and catches
typedef struct {
    uint8_t encountered[MAX_POKEMON];  // bit array: 1 if encountered
    uint8_t caught[MAX_POKEMON];       // bit array: 1 if caught
} UserState;

// Function declarations
int load_pokemon_data(const char *filename, Pokemon *pokedex, int max_count);
int load_user_state(UserState *state);
int save_user_state(const UserState *state);
Pokemon* search_by_id(Pokemon *pokedex, int count, int id);
Pokemon* search_by_name(Pokemon *pokedex, int count, const char *name);
void display_pokemon(const Pokemon *p, const UserState *state);
void list_progress(const Pokemon *pokedex, int count, const UserState *state);
void mark_encountered(UserState *state, int id);
void mark_caught(UserState *state, int id);
int is_encountered(const UserState *state, int id);
int is_caught(const UserState *state, int id);

#endif // POKEDEX_H
