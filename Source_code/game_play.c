#include "game_play.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/***************************************************************************************************************/
/*+ Function that starts an animation (or return to idle mode). Already written: call it but DO NOT MODIFY it +*/
/***************************************************************************************************************/

anim_type game_anim_signal = ANIM_SIZE;

void game_init_animation(anim_type type)
{
    if (type < 0 || type >= ANIM_SIZE)
    {
        fprintf(stderr, "Invalid animation type in game_init_animation.\n");
        exit(EXIT_FAILURE);
    }
    game_anim_signal = type;
}

/**********************************************************************************************************/
/*+ Functions that ends the game. Already written: call it when the player has lost but DO NOT MODIFY it +*/
/**********************************************************************************************************/

end_signal game_end_signal = SIGNAL_SIZE;

void game_classic_score(void)
{
    game_end_signal = SIGNAL_END_LEVEL;
}

void game_classic_end(void)
{
    game_end_signal = SIGNAL_END_CLGAME;
}

void game_puzzle_end(void)
{
    game_end_signal = SIGNAL_END_PUZZLE;
}

/* =======================================================================================
 * Internal helpers
 * ======================================================================================= */

static void seed_rng_once(void)
{
    static bool seeded = false;
    if (!seeded) {
        seeded = true;
        srand((unsigned)time(NULL));
    }
}

/* Correct convention (matches your GUI/controller usage):
 * - width  = b->largeur  (x, columns)
 * - height = b->hauteur  (y, rows)
 * - access: plateau[y][x]
 */
static inline bool in_bounds(const board *b, int x, int y)
{
    return (b && x >= 0 && y >= 0 && x < b->largeur && y < b->hauteur);
}

static inline bool is_matchable(element e)
{
    /* Only basic jewels match (AMBER..TOPAZ) */
    return (e >= AMBER && e <= TOPAZ);
}

static element random_basic_element(void)
{
    seed_rng_once();
    /* AMBER..TOPAZ are 7 values */
    return (element)(AMBER + (rand() % 7));
}

static void reset_fall(board *b)
{
    if (!b) return;
    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            b->plateau[y][x].fall = 0;
        }
    }
}

/* Total run length through (x,y) along an axis (dx,dy) + (-dx,-dy) */
static int run_length_axis(const board *b, int x, int y, int dx, int dy)
{
    if (!in_bounds(b, x, y)) return 0;

    element e = b->plateau[y][x].object;
    if (!is_matchable(e)) return 0;

    int count = 1;

    /* forward */
    int xf = x + dx, yf = y + dy;
    while (in_bounds(b, xf, yf) && b->plateau[yf][xf].object == e) {
        count++;
        xf += dx;
        yf += dy;
    }

    /* backward */
    int xb = x - dx, yb = y - dy;
    while (in_bounds(b, xb, yb) && b->plateau[yb][xb].object == e) {
        count++;
        xb -= dx;
        yb -= dy;
    }

    return count;
}

/* Is (x,y) part of any 3+ run ? */
static bool is_in_match(const board *b, int x, int y)
{
    if (!in_bounds(b, x, y)) return false;
    element e = b->plateau[y][x].object;
    if (!is_matchable(e)) return false;

    /* horizontal axis */
    if (run_length_axis(b, x, y, 1, 0) >= 3) return true;
    /* vertical axis */
    if (run_length_axis(b, x, y, 0, 1) >= 3) return true;

    return false;
}

/* Valid swap: after swap already done, check if either swapped cell makes a match */
static bool valid_swap(game *g, int x1, int y1, int x2, int y2)
{
    if (!g || !g->b) return false;
    board *b = g->b;

    if (!in_bounds(b, x1, y1) || !in_bounds(b, x2, y2)) return false;

    return is_in_match(b, x1, y1) || is_in_match(b, x2, y2);
}

/* Mark kills for all 3+ matches (basic version) */
static bool mark_kill(game *g)
{
    if (!g || !g->b) return false;
    board *b = g->b;

    bool any = false;

    /* reset previous kill marks */
    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            b->plateau[y][x].kill = KILL_NOKILL;
        }
    }

    /* mark any cell that belongs to a match */
    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            if (is_in_match(b, x, y)) {
                b->plateau[y][x].kill = KILL_SHRINK;
                any = true;
            }
        }
    }

    return any;
}

/* =======================================================================================
 * Scoring
 * ======================================================================================= */

/* combo counter (cascades): 0 for first kill, 1 for second, ... */
static int s_combo = 0;

static void score_on_kill(game *g)
{
    if (!g || !g->b || !g->score) return;

    board *b = g->b;

    int killed_total = 0;
    int killed_flame = 0;
    int killed_star  = 0;
    int killed_grav  = 0;
    int killed_hyper = 0;

    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            if (b->plateau[y][x].kill == KILL_NOKILL) continue;

            element e = b->plateau[y][x].object;
            if (e == EMPTY) continue;

            /* Count kills */
            killed_total++;

            if (e == HYPERCUBE) killed_hyper++;

            /* Count special powers if you use them later */
            if (e >= AMBER && e <= TOPAZ) {
                if (b->plateau[y][x].power == GEM_FLAME)   killed_flame++;
                if (b->plateau[y][x].power == GEM_STAR)    killed_star++;
                if (b->plateau[y][x].power == GEM_GRAVITY) killed_grav++;
            }
        }
    }

    if (killed_total <= 0) return;

    /* Basic scoring model:
       - 10 points per gem
       - combo multiplier: 1x, 2x, 3x, ... for cascades
    */
    const int base_points = 10;
    int mult = 1 + s_combo;         /* first kill = 1, second = 2, ... */
    int gained = killed_total * base_points * mult;

    g->score->scores[SC_CONSUMED] += killed_total;

    g->score->scores[SC_FLAMED]     += killed_flame;
    g->score->scores[SC_STARRED]    += killed_star;
    g->score->scores[SC_GRAVITY]    += 0;           /* gravity usage is tracked elsewhere */
    g->score->scores[SC_HYPERCUBES] += killed_hyper;

    g->score->level_score += gained;
    g->score->scores[SC_LEVEL]  = g->score->level_score;
    g->score->scores[SC_GLOBAL] += gained;

    /* next cascade will have higher multiplier */
    s_combo++;
}

static void maybe_end_classic_level(game *g)
{
    if (!g || !g->score) return;
    if (g->puzzle) return;

    /* target_score can be -1 in demo => ignore if <= 0 */
    if (g->score->target_score > 0 &&
        g->score->level_score >= g->score->target_score) {
        game_classic_score();
    }
}

/* =======================================================================================
 * Kill & fall application
 * ======================================================================================= */

static void apply_kill(board *b)
{
    if (!b) return;

    for (int y = 0; y < b->hauteur; y++) {
        for (int x = 0; x < b->largeur; x++) {
            if (b->plateau[y][x].kill != KILL_NOKILL) {
                b->plateau[y][x].object = EMPTY;
                b->plateau[y][x].kill   = KILL_NOKILL;
                b->plateau[y][x].fall   = 0;
                b->plateau[y][x].power  = GEM_BASIC;
            }
        }
    }
}

/* Helper to move a cell and record its fall distance (used by mark_fall)
 * coords are (row=y, col=x)
 */
static void move_cell(board *b,
                      int src_y, int src_x,
                      int dst_y, int dst_x,
                      int dist,
                      bool *moved)
{
    if (!b) return;
    if (src_y == dst_y && src_x == dst_x) {
        b->plateau[dst_y][dst_x].fall = 0;
        return;
    }

    cell tmp = b->plateau[src_y][src_x];

    /* clear source */
    b->plateau[src_y][src_x].object = EMPTY;
    b->plateau[src_y][src_x].kill   = KILL_NOKILL;
    b->plateau[src_y][src_x].fall   = 0;
    b->plateau[src_y][src_x].power  = GEM_BASIC;

    /* write destination */
    b->plateau[dst_y][dst_x] = tmp;
    b->plateau[dst_y][dst_x].kill = KILL_NOKILL;
    b->plateau[dst_y][dst_x].fall = (dist > 0 ? dist : 0);

    if (moved) *moved = true;
}

/* Apply gravity + refill empties; sets fall distances for animation */
static bool mark_fall(game *g)
{
    if (!g || !g->b) return false;
    board *b = g->b;

    reset_fall(b);
    seed_rng_once();

    bool moved = false;

    switch (b->gravite)
    {
    case DOWN: {
        for (int x = 0; x < b->largeur; x++) {
            int dst_y = b->hauteur - 1;

            for (int y = b->hauteur - 1; y >= 0; y--) {
                if (b->plateau[y][x].object != EMPTY) {
                    move_cell(b, y, x, dst_y, x, dst_y - y, &moved);
                    dst_y--;
                }
            }

            for (int y = dst_y; y >= 0; y--) {
                b->plateau[y][x].object = random_basic_element();
                b->plateau[y][x].power  = GEM_BASIC;
                b->plateau[y][x].kill   = KILL_NOKILL;
                b->plateau[y][x].fall   = (dst_y - y + 1);
                moved = true;
            }
        }
        break;
    }

    case UP: {
        for (int x = 0; x < b->largeur; x++) {
            int dst_y = 0;

            for (int y = 0; y < b->hauteur; y++) {
                if (b->plateau[y][x].object != EMPTY) {
                    move_cell(b, y, x, dst_y, x, y - dst_y, &moved);
                    dst_y++;
                }
            }

            for (int y = dst_y; y < b->hauteur; y++) {
                b->plateau[y][x].object = random_basic_element();
                b->plateau[y][x].power  = GEM_BASIC;
                b->plateau[y][x].kill   = KILL_NOKILL;
                b->plateau[y][x].fall   = (y - dst_y + 1);
                moved = true;
            }
        }
        break;
    }

    case RIGHT: {
        for (int y = 0; y < b->hauteur; y++) {
            int dst_x = b->largeur - 1;

            for (int x = b->largeur - 1; x >= 0; x--) {
                if (b->plateau[y][x].object != EMPTY) {
                    move_cell(b, y, x, y, dst_x, dst_x - x, &moved);
                    dst_x--;
                }
            }

            for (int x = dst_x; x >= 0; x--) {
                b->plateau[y][x].object = random_basic_element();
                b->plateau[y][x].power  = GEM_BASIC;
                b->plateau[y][x].kill   = KILL_NOKILL;
                b->plateau[y][x].fall   = (dst_x - x + 1);
                moved = true;
            }
        }
        break;
    }

    case LEFT: {
        for (int y = 0; y < b->hauteur; y++) {
            int dst_x = 0;

            for (int x = 0; x < b->largeur; x++) {
                if (b->plateau[y][x].object != EMPTY) {
                    move_cell(b, y, x, y, dst_x, x - dst_x, &moved);
                    dst_x++;
                }
            }

            for (int x = dst_x; x < b->largeur; x++) {
                b->plateau[y][x].object = random_basic_element();
                b->plateau[y][x].power  = GEM_BASIC;
                b->plateau[y][x].kill   = KILL_NOKILL;
                b->plateau[y][x].fall   = (x - dst_x + 1);
                moved = true;
            }
        }
        break;
    }

    default:
        break;
    }

    return moved;
}

/**********************************************************/
/*+ Functions called by the GUI to modify the game state +*/
/**********************************************************/

void game_handle_mouse_click(game *g, int i, int j)
{
    if (!g || !g->b) return;

    int x, y;

    if (!board_get_picked(g->b, &x, &y)) {
        g->b->x = i;
        g->b->y = j;
        g->b->select = true;
    }
    else if (x == i && y == j) {
        g->b->select = false;
    }
    else if (abs(i - x) + abs(j - y) != 1) {
        g->b->x = i;
        g->b->y = j;
    }
    else {
        g->b->x2 = i;
        g->b->y2 = j;

        /* count swap attempt */
        if (g->score) g->score->scores[SC_SWAPS]++;

        SWAP(g->b->plateau[y][x], g->b->plateau[j][i]);

        if (valid_swap(g, x, y, i, j)) {
            game_init_animation(ANIM_MOVE_GOODSWAP);
        } else {
            game_init_animation(ANIM_MOVE_BADSWAP);
        }

        g->b->select = false;
    }
}

void game_handle_hint_request(game *g)
{
    if (!g || !g->b) return;

    bool ok = false;

    int w = g->b->largeur;
    int h = g->b->hauteur;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            if (x + 1 < w) {
                SWAP(g->b->plateau[y][x], g->b->plateau[y][x + 1]);
                if (valid_swap(g, x, y, x + 1, y)) ok = true;
                SWAP(g->b->plateau[y][x], g->b->plateau[y][x + 1]);

                if (ok) {
                    g->b->select = false;
                    g->b->x2 = x;
                    g->b->y2 = y;
                    game_init_animation(ANIM_HINT);
                    return;
                }
            }

            if (y + 1 < h) {
                SWAP(g->b->plateau[y][x], g->b->plateau[y + 1][x]);
                if (valid_swap(g, x, y, x, y + 1)) ok = true;
                SWAP(g->b->plateau[y][x], g->b->plateau[y + 1][x]);

                if (ok) {
                    g->b->select = false;
                    g->b->x2 = x;
                    g->b->y2 = y;
                    game_init_animation(ANIM_HINT);
                    return;
                }
            }
        }
    }

    g->b->x = -1;
    g->b->y = -1;
    g->b->x2 = -1;
    g->b->y2 = -1;
    g->b->select = true;
}

void game_handle_detonation(game *g)
{
    if (!g) return;

    /* new elimination sequence => reset combo counter */
    s_combo = 0;

    if (mark_kill(g)) {
        game_init_animation(ANIM_KILL);
    } else {
        game_init_animation(ANIM_IDLE);
    }
}

void game_set_gravity(game *g, direction dir)
{
    if (!g || !g->b) return;
    g->b->gravite = dir;

    /* track gravity usage */
    if (g->score) g->score->scores[SC_GRAVITY]++;

    if (mark_fall(g)) {
        game_init_animation(ANIM_MOVE_FALL);
    } else {
        game_init_animation(ANIM_IDLE);
    }
}

void game_cycle_gravity(game *g)
{
    if (!g || !g->b) return;
    g->b->gravite = (direction)((g->b->gravite + 1) % 4);
    game_set_gravity(g, g->b->gravite);
}

void game_handle_refill(game *g, int k)
{
    (void)k;
    if (!g || !g->b) return;

    if (g->score) g->score->refills++;

    if (mark_fall(g)) game_init_animation(ANIM_MOVE_FALL);
    else game_init_animation(ANIM_IDLE);
}

void game_end_animation(game *g, anim_type anim)
{
    if (!g || !g->b) {
        game_init_animation(ANIM_IDLE);
        return;
    }

    switch (anim)
    {
    case ANIM_MOVE_BADSWAP:
        SWAP(g->b->plateau[g->b->y2][g->b->x2], g->b->plateau[g->b->y][g->b->x]);
        game_init_animation(ANIM_MOVE_ROLLBACK);
        break;

    case ANIM_MOVE_ROLLBACK:
    case ANIM_HINT:
        game_init_animation(ANIM_IDLE);
        break;

    case ANIM_MOVE_GOODSWAP:
        /* new elimination sequence => reset combo counter */
        s_combo = 0;
        game_handle_detonation(g);
        break;

    case ANIM_KILL:
        /* SCORE MUST BE UPDATED BEFORE apply_kill() clears things */
        score_on_kill(g);

        apply_kill(g->b);

        if (mark_fall(g)) game_init_animation(ANIM_MOVE_FALL);
        else {
            maybe_end_classic_level(g);
            game_init_animation(ANIM_IDLE);
        }
        break;

    case ANIM_MOVE_FALL:
        if (mark_kill(g)) game_init_animation(ANIM_KILL);
        else {
            maybe_end_classic_level(g);
            game_init_animation(ANIM_IDLE);
        }
        break;

    case ANIM_IDLE:
    default:
        game_init_animation(ANIM_IDLE);
        break;
    }
}

/*************************************************************************************/
/*+ Functions used by the GUI to retrieve information when it performs an animation +*/
/*************************************************************************************/

direction game_get_cell_move(game *g, int i, int j, int *n)
{
    if (!g || !g->b || !n) return SIZE_DIRECTION;

    if (game_anim_signal == ANIM_MOVE_FALL) {
        *n = g->b->plateau[j][i].fall;
        return g->b->gravite;
    }

    if (i == g->b->x && j == g->b->y) {
        *n = 1;
        direction d =
            (g->b->x2 > g->b->x) ? RIGHT :
            (g->b->x2 < g->b->x) ? LEFT  :
            (g->b->y2 > g->b->y) ? DOWN  : UP;

        return (direction)((d + 2) % 4);
    }

    if (i == g->b->x2 && j == g->b->y2) {
        *n = 1;
        direction d =
            (g->b->x2 > g->b->x) ? RIGHT :
            (g->b->x2 < g->b->x) ? LEFT  :
            (g->b->y2 > g->b->y) ? DOWN  : UP;

        return d;
    }

    *n = 0;
    return SIZE_DIRECTION;
}

gem_kill game_get_cell_kill(game *g, int i, int j)
{
    if (!g || !g->b) return KILL_NOKILL;
    return g->b->plateau[j][i].kill;
}

bool game_retrieve_hint(game *g, int *i, int *j)
{
    if (!g || !g->b || !i || !j) return false;
    *i = g->b->x2;
    *j = g->b->y2;
    return (g->b->x2 >= 0 && g->b->y2 >= 0);
}