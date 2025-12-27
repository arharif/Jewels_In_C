#include "game.h"
#include "alloc.h"
#include "gui_gamevars.h"
#include "queue.h"

/*************************************************************************/
/*  Functions called by the GUI to retrieve information on a game board  */
/*************************************************************************/

int board_get_width(board* b) {
    return b->largeur;
}
// on revoie la valeur defini dans la structure 
int board_get_height(board* b) {
    return b->hauteur;
}
// on revoie la valeur defini dans la structure 
direction board_get_gravity(board* b) {
    return b->gravite;
}
// on revoie la valeur defini dans la structure 
bool board_get_picked(board* b, int* i, int* j) {
    *i=b->x;
    *j=b->y;
    return b->select;
}

void board_get_cell(board* b, int i, int j, cell* C) {

    

    *C = b->plateau[j][i] ; 
}

int board_refill_size(board* b) {
     if (!b->q) return -1;
    else return b->queue_size;
}

static void *queue_read(queue* q, int i){
    queue *aux = create_queue();
    int count = 0;
    void *res = NULL;
    while(!isempty_queue(q)){
        void *temp = dequeue(q);
        enqueue(temp,aux);
        if(count == i){
            res = temp;
        }
        count++;
    }
    while(!isempty_queue(aux)){
        enqueue(dequeue(aux),q);
    }
    delete_queue(aux);
    return res;
}
bool board_read_refill(board* b, int k, cell* c) {
    if (b->q != NULL) {
        cell* tmp = queue_read(b->q, k);
        *c = *tmp;
        if(!c) return false;
        return true;
    }
    return false;
}

/******************************************************************************/
/*+ Functions used by the GUI to create and update a new game in classic mode */
/******************************************************************************/

bool bloc_verification(cell** plateau, int x, int y, direction dir, int i, int j, int* count) {

    if (i<0 || j<0 || i>x || (i==x && j>y) || plateau[i][j].object != plateau[x][y].object) return false;
    if (*count == 0) { *count = 3; return true; }

    (*count)--;
    return (dir!=DOWN && bloc_verification(plateau, x, y, UP, i+1, j, count)) ||
            (dir!=UP && bloc_verification(plateau, x, y, DOWN, i-1, j, count)) ||
            (dir!=LEFT && bloc_verification(plateau, x, y, RIGHT, i, j-1, count)) ||
            (dir!=RIGHT && bloc_verification(plateau, x, y, LEFT, i, j+1, count));
    }

game *game_create_classic(int width, int height, bool lines, int target_score,
                          int scale_score, bool nodelay, bool upgrades,
                          bool easy_mode, bool gravity_gems, bool bombs) {

    game* new = malloc(sizeof(game));

    // Initialisation du board >
    new->b = malloc(sizeof(board));
    new->b->largeur = width;
    new->b->hauteur = height;
    new->b->gravite = DOWN;
    new->b->select = false;
    new->b->x = new->b->y = 0;
    new->b->q= NULL;
    new->b->queue_size = 0;
    new->b->plateau = malloc(height* sizeof(cell*));

    for (int i = 0; i < height; i++) {
        new->b->plateau[i] = malloc(width * sizeof(cell));
        for (int j = 0; j < width; j++) {
            new->b->plateau[i][j].object = rand() % (ELEM_SIZE - 4);
            new->b->plateau[i][j].power = GEM_BASIC;

            if (lines) // Option ligne
                while ((j > 1 && new->b->plateau[i][j].object == new->b->plateau[i][j-1].object && 
                                new->b->plateau[i][j].object == new->b->plateau[i][j-2].object) || 
                       (i > 1 && new->b->plateau[i][j].object == new->b->plateau[i-1][j].object && 
                                new->b->plateau[i][j].object == new->b->plateau[i-2][j].object))
                    new->b->plateau[i][j].object = rand() % (ELEM_SIZE-4);

            else { // Option SCC
                int count = 3;
                while (bloc_verification(new->b->plateau, i, j, DOWN, i, j, &count))
                    new->b->plateau[i][j].object = rand() % (ELEM_SIZE-4);
            }
        }
    }

    // Initialisation des paramètres >
    new->puzzle = false;
    new->lines = lines;
    new->upgrades = upgrades;
    new->gravity_control = 0;
    new->nodelay = nodelay;
    new->easy = easy_mode;
    new->gravity_gems = gravity_gems;
    new->bombs = bombs;
    new->refill_mode = 2;


    // Initialisation du score block >
    new->score = malloc(sizeof(score_block));
    new->score->level = 1;
    new->score->level_score = 0;
    new->score->target_score = target_score;
    new->score->scale_score = scale_score;
    for (score_type x=0; x<SC_SIZE; x++) new->score->scores[x] = 0;
    new->score->refills = 0;

    return new;
}
/*
static void gravity(game* new){

    int width = new->b->largeur;
    int height = new->b->hauteur;
    short gravity_ctr = new->b->gravite;

    if(gravity_ctr == 0){
        for (int x = 0; x < width; x++) {
        int y = height - 1;

            while (x > 0) {  // on évite i == 0 pour protéger i-1
                if (new->b->plateau[y][x].object == EMPTY &&
                    new->b->plateau[y-1][x].object != EMPTY) {

                    // on fait tomber le gem
                    new->b->plateau[y][x].object = new->b->plateau[y-1][x].object;
                    new->b->plateau[y-1][x].object = EMPTY;
                } else {
                    x--; // on remonte seulement si pas de chute
                }
            }
        }
    }else if (gravity_ctr == 1){
        for (int x = 0; x < width; x++) {
        int y = height - 1;

            while (x > 0) {  // on évite i == 0 pour protéger i-1
                if (new->b->plateau[y][x].object == EMPTY &&
                    new->b->plateau[y][x+1].object != EMPTY) {

                    // on fait tomber le gem
                    new->b->plateau[y][x].object = new->b->plateau[y][x+1].object;
                    new->b->plateau[y][x+1].object = EMPTY;
                } else {
                    x--; // on remonte seulement si pas de chute
                }
            }
        }
    }else if (gravity_ctr == 2){
        for (int x = 0; x < width; x++) {
        int y = height - 1;

            while (x > 0) {  // on évite i == 0 pour protéger i-1
                if (new->b->plateau[y][x].object == EMPTY &&
                    new->b->plateau[y+1][x].object != EMPTY) {

                    // on fait tomber le gem
                    new->b->plateau[y][x].object = new->b->plateau[y+1][x].object;
                    new->b->plateau[y+1][x].object = EMPTY;
                } else {
                    x--; // on remonte seulement si pas de chute
                }
            }
        }
    }else if (gravity_ctr == 3){
        for (int x = 0; x < width; x++) {
        int y = height - 1;

            while (x > 0) {  // on évite i == 0 pour protéger i-1
                if (new->b->plateau[y][x].object == EMPTY &&
                    new->b->plateau[y][x-1].object != EMPTY) {

                    // on fait tomber le gem
                    new->b->plateau[y][x].object = new->b->plateau[y][x-1].object;
                    new->b->plateau[y][x].object = EMPTY;
                } else {
                    x--; // on remonte seulement si pas de chute
                }
            }
        }


    }else{
        return;
    }
    
}*/




/*
static void switch_cells(game *g, int i, int j, int x, int y){
    cell tmp = g->b->plateau[i][j];
    g->b->plateau[i][j] = g->b->plateau[x][y];
    g->b->plateau[x][y] = tmp;
}

static void make_gems_fall(game *g, int i, int j){
    int count = 0;
    int x;
    for(x = i - 1; x >= 0; x--){
        if(g->b->plateau[x][j].object == EMPTY){
            count ++;
        }
    }
    for(int y = i; count > 0 && x>=0 && g->b->plateau[x][j].object != EMPTY; count--){
        switch_cells(g, x, j, y, j);
    }
}

static bool up_no_empty(game *g, int i, int j){
    for(int x = i - 1; x >= 0; x--){
        if(g->b->plateau[x][j].object != EMPTY){
            return true;
        }
    }
    return false;
}

static void fall_down(game *g){
    for (int i = g->b->hauteur - 1; i >= 0; i--){
        for(int j = 0; j < g->b->largeur; j++){
            if(g->b->plateau[i][j].object == EMPTY){
                if(up_no_empty(g,i,j)){
                    make_gems_fall(g,i,j);
                }
            }
        }
    }
}*/



static void apply_gravity_down(board *b)
{
    int height = b->hauteur;
    int width  = b->largeur;

    for (int col = 0; col < width; col++) {

        int write_row = height - 1;

        // On parcourt la colonne de bas en haut
        for (int row = height - 1; row >= 0; row--) {

            if (b->plateau[row][col].object != EMPTY) {

                if (row != write_row) {
                    b->plateau[write_row][col] = b->plateau[row][col];
                }

                write_row--;
            }
        }

        // On remplit le reste avec EMPTY
        for (int row = write_row; row >= 0; row--) {
            b->plateau[row][col].object = EMPTY;
            b->plateau[row][col].power  = GEM_BASIC; // sécurité
        }
    }
}

game *game_create_puzzle(int width, int height, bool lines, bool upgrades,
                         short refill_mode, int ini, bool balanced,
                         short gravity_control) {
    
    game* new = malloc(sizeof(game));
    // Création du board
    new->b = malloc(sizeof(board));
    new->b->largeur = width;
    new->b->hauteur = height;
    new->b->gravite = DOWN;
    new->b->select = false;
    new->b->x = 0;
    new->b->y = 0;
    new->b->q = create_queue();
    new->b->queue_size = 0;

    new->b->plateau = malloc(height * sizeof(cell*));
    // Plateau
      for (int i = 0; i < height; i++) {
        new->b->plateau[i] = malloc(width * sizeof(cell));
        for (int j = 0; j < width; j++) {
            if (rand() % 3 == 0)
                new->b->plateau[i][j].object = EMPTY;
            else
                new->b->plateau[i][j].object = rand() % (ELEM_SIZE - 4);

            new->b->plateau[i][j].power = GEM_BASIC;

            if (lines) // Option ligne
                while ((j > 1 && new->b->plateau[i][j].object == new->b->plateau[i][j-1].object && 
                                new->b->plateau[i][j].object == new->b->plateau[i][j-2].object) || 
                       (i > 1 && new->b->plateau[i][j].object == new->b->plateau[i-1][j].object && 
                                new->b->plateau[i][j].object == new->b->plateau[i-2][j].object))
                    new->b->plateau[i][j].object = rand() % ((element)ELEM_SIZE-4);
                    

            else{ // Option SCC
                int count = 3;
                while (bloc_verification(new->b->plateau, i, j, DOWN, i-1, j, &count) ||
                       bloc_verification(new->b->plateau, i, j, RIGHT, i, j-1, &count))
                    new->b->plateau[i][j].object = rand() % ((element)ELEM_SIZE-4);
            }
        }
    }
    apply_gravity_down(new->b);

    // Initialisation des paramètres >
    new->puzzle = true;
    new->lines = lines;
    new->upgrades = upgrades;
    new->refill_mode = refill_mode;
    new->gravity_control = gravity_control;
    new->nodelay = true;
    new->easy = false;
    new->gravity_gems = false;
    new->bombs = false;

    // Initialisation du score block >
    new->score = malloc(sizeof(score_block));
    new->score->level = 1;
    new->score->level_score = 0;
    new->score->target_score = 0;
    new->score->scale_score = 0;
    for (score_type i = 0; i < SC_SIZE; i++) {
    new->score->scores[i] = 0;
    }
    new->score->refills = 0;


    if (refill_mode == 2) {
        delete_queue(new->b->q);
            new->b->q = NULL;
    }

    if (refill_mode == 0) { 
        new->b->q = create_queue();
        new->b->queue_size = 0;
    }   

    if (refill_mode == 1) {
        new->b->q = create_queue();
        if (ini > width * height) ini = width * height;

        for (int k = 0; k < ini; k++) {
            cell *tmp = malloc(sizeof(cell));
            if (rand() % 2 == 0){
                tmp->object = ROCK;
                tmp->power = 0;
            }else{
                tmp->object = BOMB;
                tmp->countdown = rand() % 16;
            }

            enqueue(tmp, new->b->q);
            new->b->queue_size++;
        }
    }

// Mode balanced : forcer multiple de 3 --- à corriger ---


    
    for (int x = 0; x < width; x++) {
        int y = height -2;
        // printf("height = %d, y = %d", height, y);
        while (y >= 1) {  
            if (new->b->plateau[y][x].object == EMPTY &&
                new->b->plateau[y-1][x].object != EMPTY ) {

                new->b->plateau[y][x].object = new->b->plateau[y-1][x].object;
                new->b->plateau[y-1][x].object = EMPTY;
             
            }else if(new->b->plateau[y][x].object != EMPTY &&
                    new->b->plateau[y+1][x].object == EMPTY){
                new->b->plateau[y][x].object = new->b->plateau[y+1][x].object;
                new->b->plateau[y+1][x].object = EMPTY;
            }else{
                y--; 
            }
        }
    }

    if (balanced) {
        int count[ELEM_SIZE] = {0};
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                count[new->b->plateau[i][j].object]++;
            }
        }

        for (int t = 0; t < ELEM_SIZE - 1; t++) {
            if (count[t] % 3 == 2) {
                int done = 0;
                for (int i = height -1; i >= 0 && done == 0; i--) {
                    for (int j = 0; j < width; j++) {
                        if (new->b->plateau[i][j].object == EMPTY){

                            new->b->plateau[i][j].object = t;
                            count[t]++;
                            printf("%d ", count[t]);
        
                            done ++;
                            break;
                        }
                    }
                    
                }

            }else if(count[t] % 3 == 1){
                int done = 0;

                // On parcourt de bas a haut 
                for (int i = height - 1; i >= 0 && !done; i--) {
                    for (int j = 0; j < width; j++) {

                        if (new->b->plateau[i][j].object ==(element) t) {

                            if (i == height - 1 || new->b->plateau[i+1][j].object != EMPTY) {

                                new->b->plateau[i][j].object = EMPTY;
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

/*S'assurer qu'il n'y a PAS de gemme au-dessus du vide --- à corriger ---
        for (int j = 0; j < width; j++) {
            int i = height - 1;

             while (i > 0) {  // on évite i == 0 pour protéger i-1
                 if (new->b->plateau[j][i].object == EMPTY &&
                     new->b->plateau[j][i-1].object != EMPTY) {

                     // on fait tomber le gem
                     new->b->plateau[j][i-1].object = new->b->plateau[j][i-1].object;
                     new->b->plateau[j][i-1].object = EMPTY;
                 } else {
                     i--; // on remonte seulement si pas de chute
                 }
             }
        }

*/
    return new;

}

void game_next_level(game* g) {

    printf("Passage au niveau suivant...\n");

    int level = g->score->level + 1;

    if (g->puzzle) { // améliore :)
        game *new_game = game_create_puzzle(g->b->largeur, g->b->hauteur, g->lines, g->upgrades, 
                                            g->refill_mode, 0, true, g->gravity_control);
        g->b = new_game->b;
        free(new_game);
    }

    else g = game_create_classic (g->b->largeur, g->b->hauteur, g->lines,
                              g->score->target_score + g->score->scale_score,
                              g->score->scale_score, g->nodelay,
                              g->upgrades, g->easy, g->gravity_gems,
                              g->bombs);
    
    g->score->level = level;
}

void game_release(game* g) {

    printf("Liberation du jeu...\n");

    for (int i = 0; i < g->b->hauteur; i++) {
        free(g->b->plateau[i]);
    }
    free(g->b->plateau);
    free(g->b);
    free(g->score);
    free(g);
}                                                                  

game *game_clone(game* g) {

    printf("Clonage du jeu...\n");

    game* clone = malloc(sizeof(game));
    // Copie du board >
    clone->b = malloc(sizeof(board));
    clone->b->hauteur = g->b->hauteur;
    clone->b->largeur = g->b->largeur;
    clone->b->gravite = g->b->gravite;
    clone->b->select = g->b->select;
    //-- clone->b->stockage = g->b->stockage;
    clone->b->plateau = malloc(clone->b->hauteur * sizeof(cell*));
    for (int i = 0; i < clone->b->hauteur; i++) {
        clone->b->plateau[i] = malloc(clone->b->largeur * sizeof(cell));
        for (int j = 0; j < clone->b->largeur; j++) {
            clone->b->plateau[i][j] = g->b->plateau[i][j];
        }
    }
    // Copie des paramètres >
    clone->puzzle = g->puzzle;
    clone->lines = g->lines;
    clone->upgrades = g->upgrades;
    clone->gravity_control = g->gravity_control;
    clone->nodelay = g->nodelay;
    clone->easy = g->easy;
    clone->gravity_gems = g->gravity_gems;
    clone->bombs = g->bombs;
    clone->refill_mode = g->refill_mode;
    // Copie du score block >
    clone->score = malloc(sizeof(score_block));
    clone->score->level = g->score->level;
    clone->score->level_score = g->score->level_score;
    clone->score->target_score = g->score->target_score;
    clone->score->scale_score = g->score->scale_score;
    for (score_type x=0; x<SC_SIZE; x++) clone->score->scores[x] = g->score->scores[x];
    clone->score->refills = g->score->refills;

    return clone;
}



