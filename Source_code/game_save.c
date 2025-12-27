#include "game_save.h"
#include "game_scores.h"

const char *save_keyword_strings[SK_SIZE] = {
    "width",
    "height",
    "board",
    "gravity",
    "refill_queue",
    "puzzle",
    "lines",
    "upgrades",
    "nodelay",
    "easy",
    "wrap",
    "bombs",
    "refill",
    "gravity_control",
    "level",
    "level_score",
    "target_score",
    "scale_score",
    "score_array",
    "refill_count"};


bool save_game(game *g, const char *filepath)
{
    if (g == NULL || filepath == NULL)
        return false;

    FILE *f = fopen(filepath, "w");
    if (f == NULL)
        return false;

    board *b = g->b;

    fprintf(f, "%s %d\n", save_keyword_strings[SK_WIDTH], b->largeur);
    fprintf(f, "%s %d\n", save_keyword_strings[SK_HEIGHT], b->hauteur);
    fprintf(f, "%s %d\n", save_keyword_strings[SK_PUZZLE], g->puzzle);
    fprintf(f, "%s %d\n", save_keyword_strings[SK_LINES], g->lines);
    fprintf(f, "%s %d\n", save_keyword_strings[SK_UPGRADES], g->upgrades);

    fprintf(f, "%s\n", save_keyword_strings[SK_BOARD]);
    for (int i = 0; i < b->hauteur; i++) {
        for (int j = 0; j < b->largeur; j++) {
            fprintf(f, "%d ", b->plateau[i][j].object);
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return true;
}



game *load_game(const char *filepath)
{
    if (filepath == NULL)
        return NULL;

    FILE *f = fopen(filepath, "r");
    if (f == NULL)
        return NULL;

    int width = 0, height = 0;
    int puzzle = 0, lines = 0, upgrades = 0;

    char key[32];

    while (fscanf(f, "%31s", key) == 1) {

        if (strcmp(key, save_keyword_strings[SK_WIDTH]) == 0)
            fscanf(f, "%d", &width);

        else if (strcmp(key, save_keyword_strings[SK_HEIGHT]) == 0)
            fscanf(f, "%d", &height);

        else if (strcmp(key, save_keyword_strings[SK_PUZZLE]) == 0)
            fscanf(f, "%d", &puzzle);

        else if (strcmp(key, save_keyword_strings[SK_LINES]) == 0)
            fscanf(f, "%d", &lines);

        else if (strcmp(key, save_keyword_strings[SK_UPGRADES]) == 0)
            fscanf(f, "%d", &upgrades);

        else if (strcmp(key, save_keyword_strings[SK_BOARD]) == 0)
            break;
    }

    if (width <= 0 || height <= 0) {
        fclose(f);
        return NULL;
    }

    game *g = game_create_classic(
        width, height,
        lines,
        100, 10,
        true,
        upgrades,
        false,
        false,
        false
    );

    g->puzzle = puzzle;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int obj;
            fscanf(f, "%d", &obj);
            g->b->plateau[i][j].object = (element)obj;
        }
    }

    fclose(f);
    return g;
}

