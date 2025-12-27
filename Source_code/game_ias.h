#ifndef GAME_IAS_H_
#define GAME_IAS_H_

#include "game.h"

typedef enum { IA_RANDOM, IA_SAFE, IA_AGRESSIVE, IA_CLASSIC_SIZE } ia_classic;

extern bool (*ia_functions[IA_CLASSIC_SIZE])(game *);

extern char *ia_names[IA_CLASSIC_SIZE];

bool ia_random(game *g);

bool ia_safe(game *g);

bool ia_agressive(game *g);

#endif
