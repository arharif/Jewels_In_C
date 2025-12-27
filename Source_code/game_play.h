#ifndef GAME_PLAY_H_
#define GAME_PLAY_H_

#include "game.h"
#include "game_solver.h"

/*********************************************************************
 * Fonctions qui démarre une animation (ou revient au mode IDLE).    *
 *                                                                   *
 * Déjà écrite : appelez-la mais NE LA MODIFIEZ PAS                  *
 *********************************************************************/

typedef enum {
    ANIM_IDLE,
    ANIM_MOVE_GOODSWAP,
    ANIM_MOVE_BADSWAP,
    ANIM_MOVE_ROLLBACK,
    ANIM_MOVE_FALL,
    ANIM_KILL,
    ANIM_HINT,
    ANIM_SIZE
} anim_type;

extern anim_type game_anim_signal;

void game_init_animation(anim_type type);

/*********************************************************************
 * Fonctions qui terminent la partie.                                *
 *                                                                   *
 * Déjà écrites : appelez-les quand la quand la partie est terminée. *
 *                                                                   *
 * NE PAS MODIFIER                                                   *
 *********************************************************************/

typedef enum {
    SIGNAL_END_LEVEL,
    SIGNAL_END_CLGAME,
    SIGNAL_END_PUZZLE,
    SIGNAL_SIZE
} end_signal;

extern end_signal game_end_signal;

void game_classic_score(void);

void game_classic_end(void);

void game_puzzle_end(void);

/*******************************************************************************
 * Fonctions appelées par l'interface pour gérer les entrées et les            *
 * modifications de l'état du jeu. Vous devez implémenter ces fonctions (elles *
 * démarrent des animations).                                                  *
 *******************************************************************************/

void game_handle_mouse_click(game *g, int i, int j);

void game_handle_hint_request(game *g);

void game_handle_detonation(game *g);

void game_set_gravity(game *g, direction dir);

void game_cycle_gravity(game *g);

void game_handle_refill(game *g, int k);

void game_end_animation(game *g, anim_type type);

/****************************************************************************
 * Fonctions utilisées par l'interface pour récupérer des informations       *
 * lorsqu'elle +* exécute une animation.Vous devez implémenter ces fonctions *
 * pour que les animations s'affichent correctement.                         *
 *****************************************************************************/

direction game_get_cell_move(game *g, int i, int j, int *distance);



gem_kill game_get_cell_kill(game *g, int i, int j);

bool game_retrieve_hint(game *g, int *i, int *j);

#endif // GAME_PLAY_H_
