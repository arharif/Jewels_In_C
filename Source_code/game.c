#include "game.h"
#include "alloc.h"
#include "gui_gamevars.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =======================================================================================
 * RNG seed (once)
 * ======================================================================================= */
static void seed_rng_once(void)
{
    static bool seeded = false;
    if (!seeded) {
        seeded = true;
        srand((unsigned)time(NULL));
    }
}

/*************************************************************************/
/*  Functions called by the GUI to retrieve information on a game board  */
/*************************************************************************/

int board_get_width(board *b)
{
    return b ? b->largeur : 0;
}

int board_get_height(board *b)
{
    return b ? b->hauteur : 0;
}

direction board_get_gravity(board *b)
{
    return b ? b->gravite : DOWN;
}

bool board_get_picked(board *b, int *i, int *j)
{
    if (!b || !i || !j) return false;
    *i = b->x;
    *j = b->y;
    return b->select;
}

void board_get_cell(board *b, int i, int j, cell *C)
{
    if (!b || !C) return;
    /* board uses plateau[row][col] => plateau[y][x] */
    *C = b->plateau[j][i];
}

int board_refill_size(board *b)
{
    if (!b || !b->q) return -1;
    return b->queue_size;
}

/* Read the i-th element in queue without changing the queue order. */
static void *queue_read(queue *q, int i)
{
    if (q == NULL || i < 0) return NULL;

    size_t cap = 16;
    size_t n = 0;
    void **tmp = malloc(cap * sizeof(*tmp));
    if (!tmp) return NULL;

    /* Drain queue into tmp */
    while (!isempty_queue(q)) {
        void *v = dequeue(q);

        if (n == cap) {
            cap *= 2;
            void **new_tmp = realloc(tmp, cap * sizeof(*tmp));
            if (!new_tmp) {
                free(tmp);
                return NULL;
            }
            tmp = new_tmp;
        }
        tmp[n++] = v;
    }

    /* Restore queue in same order */
    for (size_t k = 0; k < n; k++) {
        enqueue(tmp[k], q);
    }

    void *ret = (i < (int)n) ? tmp[i] : NULL;
    free(tmp);
    return ret;
}

bool board_read_refill(board *b, int k, cell *c)
{
    if (b == NULL || c == NULL) return false;
    if (b->q == NULL) return false;

    cell *tmp = (cell *)queue_read(b->q, k);
    if (tmp == NULL) return false;

    *c = *tmp;
    return true;
}

/******************************************************************************/
/*+ Functions used by the GUI to create and update a new game in classic mode */
/******************************************************************************/

/* SCC helper (your logic kept, but braces/returns made safe) */
bool bloc_verification(cell **plateau, int x, int y, direction dir,
                       int i, int j, int *count)
{
    if (!plateau || !count) return false;

    /* x,y is the reference cell (row=x, col=y) */
    if (i < 0 || j < 0) return false;
    if (i > x || (i == x && j > y)) return false;
    if (plateau[i][j].object != plateau[x][y].object) return false;

    if (*count == 0) {
        *count = 3;
        return true;
    }

    (*count)--;

    return ((dir != DOWN)  && bloc_verification(plateau, x, y, UP,    i + 1, j,     count)) ||
           ((dir != UP)    && bloc_verification(plateau, x, y, DOWN,  i - 1, j,     count)) ||
           ((dir != LEFT)  && bloc_verification(plateau, x, y, RIGHT, i,     j - 1, count)) ||
           ((dir != RIGHT) && bloc_verification(plateau, x, y, LEFT,  i,     j + 1, count));
}

/* Gravity-down compaction used by puzzle init */
static void apply_gravity_down(board *b)
{
    int height = b->hauteur;  /* rows */
    int width  = b->largeur;  /* cols */

    for (int col = 0; col < width; col++) {

        int write_row = height - 1;

        /* scan bottom -> top */
        for (int row = height - 1; row >= 0; row--) {

            if (b->plateau[row][col].object != EMPTY) {

                if (row != write_row) {
                    b->plateau[write_row][col] = b->plateau[row][col];
                }

                write_row--;
            }
        }

        /* fill remainder with EMPTY */
        for (int row = write_row; row >= 0; row--) {
            b->plateau[row][col].object = EMPTY;
            b->plateau[row][col].power  = GEM_BASIC;
            b->plateau[row][col].fall   = 0;
            b->plateau[row][col].kill   = KILL_NOKILL;
        }
    }
}

game *game_create_classic(int width, int height, bool lines, int target_score,
                          int scale_score, bool nodelay, bool upgrades,
                          bool easy_mode, bool gravity_gems, bool bombs)
{
    seed_rng_once();

    game *new = malloc(sizeof(game));
    if (!new) {
        fprintf(stderr, "Out of memory: game_create_classic(game)\n");
        exit(EXIT_FAILURE);
    }

    /* Board */
    new->b = malloc(sizeof(board));
    if (!new->b) {
        fprintf(stderr, "Out of memory: game_create_classic(board)\n");
        exit(EXIT_FAILURE);
    }

    new->b->largeur = width;   /* cols */
    new->b->hauteur = height;  /* rows */
    new->b->gravite = DOWN;

    new->b->select = false;
    new->b->x = new->b->y = 0;
    new->b->x2 = new->b->y2 = 0;

    new->b->q = NULL;
    new->b->queue_size = 0;

    new->b->plateau = malloc(height * sizeof(cell *));
    if (!new->b->plateau) {
        fprintf(stderr, "Out of memory: game_create_classic(plateau rows)\n");
        exit(EXIT_FAILURE);
    }

    for (int row = 0; row < height; row++) {
        new->b->plateau[row] = malloc(width * sizeof(cell));
        if (!new->b->plateau[row]) {
            fprintf(stderr, "Out of memory: game_create_classic(plateau row)\n");
            exit(EXIT_FAILURE);
        }

        for (int col = 0; col < width; col++) {

            /* Only basic jewels AMBER..TOPAZ (0..6) */
            new->b->plateau[row][col].object = (element)(rand() % 7);
            new->b->plateau[row][col].power = GEM_BASIC;
            new->b->plateau[row][col].fall  = 0;
            new->b->plateau[row][col].kill  = KILL_NOKILL;

            if (lines) {
                /* Prevent immediate 3-in-a-row/column */
                while ((col > 1 &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row][col - 1].object &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row][col - 2].object) ||
                       (row > 1 &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row - 1][col].object &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row - 2][col].object))
                {
                    new->b->plateau[row][col].object = (element)(rand() % 7);
                }
            } else {
                /* SCC mode: keep your constraint logic */
                int count = 3;
                while (bloc_verification(new->b->plateau, row, col, DOWN, row, col, &count)) {
                    new->b->plateau[row][col].object = (element)(rand() % 7);
                }
            }
        }
    }

    /* Params */
    new->puzzle = false;
    new->lines = lines;
    new->upgrades = upgrades;
    new->gravity_control = 0;
    new->nodelay = nodelay;
    new->easy = easy_mode;
    new->gravity_gems = gravity_gems;
    new->bombs = bombs;
    new->refill_mode = 2;

    /* Score */
    new->score = malloc(sizeof(score_block));
    if (!new->score) {
        fprintf(stderr, "Out of memory: game_create_classic(score)\n");
        exit(EXIT_FAILURE);
    }

    new->score->level = 1;
    new->score->level_score = 0;
    new->score->target_score = target_score;
    new->score->scale_score = scale_score;
    for (score_type x = 0; x < SC_SIZE; x++) new->score->scores[x] = 0;
    new->score->refills = 0;

    return new;
}

game *game_create_puzzle(int width, int height, bool lines, bool upgrades,
                         short refill_mode, int ini, bool balanced,
                         short gravity_control)
{
    seed_rng_once();

    game *new = malloc(sizeof(game));
    if (!new) {
        fprintf(stderr, "Out of memory: game_create_puzzle(game)\n");
        exit(EXIT_FAILURE);
    }

    new->b = malloc(sizeof(board));
    if (!new->b) {
        fprintf(stderr, "Out of memory: game_create_puzzle(board)\n");
        exit(EXIT_FAILURE);
    }

    new->b->largeur = width;
    new->b->hauteur = height;
    new->b->gravite = DOWN;

    new->b->select = false;
    new->b->x = new->b->y = 0;
    new->b->x2 = new->b->y2 = 0;

    new->b->q = create_queue();
    new->b->queue_size = 0;

    new->b->plateau = malloc(height * sizeof(cell *));
    if (!new->b->plateau) {
        fprintf(stderr, "Out of memory: game_create_puzzle(plateau rows)\n");
        exit(EXIT_FAILURE);
    }

    for (int row = 0; row < height; row++) {
        new->b->plateau[row] = malloc(width * sizeof(cell));
        if (!new->b->plateau[row]) {
            fprintf(stderr, "Out of memory: game_create_puzzle(plateau row)\n");
            exit(EXIT_FAILURE);
        }

        for (int col = 0; col < width; col++) {

            if (rand() % 3 == 0)
                new->b->plateau[row][col].object = EMPTY;
            else
                new->b->plateau[row][col].object = (element)(rand() % 7);

            new->b->plateau[row][col].power = GEM_BASIC;
            new->b->plateau[row][col].fall  = 0;
            new->b->plateau[row][col].kill  = KILL_NOKILL;

            if (lines) {
                while ((col > 1 &&
                        new->b->plateau[row][col].object != EMPTY &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row][col - 1].object &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row][col - 2].object) ||
                       (row > 1 &&
                        new->b->plateau[row][col].object != EMPTY &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row - 1][col].object &&
                        new->b->plateau[row][col].object ==
                        new->b->plateau[row - 2][col].object))
                {
                    new->b->plateau[row][col].object =
                        (rand() % 3 == 0) ? EMPTY : (element)(rand() % 7);
                }
            } else {
                int count = 3;
                while (bloc_verification(new->b->plateau, row, col, DOWN, row - 1, col, &count) ||
                       bloc_verification(new->b->plateau, row, col, RIGHT, row, col - 1, &count))
                {
                    new->b->plateau[row][col].object =
                        (rand() % 3 == 0) ? EMPTY : (element)(rand() % 7);
                }
            }
        }
    }

    /* Apply gravity once (clean board) */
    apply_gravity_down(new->b);

    /* Params */
    new->puzzle = true;
    new->lines = lines;
    new->upgrades = upgrades;
    new->refill_mode = refill_mode;
    new->gravity_control = gravity_control;
    new->nodelay = true;
    new->easy = false;
    new->gravity_gems = false;
    new->bombs = false;

    /* Score block */
    new->score = malloc(sizeof(score_block));
    if (!new->score) {
        fprintf(stderr, "Out of memory: game_create_puzzle(score)\n");
        exit(EXIT_FAILURE);
    }

    new->score->level = 1;
    new->score->level_score = 0;
    new->score->target_score = 0;
    new->score->scale_score = 0;
    for (score_type i = 0; i < SC_SIZE; i++) new->score->scores[i] = 0;
    new->score->refills = 0;

    /* Refill modes */
    if (refill_mode == 2) {
        /* no refill queue */
        delete_queue(new->b->q);
        new->b->q = NULL;
    } else if (refill_mode == 0) {
        /* infinite (queue exists but empty) */
        /* already created */
        new->b->queue_size = 0;
    } else if (refill_mode == 1) {
        /* finite queue with ini items */
        if (ini > width * height) ini = width * height;

        for (int k = 0; k < ini; k++) {
            cell *tmp = malloc(sizeof(cell));
            if (!tmp) {
                fprintf(stderr, "Out of memory: puzzle refill cell\n");
                exit(EXIT_FAILURE);
            }

            if (rand() % 2 == 0) {
                tmp->object = ROCK;
                tmp->power = 0;
            } else {
                tmp->object = BOMB;
                tmp->countdown = (short)(rand() % 16);
            }
            tmp->fall = 0;
            tmp->kill = KILL_NOKILL;

            enqueue(tmp, new->b->q);
            new->b->queue_size++;
        }
    }

    /* Balanced mode (kept, but safer bounds) */
    if (balanced) {
        int count[ELEM_SIZE] = {0};
        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                element e = new->b->plateau[r][c].object;
                if (e >= 0 && e < ELEM_SIZE) count[e]++;
            }
        }

        for (int t = 0; t < 7; t++) { /* only basic jewels 0..6 */
            if (count[t] % 3 == 2) {
                int done = 0;
                for (int r = height - 1; r >= 0 && !done; r--) {
                    for (int c = 0; c < width; c++) {
                        if (new->b->plateau[r][c].object == EMPTY) {
                            new->b->plateau[r][c].object = (element)t;
                            count[t]++;
                            done = 1;
                            break;
                        }
                    }
                }
            } else if (count[t] % 3 == 1) {
                int done = 0;
                for (int r = height - 1; r >= 0 && !done; r--) {
                    for (int c = 0; c < width; c++) {
                        if (new->b->plateau[r][c].object == (element)t) {
                            if (r == height - 1 || new->b->plateau[r + 1][c].object != EMPTY) {
                                new->b->plateau[r][c].object = EMPTY;
                                count[t]--;
                                done = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    apply_gravity_down(new->b);
    return new;
}

void game_next_level(game *g)
{
    if (!g) return;

    printf("Passage au niveau suivant...\n");
    int next_level = g->score->level + 1;

    if (g->puzzle) {
        game *ng = game_create_puzzle(g->b->largeur, g->b->hauteur, g->lines, g->upgrades,
                                      g->refill_mode, 0, true, g->gravity_control);

        /* free current game board, replace */
        /* NOTE: this is minimal; for a full refactor, we can do cleaner ownership */
        for (int r = 0; r < g->b->hauteur; r++) free(g->b->plateau[r]);
        free(g->b->plateau);

        if (g->b->q) {
            while (!isempty_queue(g->b->q)) {
                void *p = dequeue(g->b->q);
                free(p);
            }
            delete_queue(g->b->q);
        }

        free(g->b);

        g->b = ng->b;
        free(ng);

    } else {
        game *ng = game_create_classic(g->b->largeur, g->b->hauteur, g->lines,
                                       g->score->target_score + g->score->scale_score,
                                       g->score->scale_score, g->nodelay,
                                       g->upgrades, g->easy, g->gravity_gems,
                                       g->bombs);

        for (int r = 0; r < g->b->hauteur; r++) free(g->b->plateau[r]);
        free(g->b->plateau);
        free(g->b);

        g->b = ng->b;
        free(ng);
    }

    g->score->level = next_level;
}

void game_release(game *g)
{
    if (!g) return;

    printf("Liberation du jeu...\n");

    if (g->b) {
        if (g->b->plateau) {
            for (int r = 0; r < g->b->hauteur; r++) {
                free(g->b->plateau[r]);
            }
            free(g->b->plateau);
        }

        if (g->b->q) {
            /* elements are cell* allocated in refill_mode==1 */
            while (!isempty_queue(g->b->q)) {
                void *p = dequeue(g->b->q);
                free(p);
            }
            delete_queue(g->b->q);
        }

        free(g->b);
    }

    free(g->score);
    free(g);
}

/* Deep clone: board + grid + refill queue */
game *game_clone(game *g)
{
    if (!g) return NULL;

    printf("Clonage du jeu...\n");

    game *clone = malloc(sizeof(game));
    if (!clone) {
        fprintf(stderr, "Out of memory: game_clone(game)\n");
        exit(EXIT_FAILURE);
    }

    /* clone board */
    clone->b = malloc(sizeof(board));
    if (!clone->b) {
        fprintf(stderr, "Out of memory: game_clone(board)\n");
        exit(EXIT_FAILURE);
    }

    clone->b->hauteur = g->b->hauteur;
    clone->b->largeur = g->b->largeur;
    clone->b->gravite = g->b->gravite;

    clone->b->select = g->b->select;
    clone->b->x = g->b->x;
    clone->b->y = g->b->y;
    clone->b->x2 = g->b->x2;
    clone->b->y2 = g->b->y2;

    clone->b->plateau = malloc(clone->b->hauteur * sizeof(cell *));
    if (!clone->b->plateau) {
        fprintf(stderr, "Out of memory: game_clone(plateau)\n");
        exit(EXIT_FAILURE);
    }

    for (int r = 0; r < clone->b->hauteur; r++) {
        clone->b->plateau[r] = malloc(clone->b->largeur * sizeof(cell));
        if (!clone->b->plateau[r]) {
            fprintf(stderr, "Out of memory: game_clone(plateau row)\n");
            exit(EXIT_FAILURE);
        }
        for (int c = 0; c < clone->b->largeur; c++) {
            clone->b->plateau[r][c] = g->b->plateau[r][c];
        }
    }

    /* clone queue */
    clone->b->q = NULL;
    clone->b->queue_size = g->b->queue_size;

    if (g->b->q) {
        clone->b->q = create_queue();

        /* drain g->b->q into temp array, restore, and deep-copy into clone */
        size_t cap = 16, n = 0;
        void **tmp = malloc(cap * sizeof(*tmp));
        if (!tmp) {
            fprintf(stderr, "Out of memory: game_clone(queue tmp)\n");
            exit(EXIT_FAILURE);
        }

        while (!isempty_queue(g->b->q)) {
            void *v = dequeue(g->b->q);
            if (n == cap) {
                cap *= 2;
                void **nt = realloc(tmp, cap * sizeof(*tmp));
                if (!nt) {
                    free(tmp);
                    fprintf(stderr, "Out of memory: game_clone(queue tmp realloc)\n");
                    exit(EXIT_FAILURE);
                }
                tmp = nt;
            }
            tmp[n++] = v;
        }

        /* restore original + clone */
        for (size_t k = 0; k < n; k++) {
            enqueue(tmp[k], g->b->q);

            cell *src = (cell *)tmp[k];
            if (src) {
                cell *cpy = malloc(sizeof(cell));
                if (!cpy) {
                    fprintf(stderr, "Out of memory: game_clone(queue cell)\n");
                    exit(EXIT_FAILURE);
                }
                *cpy = *src;
                enqueue(cpy, clone->b->q);
            } else {
                enqueue(NULL, clone->b->q);
            }
        }

        free(tmp);
    }

    /* clone params */
    clone->puzzle = g->puzzle;
    clone->lines = g->lines;
    clone->upgrades = g->upgrades;
    clone->gravity_control = g->gravity_control;
    clone->nodelay = g->nodelay;
    clone->easy = g->easy;
    clone->gravity_gems = g->gravity_gems;
    clone->bombs = g->bombs;
    clone->refill_mode = g->refill_mode;

    /* clone score */
    clone->score = malloc(sizeof(score_block));
    if (!clone->score) {
        fprintf(stderr, "Out of memory: game_clone(score)\n");
        exit(EXIT_FAILURE);
    }

    *clone->score = *g->score;

    return clone;
}