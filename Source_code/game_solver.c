#include "game_solver.h"
#include "game_play.h"
#include "game_save.h"

/*************************************/
/*+ Hash table (already programmed) +*/
/*************************************/

hash_table *create_hash_table(int element_size)
{
    if (element_size <= 0) {
        fprintf(stderr, "Invalid element_size in create_hash_table.\n");
        exit(EXIT_FAILURE);
    }

    hash_table *ht;
    CREATE_MEMORY(ht, 1);

    ht->hash_capacity = 128;
    ht->hash_size     = 0;

    CREATE_MEMORY(ht->hash_table, ht->hash_capacity);

    for (ulong i = 0; i < ht->hash_capacity; i++) {
        ht->hash_table[i] = ULONG_MAX;
    }

    ht->element_size = element_size;
    return ht;
}

void delete_hash_table(hash_table *ht)
{
    if (ht == NULL) return;
    free(ht->hash_table);
    free(ht);
}

/* Hash function using a simple polynomial rolling hash. Hashes an array of integers. */
static ulong
hash_function(int *data,          //!< The element to hash.
              int size,           //!< Size of the element (number of integers).
              ulong hash_capacity //!< Capacity of the hash table.
)
{
    ulong hash = 0;
    ulong a    = 0x9e3779b9UL; // fractional bits of the golden ratio

    for (int j = 0; j < size; j++) {
        hash = (hash * ((ulong)size + 1UL) + ((ulong)data[j]) * a) % hash_capacity;
    }
    return hash;
}

/* Rehashing function to resize the hash table when load factor exceeds 0.5 */
static void hash_table_rehash(hash_table *ht, int *elements)
{
    ulong *old_table = ht->hash_table;
    ulong old_size   = ht->hash_capacity;

    ht->hash_capacity *= 2;
    CREATE_MEMORY(ht->hash_table, ht->hash_capacity);

    for (ulong i = 0; i < ht->hash_capacity; i++) {
        ht->hash_table[i] = ULONG_MAX;
    }

    for (ulong i = 0; i < old_size; i++) {
        if (old_table[i] == ULONG_MAX) continue;

        ulong index = hash_function(elements + old_table[i] * ((ulong)ht->element_size),
                                    ht->element_size, ht->hash_capacity);

        ulong j = 0;
        while (true) {
            ulong new_index = (index + j) % ht->hash_capacity;
            if (ht->hash_table[new_index] == ULONG_MAX) {
                ht->hash_table[new_index] = old_table[i];
                break;
            }
            j++;
        }
    }

    free(old_table);
}

/* Compares two arrays of integers */
static bool hash_array_compare(int *a, int *b, int size)
{
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool hash_table_insert(hash_table *ht, int *elements, ulong i)
{
    if (ht == NULL || elements == NULL) return false;

    if (ht->hash_size >= (ht->hash_capacity >> 1)) {
        hash_table_rehash(ht, elements);
    }

    ulong index = hash_function(elements + i * ((ulong)ht->element_size),
                                ht->element_size, ht->hash_capacity);

    ulong s = 0;
    while (true) {
        ulong s_index = (index + s) % ht->hash_capacity;

        if (ht->hash_table[s_index] == ULONG_MAX) {
            ht->hash_table[s_index] = i;
            ht->hash_size++;
            return true;
        }

        if (hash_array_compare(elements + i * ((ulong)ht->element_size),
                               elements + ht->hash_table[s_index] * ((ulong)ht->element_size),
                               ht->element_size)) {
            return false; // already present
        }

        s++;
    }

    /* Unreachable, but keeps some compilers happy */
    /* return false; */
}

/**********************/
/*+ Solver functions +*/
/**********************/

solver_move *solve_puzzle(game *g,
                          solver method,
                          int bound,
                          solver_result_type *result,
                          int *nb_moves,
                          int *nb_configs)
{
    (void)g;
    (void)method;
    (void)bound;

    /* Safe defaults (match enum in game_solver.h) */
    if (result)     *result = SOLVER_UNSOLVABLE;
    if (nb_moves)   *nb_moves = 0;
    if (nb_configs) *nb_configs = 0;

    /* TODO: implement real solver (DFS/BFS/A*) */
    return NULL;
}

bool game_implement_solver_move(game *g, solver_move *move)
{
    (void)g;
    (void)move;

    /* TODO: apply one solver move to the game state */
    return false;
}