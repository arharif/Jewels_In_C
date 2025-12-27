/* game_ias.c */
#include "game_ias.h"
#include "game_play.h"
#include <stdlib.h>
#include <time.h>

/* Define the globals declared as extern in game_ias.h */
bool (*ia_functions[IA_CLASSIC_SIZE])(game *) = { ia_random, ia_safe, ia_agressive };
char *ia_names[IA_CLASSIC_SIZE] = { "Random", "Safe", "Agressive" };

static void seed_rng_once(void)
{
    static int seeded = 0;
    if (!seeded) {
        seeded = 1;
        srand((unsigned)time(NULL));
    }
}

/* Very simple IA: try random adjacent swaps until one is valid.
   Returns true if it started an action (swap animation), false otherwise. */
bool ia_random(game *g)
{
    if (!g || !g->b) return false;

    seed_rng_once();

    int w = g->b->largeur;
    int h = g->b->hauteur;
    if (w <= 1 && h <= 1) return false;

    /* Try a few random attempts */
    for (int tries = 0; tries < 50; tries++) {
        int x = rand() % w;
        int y = rand() % h;

        /* pick a random neighbor direction */
        int r = rand() % 4;
        int nx = x, ny = y;
        if (r == 0) nx = x + 1;
        if (r == 1) nx = x - 1;
        if (r == 2) ny = y + 1;
        if (r == 3) ny = y - 1;

        if (nx < 0 || nx >= w || ny < 0 || ny >= h) continue;

        /* simulate two clicks (select then swap) */
        game_handle_mouse_click(g, x, y);
        game_handle_mouse_click(g, nx, ny);

        /* If the move was invalid, your game_play will rollback automatically. */
        return true;
    }

    return false;
}

/* For now: same behavior as random (you can improve later) */
bool ia_safe(game *g)
{
    return ia_random(g);
}

bool ia_agressive(game *g)
{
    return ia_random(g);
}