# Pokedex

A command-line Pokedex application for tracking and viewing Pokémon from the Kanto region.

## Features

- **Search Pokémon** by Dex number or name
- **Track Progress** - mark Pokémon as encountered or caught
- **View Statistics** - see completion percentage
- **View Progress** - list all caught and encountered Pokémon
- **Persistent Storage** - saves your progress to disk

## Building

```bash
gcc -o pokedex main.c pokedex.c -Wall -O2
```

## Usage

```bash
./pokedex
```

Navigate the menu to search for Pokémon, track encounters/catches, and view your progress.

## Data

Contains all 151 Pokémon from Generation 1 with base stats (HP, Attack, Defense, Speed) and type information.
