#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pokedex.h"

// Load Pokemon data from CSV file
int load_pokemon_data(const char *filename, Pokemon *pokedex, int max_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening pokemon.csv");
        return -1;
    }

    char line[256];
    // Skip header line
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return -1;
    }

    int count = 0;
    while (fgets(line, sizeof(line), file) && count < max_count) {
        Pokemon *p = &pokedex[count];
        
        // Manual CSV parsing to handle empty fields
        char *ptr = line;
        char *field_start;
        int field_num = 0;
        
        while (*ptr && field_num < 8) {
            field_start = ptr;
            
            // Find end of field (comma or newline)
            while (*ptr && *ptr != ',' && *ptr != '\n' && *ptr != '\r') {
                ptr++;
            }
            
            // Temporarily null-terminate the field
            char delimiter = *ptr;
            *ptr = '\0';
            
            // Parse the field based on position
            switch (field_num) {
                case 0: // id
                    p->id = (uint16_t)atoi(field_start);
                    break;
                case 1: // name
                    strncpy(p->name, field_start, MAX_NAME_LENGTH - 1);
                    p->name[MAX_NAME_LENGTH - 1] = '\0';
                    break;
                case 2: // type1
                    strncpy(p->type1, field_start, MAX_TYPE_LENGTH - 1);
                    p->type1[MAX_TYPE_LENGTH - 1] = '\0';
                    break;
                case 3: // type2 (may be empty)
                    if (*field_start != '\0') {
                        strncpy(p->type2, field_start, MAX_TYPE_LENGTH - 1);
                        p->type2[MAX_TYPE_LENGTH - 1] = '\0';
                    } else {
                        p->type2[0] = '\0';
                    }
                    break;
                case 4: // hp
                    p->hp = (uint8_t)atoi(field_start);
                    break;
                case 5: // attack
                    p->attack = (uint8_t)atoi(field_start);
                    break;
                case 6: // defense
                    p->defense = (uint8_t)atoi(field_start);
                    break;
                case 7: // speed
                    p->speed = (uint8_t)atoi(field_start);
                    break;
            }
            
            // Move to next field
            if (delimiter == ',' || delimiter == '\n' || delimiter == '\r') {
                ptr++;
                field_num++;
            } else {
                break; // End of line
            }
        }
        
        // Only increment count if we successfully parsed all 8 fields
        if (field_num >= 8) {
            count++;
        }
    }

    fclose(file);
    return count;
}

// Load user state from binary file
int load_user_state(UserState *state) {
    FILE *file = fopen(STATE_FILE, "rb");
    if (!file) {
        // First run - initialize empty state
        memset(state, 0, sizeof(UserState));
        return 0;
    }

    size_t read = fread(state, sizeof(UserState), 1, file);
    fclose(file);
    
    if (read != 1) {
        memset(state, 0, sizeof(UserState));
        return -1;
    }
    
    return 0;
}

// Save user state to binary file
int save_user_state(const UserState *state) {
    FILE *file = fopen(STATE_FILE, "wb");
    if (!file) {
        perror("Error saving state");
        return -1;
    }

    size_t written = fwrite(state, sizeof(UserState), 1, file);
    fclose(file);
    
    return (written == 1) ? 0 : -1;
}

// Search for Pokemon by ID
Pokemon* search_by_id(Pokemon *pokedex, int count, int id) {
    if (id < 1 || id > count) {
        return NULL;
    }
    return &pokedex[id - 1];
}

// Search for Pokemon by name (case-insensitive)
Pokemon* search_by_name(Pokemon *pokedex, int count, const char *name) {
    char search_name[MAX_NAME_LENGTH];
    strncpy(search_name, name, MAX_NAME_LENGTH - 1);
    search_name[MAX_NAME_LENGTH - 1] = '\0';
    
    // Convert to lowercase for comparison
    for (int i = 0; search_name[i]; i++) {
        search_name[i] = tolower(search_name[i]);
    }
    
    for (int i = 0; i < count; i++) {
        char pokemon_name[MAX_NAME_LENGTH];
        strncpy(pokemon_name, pokedex[i].name, MAX_NAME_LENGTH - 1);
        pokemon_name[MAX_NAME_LENGTH - 1] = '\0';
        
        for (int j = 0; pokemon_name[j]; j++) {
            pokemon_name[j] = tolower(pokemon_name[j]);
        }
        
        if (strcmp(search_name, pokemon_name) == 0) {
            return &pokedex[i];
        }
    }
    
    return NULL;
}

// Display Pokemon information
void display_pokemon(const Pokemon *p, const UserState *state) {
    if (!p) return;
    
    printf("\n===========================================\n");
    printf("  #%03d - %s\n", p->id, p->name);
    printf("===========================================\n");
    
    if (p->type2[0] != '\0') {
        printf("  Type: %s / %s\n", p->type1, p->type2);
    } else {
        printf("  Type: %s\n", p->type1);
    }
    
    printf("\n  Base Stats:\n");
    printf("    HP:      %3d\n", p->hp);
    printf("    Attack:  %3d\n", p->attack);
    printf("    Defense: %3d\n", p->defense);
    printf("    Speed:   %3d\n", p->speed);
    
    printf("\n  Status:\n");
    printf("    Encountered: %s\n", is_encountered(state, p->id) ? "Yes" : "No");
    printf("    Caught:      %s\n", is_caught(state, p->id) ? "Yes" : "No");
    printf("===========================================\n\n");
}

void list_progress(const Pokemon *pokedex, int count, const UserState *state) {
    int encountered_count = 0;
    int caught_count = 0;

    for (int i = 0; i < count; i++) {
        if (state->encountered[i]) {
            encountered_count++;
        }
        if (state->caught[i]) {
            caught_count++;
        }
    }

    int encountered_only = encountered_count - caught_count;

    printf("\n===========================================\n");
    printf("  POKEDEX PROGRESS\n");
    printf("===========================================\n");

    printf("  Caught (%d):\n", caught_count);
    if (caught_count == 0) {
        printf("    None yet.\n");
    } else {
        for (int i = 0; i < count; i++) {
            if (state->caught[i]) {
                printf("    #%03d %s\n", pokedex[i].id, pokedex[i].name);
            }
        }
    }

    printf("\n  Encountered (not caught) (%d):\n", encountered_only);
    if (encountered_only == 0) {
        printf("    None yet.\n");
    } else {
        for (int i = 0; i < count; i++) {
            if (state->encountered[i] && !state->caught[i]) {
                printf("    #%03d %s\n", pokedex[i].id, pokedex[i].name);
            }
        }
    }

    printf("===========================================\n\n");
}

// Mark Pokemon as encountered
void mark_encountered(UserState *state, int id) {
    if (id >= 1 && id <= MAX_POKEMON) {
        state->encountered[id - 1] = 1;
    }
}

// Mark Pokemon as caught (also marks as encountered)
void mark_caught(UserState *state, int id) {
    if (id >= 1 && id <= MAX_POKEMON) {
        state->encountered[id - 1] = 1;
        state->caught[id - 1] = 1;
    }
}

// Check if Pokemon is encountered
int is_encountered(const UserState *state, int id) {
    if (id >= 1 && id <= MAX_POKEMON) {
        return state->encountered[id - 1];
    }
    return 0;
}

// Check if Pokemon is caught
int is_caught(const UserState *state, int id) {
    if (id >= 1 && id <= MAX_POKEMON) {
        return state->caught[id - 1];
    }
    return 0;
}
