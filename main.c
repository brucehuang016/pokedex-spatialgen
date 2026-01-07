#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pokedex.h"

void print_menu(void) {
    printf("\n");
    printf("==========================================\n");
    printf("          KANTO REGION POKEDEX            \n");
    printf("==========================================\n");
    printf("\n");
    printf("  1. Search by Dex Number\n");
    printf("  2. Search by Name\n");
    printf("  3. Mark as Encountered\n");
    printf("  4. Mark as Caught\n");
    printf("  5. View Caught/Encountered\n");
    printf("  6. View Statistics\n");
    printf("  7. Exit\n");
    printf("\n");
    printf("  Enter choice: ");
}

void print_stats(const UserState *state) {
    int encountered_count = 0;
    int caught_count = 0;
    
    for (int i = 0; i < MAX_POKEMON; i++) {
        if (state->encountered[i]) encountered_count++;
        if (state->caught[i]) caught_count++;
    }
    
    printf("\n===========================================\n");
    printf("  POKEDEX STATISTICS\n");
    printf("===========================================\n");
    printf("  Total Pokemon:     151\n");
    printf("  Encountered:       %3d / 151 (%.1f%%)\n", 
           encountered_count, (encountered_count / 151.0) * 100);
    printf("  Caught:            %3d / 151 (%.1f%%)\n", 
           caught_count, (caught_count / 151.0) * 100);
    printf("===========================================\n\n");
}

int main(void) {
    Pokemon pokedex[MAX_POKEMON];
    UserState state;
    
    // Load Pokemon data
    printf("Loading Pokedex data...\n");
    int pokemon_count = load_pokemon_data("pokemon.csv", pokedex, MAX_POKEMON);
    if (pokemon_count <= 0) {
        fprintf(stderr, "Failed to load Pokemon data. Make sure pokemon.csv is in the current directory.\n");
        return 1;
    }
    printf("Loaded %d Pokemon!\n", pokemon_count);
    
    // Load user state
    if (load_user_state(&state) == 0) {
        printf("Loaded user progress.\n");
    } else {
        printf("Starting new Pokedex.\n");
    }
    
    char input[100];
    int running = 1;
    
    while (running) {
        print_menu();
        
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        int choice = atoi(input);
        
        switch (choice) {
            case 1: {
                // Search by ID
                printf("\n  Enter Dex Number (1-151): ");
                if (fgets(input, sizeof(input), stdin)) {
                    int id = atoi(input);
                    Pokemon *p = search_by_id(pokedex, pokemon_count, id);
                    if (p) {
                        display_pokemon(p, &state);
                    } else {
                        printf("\n  Pokemon #%d not found.\n", id);
                    }
                }
                break;
            }
            
            case 2: {
                // Search by name
                printf("\n  Enter Pokemon Name: ");
                if (fgets(input, sizeof(input), stdin)) {
                    // Remove newline
                    input[strcspn(input, "\n")] = 0;
                    Pokemon *p = search_by_name(pokedex, pokemon_count, input);
                    if (p) {
                        display_pokemon(p, &state);
                    } else {
                        printf("\n  Pokemon '%s' not found.\n", input);
                    }
                }
                break;
            }
            
            case 3: {
                // Mark encountered
                printf("\n  Enter Dex Number to mark as encountered: ");
                if (fgets(input, sizeof(input), stdin)) {
                    int id = atoi(input);
                    Pokemon *p = search_by_id(pokedex, pokemon_count, id);
                    if (p) {
                        mark_encountered(&state, id);
                        save_user_state(&state);
                        printf("\n  Marked %s as encountered!\n", p->name);
                    } else {
                        printf("\n  Pokemon #%d not found.\n", id);
                    }
                }
                break;
            }
            
            case 4: {
                // Mark caught
                printf("\n  Enter Dex Number to mark as caught: ");
                if (fgets(input, sizeof(input), stdin)) {
                    int id = atoi(input);
                    Pokemon *p = search_by_id(pokedex, pokemon_count, id);
                    if (p) {
                        mark_caught(&state, id);
                        save_user_state(&state);
                        printf("\n  Congratulations! You caught %s!\n", p->name);
                    } else {
                        printf("\n  Pokemon #%d not found.\n", id);
                    }
                }
                break;
            }
            
            case 5: {
                // View caught/encountered summary
                list_progress(pokedex, pokemon_count, &state);
                break;
            }
            
            case 6: {
                // View statistics
                print_stats(&state);
                break;
            }
            
            case 7: {
                // Exit
                printf("\n  Saving progress...\n");
                save_user_state(&state);
                printf("  Thank you for using the Pokedex!\n\n");
                running = 0;
                break;
            }
            
            default:
                printf("\n  Invalid choice. Please try again.\n");
                break;
        }
    }
    
    return 0;
}
