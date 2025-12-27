#ifndef GAME_SOLVER_H_
#define GAME_SOLVER_H_

/*  ____        _                 */
/* / ___|  ___ | |_   _____ _ __  */
/* \___ \ / _ \| \ \ / / _ \ '__| */
/*  ___) | (_) | |\ V /  __/ |    */
/* |____/ \___/|_| \_/ \___|_|    */

#include "game.h"

/*************************************/
/*+ Table de hachage (déjà écrite)  +*/
/*************************************/

typedef struct {
    ulong hash_capacity;
    ulong hash_size;
    ulong *hash_table;
    int element_size;
} hash_table;

hash_table *create_hash_table(int element_size);

void delete_hash_table(hash_table *ht);

bool hash_table_insert(hash_table *ht, int *elements, ulong i);

/**********************/
/*+ Fonctions solveur +*/
/**********************/

typedef enum {
    SOLVER_SWAP,
    SOLVER_GRAVITY,
    SOLVER_CYCLE,
    SOLVER_STORAGE
} solver_move_type;

typedef struct solver_move {
    solver_move_type type;
    int index;
    direction dir;
    struct solver_move *next;
} solver_move;

typedef enum {
    SOLVER_TIMEOUT,
    SOLVER_UNSOLVABLE,
    SOLVER_SOLVED,
} solver_result_type;

typedef enum { SOLVER_DFS, SOLVER_BFS, SOLVER_ASTAR } solver;

solver_move *solve_puzzle(game *g, solver method, int bound,
                          solver_result_type *result, int *nb_moves,
                          int *nb_configs);

bool game_implement_solver_move(game *g, solver_move *move);

#endif // GAME_SOLVER_H_
