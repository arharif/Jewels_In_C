#include "game_solver.h"
#include "game_play.h"
#include "game_save.h"

/*************************************/
/*+ Hash table (already programmed) +*/
/*************************************/

hash_table *create_hash_table(int element_size) {
    // Allocating memory for the hash table structure.
    hash_table *ht;
    CREATE_MEMORY(ht, 1);

    // Initializing the hash table.
    ht->hash_capacity = 128; // Initial capacity is set to 128.
    ht->hash_size     = 0;   // Initially, the hash table is empty.
    CREATE_MEMORY(ht->hash_table, ht->hash_capacity);

    // Setting all slots to ULONG_MAX (indicating empty slots).
    for (ulong i = 0; i < ht->hash_capacity; i++) {
        ht->hash_table[i] = ULONG_MAX;
    }

    // Setting the element size.
    ht->element_size = element_size;
    return ht;
}

void delete_hash_table(hash_table *ht) {
    if (ht == NULL) {
        return;
    }
    free(ht->hash_table);
    free(ht);
}

// Hash function using a simple polynomial rolling hash. Hashes an array of
// integers.
static ulong
hash_function(int *element,       //!< The element to hash.
              int size,           //!< Size of the element (number of integers).
              ulong hash_capacity //!< Capacity of the hash table.
) {
    ulong hash = 0;
    ulong a    = 0x9e3779b9; // fractional bits of the golden ratio
    for (int j = 0; j < size; j++) {
        hash = (hash * ((ulong)size + 1) + ((ulong)element[j]) * a) %
               hash_capacity;
    }
    return hash;
}

// Rehashing function to resize the hash table when load factor exceeds 0.5
static void hash_table_rehash(hash_table *ht, int *elements) {
    // Saving the pointer to the old table and its size.
    ulong *old_table = ht->hash_table;
    ulong old_size   = ht->hash_capacity;

    // Doubling the size of the hash table.
    ht->hash_capacity *= 2;
    CREATE_MEMORY(ht->hash_table, ht->hash_capacity);
    for (ulong i = 0; i < ht->hash_capacity; i++) {
        ht->hash_table[i] = ULONG_MAX;
    }

    // Rehashing the elements from the old table to the new one.
    for (ulong i = 0; i < old_size; i++) {
        if (old_table[i] == ULONG_MAX) {
            continue; // Skip empty slots
        }
        // Computing the index correspinding to the element with the hash
        // function.
        ulong index =
            hash_function(elements + old_table[i] * ((ulong)ht->element_size),
                          ht->element_size, ht->hash_capacity);

        // linear probing to find the next available slot
        ulong j = 0;
        while (true) {
            ulong new_index = (index + j) % ht->hash_capacity; // Linear probing
            if (ht->hash_table[new_index] == ULONG_MAX) {
                ht->hash_table[new_index] = old_table[i];
                break; // Successfully inserted
            } else {
                j++;
            }
        }
    }
    free(old_table);
}

// Compares two array of integers to check if they are equal
static bool hash_array_compare(int *a, int *b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool hash_table_insert(hash_table *ht, int *elements, ulong i) {
    // Rehashing if the load factor exceeds 0.5
    if (ht->hash_size >= (ht->hash_capacity >> 1)) {
        hash_table_rehash(ht, elements);
    }

    // Computing the index where the element should be.
    ulong index = hash_function(elements + i * ((ulong)ht->element_size),
                                ht->element_size, ht->hash_capacity);

    // Linear probing to find either the element or a free spot
    ulong s     = 0;
    while (true) {
        ulong s_index = (index + s) % ht->hash_capacity; // Linear probing
        if (ht->hash_table[s_index] == ULONG_MAX) {
            // If we find an empty spot, we insert the value and return true to
            // indicate that the insertion was performed.
            ht->hash_table[s_index] = i;
            ht->hash_size++;
            return true;
        } else if (hash_array_compare(elements + i * ((ulong)ht->element_size),
                                      elements + ht->hash_table[s_index] *
                                                     ((ulong)ht->element_size),
                                      ht->element_size)) {
            // If the value is already present, we do not insert it and return
            // false to indicate that the insertion failed.
            return false;
        } else {
            // If the slot is occupied by a different value, we try the next
            // slot.
            s++;
        }
    }
    return false;
}

/**********************/
/*+ Solver functions +*/
/**********************/

solver_move *solve_puzzle(game *, solver, int, solver_result_type *, int *,
                          int *) {
    return NULL;
}

bool game_implement_solver_move(game *, solver_move *) {
    return false;
}
