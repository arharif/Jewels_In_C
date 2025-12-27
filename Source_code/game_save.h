#ifndef GAME_SAVE_H
#define GAME_SAVE_H

/*   ____                        ____                        */
/*  / ___| __ _ _ __ ___   ___  / ___|  __ ___   _____  ___  */
/* | |  _ / _` | '_ ` _ \ / _ \ \___ \ / _` \ \ / / _ \/ __| */
/* | |_| | (_| | | | | | |  __/  ___) | (_| |\ V /  __/\__ \ */
/*  \____|\__,_|_| |_| |_|\___| |____/ \__,_| \_/ \___||___/ */

#include "game.h"
#include "game_scores.h"

typedef enum {
    SK_WIDTH,
    SK_HEIGHT,
    SK_BOARD,
    SK_GRAVITY,
    SK_REFILL_QUEUE,
    SK_PUZZLE,
    SK_LINES,
    SK_UPGRADES,
    SK_NODELAY,
    SK_EASY,
    SK_WRAP,
    SK_BOMBS,
    SK_REFILL,
    SK_GRAVITY_CONTROL,
    SK_LEVEL,
    SK_LEVEL_SCORE,
    SK_TARGET_SCORE,
    SK_SCALE_SCORE,
    SK_SCORE_ARRAY,
    SK_REFILL_COUNT,
    SK_SIZE
} save_keywords;

extern const char *save_keyword_strings[SK_SIZE];

bool save_game(game *g, const char *filepath);

game *load_game(const char *filepath);

#endif // GAME_SAVE_H
