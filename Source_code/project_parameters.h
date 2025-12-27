#ifndef _PROJECT_PARAMETERS_H_
#define _PROJECT_PARAMETERS_H_

/***********************/
/*+ Window dimensions +*/
/***********************/

#define WINDOW_SCREENWIDTH   1820

#define WINDOW_SCREENHEIGHT  980

/************************/
/*+ Default parameters +*/
/************************/

#define GAME_WIDTH           8

#define GAME_HEIGHT          8

#define GAME_LINES           true

#define GAME_NODELAY         true

#define GAME_EASY            false

#define GAME_WRAP            false

#define GAME_BOMBS           false

#define GAME_UPGRADES        true

#define GAME_REFILL_MODE     1

#define GAME_BALANCED        true

#define GAME_STORAGE_INITIAL 2

#define GAME_CLEARSCORE      10000

#define GAME_CLEARMULTI      50

/***************************/
/*+ Test mode parameters +*/
/***************************/

#define TEST_WIDTH           8

#define TEST_HEIGHT          8

#define TEST_PUZZLE          false

#define TEST_LINES           true

#define TEST_NODELAY         true

#define TEST_EASY            false

#define TEST_WRAP            false

#define TEST_BOMBS           false

#define TEST_UPGRADES        true

#define TEST_REFILL_MODE     1

#define TEST_STORAGE_INITIAL 2

#define TEST_BALANCED        true

/***********************/
/*+ Solver limitation +*/
/***********************/

#define SOLVER_MAX_CONFIGS   4000000

#endif // _PROJECT_PARAMETERS_H_
