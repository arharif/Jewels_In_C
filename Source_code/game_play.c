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
    game_end_signal = SIGNAL_END_LEVEL; // Signals the end of a level in classic mode.
}

void game_classic_end(void)
{
    game_end_signal = SIGNAL_END_CLGAME; // Signals the end of the game in classic mode.
}

void game_puzzle_end(void)
{
    game_end_signal = SIGNAL_END_PUZZLE; // Signals the end of a puzzle game.
}

// * * * * *  Ajout Fonctions Tests Principaux  * * * * * //

bool combo_block(board *b, int x, int y, direction dir, int size, int i, int j)
{
    return false;
}

bool combo_line(board *b, int x, int y, direction dir, int* n, int i, int j)
{
    if (x<0 || y<0 || x>=b->hauteur || y>=b->largeur || b->plateau[x][y].object != b->plateau[i][j].object) return false;
    if (*n == 0) return true;

    (*n)--;

    switch (dir) {
    case DOWN:
        return combo_line(b, x+1, y, dir, n, i, j);
    case UP:
        return combo_line(b, x-1, y, dir, n, i, j);
    case LEFT:
        return combo_line(b, x, y-1, dir, n, i, j);
    case RIGHT:
        return combo_line(b, x, y+1, dir, n, i, j);
    }
}

bool valid_swap(game *g, int i, int j, int i2, int j2)
{
    // Cas des gemmes spéciales...

    int n;
    if (g->lines) {
            n=1;
            if (combo_line(g->b, j+1, i, DOWN, &n, j, i) || combo_line(g->b, j-1, i, UP, &n, j, i)) return true;
            n=1;
            if (combo_line(g->b, j, i+1, RIGHT, &n, j, i) || combo_line(g->b, j, i-1, LEFT, &n, j, i)) return true;
            n=1;
            if (combo_line(g->b, j2+1, i2, DOWN, &n, j2, i2) || combo_line(g->b, j2-1, i2, UP, &n, j2, i2)) return true;
            n=1;
            if (combo_line(g->b, j2, i2+1, RIGHT, &n, j2, i2) || combo_line(g->b, j2, i2-1, LEFT, &n, j2, i2)) return true;
        }

    else {
        // SCC (à finir)
        return false;
    }
}

bool mark_kill(game* g)
{
    // Hum..
    return false;
}

bool mark_fall(game* g)
{
    // Optimiser..
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
    // même case
    else if (x==i && y==j) g->b->select = false;
    // case éloignée
    else if (abs(i-x)>1 || abs(j-y)>1 || (abs(i-x) && abs(j-y))) {
        g->b->x = i;
        g->b->y = j;
    }
    // case adjacente
    else {
        g->b->x2 = i;
        g->b->y2 = j;
        SWAP(g->b->plateau[y][x], g->b->plateau[j][i]);
        // échange valide ?
        if (valid_swap(g, x,y, i,j)) {
            game_init_animation(ANIM_MOVE_GOODSWAP);
        }
        // sinon rollback
        else {
            game_init_animation(ANIM_MOVE_BADSWAP);
        }
        g->b->select = false;
    }
}

void game_handle_hint_request(game *g)
{
    bool ok = false;
    for (int i=0; i<g->b->hauteur-1; i++)
        for (int j=0; j<g->b->largeur-1; j++) {
            SWAP(g->b->plateau[j][i], g->b->plateau[j][i+1]);
            if (valid_swap(g, i,j, i+1,j)) ok = true;
            SWAP(g->b->plateau[j][i], g->b->plateau[j][i+1]);
            SWAP(g->b->plateau[j][i], g->b->plateau[j+1][i]);
            if (valid_swap(g, i,j, i,j+1)) ok = true;
            SWAP(g->b->plateau[j][i], g->b->plateau[j+1][i]);
            if (ok) {
                g->b->select = false;
                g->b->x2 = i;
                g->b->y2 = j;
                game_init_animation(ANIM_HINT);
                return;
            }
        }
    // cas impossible
    g->b->x = -1;
    g->b->y = -1;
    g->b->select = true;
}

void game_handle_detonation(game *g)
{
    // lancer mark_kill.g
    // animation KILL si True

    return;
}

void game_set_gravity(game *g, direction dir)
{
    // pour chaque case en partant du sol (direction gravité)
        // tant que vide en dessous
            // nfall.i.j ++
        // la déplacer à nfall case vers le sol
    // animation FALL si besoin !

    return;
}

void game_cycle_gravity(game *g)
{
    // lancer set_gravity avec (dir+1)%4

    return;
}

void game_handle_refill(game *g, int k)
{
    // Si file vide : rien

    // mettre l'objet à l'indice k (côté opposé gravité)
    // le faire tomber (méthode gravity)
    // animation FALL

    return;
}

void game_end_animation(game *g, anim_type anim)
{
    switch (anim)
    {

    // Cas BADSWAP ou ROLLBACK ou HINT:
    case ANIM_MOVE_BADSWAP:
        SWAP(g->b->plateau[g->b->y2][g->b->x2], g->b->plateau[g->b->y][g->b->x]);
        game_init_animation(ANIM_MOVE_ROLLBACK);
        break;

    case ANIM_MOVE_ROLLBACK:
    case ANIM_HINT:
        game_init_animation(ANIM_IDLE);

    // Cas GOODSWAP :
    // lancer détonation
    // (ou KILL si on optimise -> marquer en même temps que tester si valide)

    // Cas KILL :
    //
    // Si mark_fall.g alors anim fall
    // Sinon en IDLE

    // Cas FALL :
    // Si mark_kill.g alors anim kill
    // Sinon end IDLE

    // Cas IDLE :
        // Tester si le score est atteint
            // game_classic_score

        // Tester s'il reste une possibilité en classic (méthode hint)
            // retour animation IDLE
            // sinon game_classic_end
        
        // Tester si le jeu est vide en puzzle
            // game_puzzle_end
    
    }
    return;
}

/*************************************************************************************/
/*+ Functions used by the GUI to retrieve information when it performs an animation +*/
/*+ You must implement these functions so that the GUI can display the animations   +*/
/*************************************************************************************/

direction game_get_cell_move(game *g, int i, int j, int *n)
{
    if (game_anim_signal == ANIM_MOVE_FALL) {
        *n = g->b->plateau[j][i].fall;
        return g->b->gravite;
    }
    // Si SWAP_MOVE
    
    if (i == g->b->x && j == g->b->y)
    {
        *n = 1;
        return (((g->b->x2>g->b->x) ? RIGHT : (g->b->x2<g->b->x) ? LEFT : (g->b->y2>g->b->y) ? DOWN : UP)+2)%4;
    }

    if (i == g->b->x2 && j == g->b->y2)
    {
        *n = 1;
        return ((g->b->x2>g->b->x) ? RIGHT : (g->b->x2<g->b->x) ? LEFT : (g->b->y2>g->b->y) ? DOWN : UP);
    }

    // Pas de move
    *n=NULL;
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
