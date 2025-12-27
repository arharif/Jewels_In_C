#include "game_play.h"

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
 * Internal helpers (static to avoid -Wmissing-prototypes)
 * ======================================================================================= */

static bool combo_line(board *b, int x, int y, direction dir, int *n, int i, int j)
{
    if (b == NULL || n == NULL) return false;

    if (x < 0 || y < 0 || x >= b->hauteur || y >= b->largeur) return false;
    if (b->plateau[x][y].object != b->plateau[i][j].object) return false;

    if (*n == 0) return true;

    (*n)--;

    switch (dir) {
    case DOWN:
        return combo_line(b, x + 1, y, dir, n, i, j);
    case UP:
        return combo_line(b, x - 1, y, dir, n, i, j);
    case LEFT:
        return combo_line(b, x, y - 1, dir, n, i, j);
    case RIGHT:
        return combo_line(b, x, y + 1, dir, n, i, j);
    default:
        return false;
    }
}

static bool valid_swap(game *g, int i, int j, int i2, int j2)
{
    if (g == NULL || g->b == NULL) return false;

    /* Minimal “lines” validity test: after swap, at least one of the 2 cells is in a line of 3 */
    if (g->lines) {
        int n;

        /* Check around first swapped cell (j,i) */
        n = 1;
        if (combo_line(g->b, j + 1, i, DOWN, &n, j, i) ||
            combo_line(g->b, j - 1, i, UP,   &n, j, i)) return true;

        n = 1;
        if (combo_line(g->b, j, i + 1, RIGHT, &n, j, i) ||
            combo_line(g->b, j, i - 1, LEFT,  &n, j, i)) return true;

        /* Check around second swapped cell (j2,i2) */
        n = 1;
        if (combo_line(g->b, j2 + 1, i2, DOWN, &n, j2, i2) ||
            combo_line(g->b, j2 - 1, i2, UP,   &n, j2, i2)) return true;

        n = 1;
        if (combo_line(g->b, j2, i2 + 1, RIGHT, &n, j2, i2) ||
            combo_line(g->b, j2, i2 - 1, LEFT,  &n, j2, i2)) return true;

        return false;
    }

    /* SCC mode not implemented yet */
    return false;
}

static bool mark_kill(game *g)
{
    (void)g;
    return false;
}

static bool mark_fall(game *g)
{
    (void)g;
    return false;
}

/**********************************************************/
/*+ Functions called by the GUI to modify the game state +*/
/**********************************************************/

void game_handle_mouse_click(game *g, int i, int j)
{
    int x, y;
    if (!board_get_picked(g->b, &x, &y)) {
        g->b->x = i;
        g->b->y = j;
        g->b->select = true;
    }
    else if (x == i && y == j) {
        g->b->select = false;
    }
    else if (abs(i - x) > 1 || abs(j - y) > 1 || (abs(i - x) && abs(j - y))) {
        g->b->x = i;
        g->b->y = j;
    }
    else {
        g->b->x2 = i;
        g->b->y2 = j;
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
    bool ok = false;
    for (int i = 0; i < g->b->hauteur - 1; i++)
        for (int j = 0; j < g->b->largeur - 1; j++) {
            SWAP(g->b->plateau[j][i], g->b->plateau[j][i + 1]);
            if (valid_swap(g, i, j, i + 1, j)) ok = true;
            SWAP(g->b->plateau[j][i], g->b->plateau[j][i + 1]);

            SWAP(g->b->plateau[j][i], g->b->plateau[j + 1][i]);
            if (valid_swap(g, i, j, i, j + 1)) ok = true;
            SWAP(g->b->plateau[j][i], g->b->plateau[j + 1][i]);

            if (ok) {
                g->b->select = false;
                g->b->x2 = i;
                g->b->y2 = j;
                game_init_animation(ANIM_HINT);
                return;
            }
        }

    g->b->x = -1;
    g->b->y = -1;
    g->b->select = true;
}

void game_handle_detonation(game *g)
{
    if (mark_kill(g)) game_init_animation(ANIM_KILL);
    else game_init_animation(ANIM_IDLE);
}

void game_set_gravity(game *g, direction dir)
{
    (void)dir;

    if (mark_fall(g)) {
        game_init_animation(ANIM_MOVE_FALL);
    } else if (mark_kill(g)) {
        game_init_animation(ANIM_KILL);
    } else {
        game_init_animation(ANIM_IDLE);
    }
}

void game_cycle_gravity(game *g)
{
    g->b->gravite = (direction)((g->b->gravite + 1) % 4);
    game_set_gravity(g, g->b->gravite);
}

void game_handle_refill(game *g, int k)
{
    (void)g;
    (void)k;
    game_init_animation(ANIM_IDLE);
}

void game_end_animation(game *g, anim_type anim)
{
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
        game_handle_detonation(g);
        break;

    case ANIM_KILL:
        game_set_gravity(g, g->b->gravite);
        break;

    case ANIM_MOVE_FALL:
        if (mark_kill(g)) game_init_animation(ANIM_KILL);
        else game_init_animation(ANIM_IDLE);
        break;

    case ANIM_IDLE:
        game_init_animation(ANIM_IDLE);
        break;

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
    if (n == NULL) return SIZE_DIRECTION;

    if (game_anim_signal == ANIM_MOVE_FALL) {
        *n = g->b->plateau[j][i].fall;
        return g->b->gravite;
    }

    if (i == g->b->x && j == g->b->y)
    {
        *n = 1;
        return (direction)((((g->b->x2 > g->b->x) ? RIGHT :
                             (g->b->x2 < g->b->x) ? LEFT  :
                             (g->b->y2 > g->b->y) ? DOWN  : UP) + 2) % 4);
    }

    if (i == g->b->x2 && j == g->b->y2)
    {
        *n = 1;
        return (direction)(((g->b->x2 > g->b->x) ? RIGHT :
                            (g->b->x2 < g->b->x) ? LEFT  :
                            (g->b->y2 > g->b->y) ? DOWN  : UP));
    }

    *n = 0;
    return SIZE_DIRECTION;
}

gem_kill game_get_cell_kill(game *g, int i, int j)
{
    gem_kill actu = g->b->plateau[j][i].kill;
    g->b->plateau[j][i].kill = KILL_NOKILL;
    return actu;
}

bool game_retrieve_hint(game *g, int *i, int *j)
{
    *i = g->b->x2;
    *j = g->b->y2;
    return !g->b->select;
}
