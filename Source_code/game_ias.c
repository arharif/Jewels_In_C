#include "game_ias.h"
#include "game_play.h"

bool (*ia_functions[IA_CLASSIC_SIZE])(game *) = {ia_random, ia_safe, ia_agressive};
char *ia_names[IA_CLASSIC_SIZE] = {"Random", "Safe", "Agressive"};

bool ia_random(game *)
{
    return false;
}

bool ia_safe(game *)
{
    return false;
}
bool ia_agressive(game *)
{
    return false;
}
