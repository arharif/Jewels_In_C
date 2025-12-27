#ifndef _GAME_SCORES_H_
#define _GAME_SCORES_H_

#include "alloc.h"
#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char (*names)[64];
    int **scores;
    int size;
    int player_index;
    int *permutation;
} score_table;

score_table *load_score_table(const char *filepath, int *player_score);

void save_score_table(score_table *table, const char *filepath,
                      const char *playername);

void delete_score_table(score_table *table);

void sort_score_table(score_table *table, score_type type);

#endif // _GAME_SCORES_H_
