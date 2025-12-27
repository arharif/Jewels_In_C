/**
 * @file gui_controller.h
 * @brief
 * Game loop. DO NOT MODIFY.
 *
 * @attention
 * This part is already implemented. It should not be modified.
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "gui_rendering.h"
#include <time.h>

/*   ____                        _                    */
/*  / ___| __ _ _ __ ___   ___  | | ___   ___  _ __   */
/* | |  _ / _` | '_ ` _ \ / _ \ | |/ _ \ / _ \| '_ \  */
/* | |_| | (_| | | | | | |  __/ | | (_) | (_) | |_) | */
/*  \____|\__,_|_| |_| |_|\___| |_|\___/ \___/| .__/  */
/*                                            |_|     */

/***********************/
/*+ General functions +*/
/***********************/

/**
 * @brief
 * Checks whether the given coordinates are valid in a game board.
 */
bool ctl_valid_coordinates(game *g, //!< The game.
                           int i,   //!< The horizontal coordinate.
                           int j    //!< The vertical coordinate.
);

/**
 * @brief
 * Checks whether the given border coordinates are valid in a game board.
 *
 * @remark
 * Used to handle refill.
 *
 * @return
 * Returns the index along the border if valid, INT_MAX otherwise.
 */
int ctl_valid_border_coordinates(game *g, //!< The game.
                                 int i,   //!< The horizontal coordinate.
                                 int j    //!< The vertical coordinate.
);

/**
 * @brief
 * Checks the validity of a game structure.
 *
 * @return
 * Returns true if the game structure is valid, false otherwise.
 */
bool ctl_check_game(game *g,      //!< The game.
                    bool messages //!< Whether to display error messages on the terminal.
);

/**
 * @brief
 * Retrieves the game parameters from a game structure and stores them in the global variables.
 */
void ctl_get_params(game *g //!< The game.
);

/**
 * @brief Releases the global game.
 */
void ctl_kill_game(void);

/**
 * @brief
 * Flags to indicate whether the fill buttons are allocated.
 */
extern bool ctl_fillbuttons_used;

/**
 * @brief
 * Pointers to the fill buttons for each direction.
 */
extern bool *ctl_fillbuttons[SIZE_DIRECTION];

/**
 * @brief
 * Number of fill buttons for each direction.
 */
extern int ctl_fillbuttons_count[SIZE_DIRECTION];

/**
 * @brief
 * Initializes the storage buttons for refill mode.
 */
void ctl_init_storage_buttons(int w, //!< The width of the game.
                              int h  //!< The height of the game.
);

/**
 * @brief
 * Releases the storage buttons for refill mode.
 */
void ctl_release_storage_buttons(void);

/**
 * @brief
 * Updates the game timer.
 */
void ctl_update_game_timer(void);

/**
 * @brief Handles general keyboard events (not specific to a menu).
 */
void ctl_keyboard_general_menu(SDL_KeyCode key //!< The key.
);

/**
 * @brief Handles transmitted animation signals.
 */
void ctl_handle_anim_signals(void);

/**
 * @brief Handles transmitted end signals.
 */
void ctl_handle_end_signals(void);

/********************************/
/*+ Management of score screen +*/
/********************************/

/**
 * @brief Handles mouse inputs in the score screen.
 */
bool ctl_mouse_score_screen(int x, int y);

/**
 * @brief Handles keyboard inputs in the score screen.
 */
void ctl_button_push_score(void);

/************************/
/*+ Management of zoom +*/
/************************/

/**
 * @brief Handles keyboard inputs for zoom control.
 */
bool ctl_keyboard_zoom(SDL_KeyCode key);

/*********************************/
/*+ Management of gravity wheel +*/
/*********************************/

/**
 * @brief Handles keyboard inputs in the gravity wheel.
 */
bool ctl_keyboard_gravitywheel(SDL_KeyCode key //!< The key.
);

/**
 * @brief Handles mouse clicks in the gravity wheel.
 */
bool ctl_mouse_gravitywheel(int x, //!< Horizontal coordinate of the mouse.
                            int y  //!< Vertical coordinate of the mouse.
);

/**
 * @brief Handles a button push in the gravity wheel.
 */
void ctl_button_push_gravitywheel(void);

/*****************************/
/*+ Management of main menu +*/
/*****************************/

/**
 * @brief Initializes the demo game (special classic game displayed behind the menus).
 */
void ctl_init_demo_game(void);

/**
 * @brief Handles the keyboard on the main menu.
 */
void ctl_keyboard_mainmenu(SDL_KeyCode key //!< The key.
);

/**
 * @brief Handles the mouse on the main menu.
 */
bool ctl_mouse_mainmenu(int x, //!< Horizontal coordinate of the mouse.
                        int y  //!< Vertical coordinate of the mouse.
);

/**
 * @brief Handles the mouse on the classic setup menu.
 */
bool ctl_mouse_classic_setup(int x, //!< Horizontal coordinate of the mouse.
                             int y  //!< Vertical coordinate of the mouse.
);

/**
 * @brief Handles the mouse on the puzzle setup menu.
 */
bool ctl_mouse_puzzle_setup(int x, //!< Horizontal coordinate of the mouse.
                            int y  //!< Vertical coordinate of the mouse.
);

/**
 * @brief Enters the menu mode.
 */
void ctl_enter_menu(bool initdemo //!< Whether to initialize the demo game.
);

/**
 * @brief Enters the setup menu for classic mode.
 */
void ctl_enter_setupclassic(void);

/**
 * @brief Enters the setup menu for puzzle mode.
 */
void ctl_enter_setuppuzzle(void);

/**
 * @brief Enters the highscores menu.
 */
void ctl_enter_highscores(void);

/**
 * @brief Handles a button push in the main menu.
 */
void ctl_button_push_mainmenu(void);

/**
 * @brief Handles a button push in the classic setup menu.
 */
void ctl_button_push_setupclassic(void);

/**
 * @brief Handles a button push in the puzzle setup menu.
 */
void ctl_button_push_setuppuzzle(void);

/********************************/
/*+ Management of classic mode +*/
/********************************/

/**
 * @brief Initializes a classic game.
 */
void ctl_init_classic(void);

/**
 * @brief Initializes a new level in classic mode.
 */
void ctl_newlevel_classic(void);

/**
 * @brief Ends the current level or the whole game in classic mode and displays the score screen.
 *
 * @details Called when the player has completed the level or has lost the game.
 */
void ctl_end_classic(bool lost //!< Whether the game was lost (if false, a level was completed).
);

/**
 * @brief Enters the classic menu.
 */
void ctl_menu_classic(void);

/**
 * @brief Resumes the classic gameplay when in a menu (not when the game is lost).
 */
void ctl_gameplay_classic(void);

/**
 * @brief Handles the keyboard in classic mode.
 */
bool ctl_keyboard_classicmode(SDL_KeyCode key //!< The key.
);

/**
 * @brief Handles the mouse in classic mode.
 */
bool ctl_mouse_classicmode(int x, //!< Horizontal coordinate of the mouse.
                           int y  //!< Vertical coordinate of the mouse.
);

/**
 * @brief Handles a button push in classic mode.
 */
void ctl_button_push_classic(void);

/*******************************/
/*+ Management of puzzle mode +*/
/*******************************/

/**
 * @brief Initializes a puzzle game.
 */
void ctl_init_puzzle(void);

/**
 * @brief Restores a backup of the current puzzle game.
 */
void ctl_restore_puzzle(void);

/**
 * @brief Ends the current puzzle game.
 *
 * @details Called when the player has completed the puzzle.
 */
void ctl_end_puzzle(void);

/**
 * @brief Enters the puzzle menu.
 */
void ctl_menu_puzzle(void);

/**
 * @brief Resumes the puzzle gameplay when in a menu (not when the puzzle is completed).
 */
void ctl_gameplay_puzzle(void);

/**
 * @brief Solves the current puzzle game.
 */
void ctl_solver_puzzle(solver mode //!< The solving mode.
);

/**
 * @brief Implements the solution found by the solver in puzzle mode.
 */
void ctl_implement_solution_puzzle(void);

/**
 * @brief Handles a button push in puzzle mode.
 */
void ctl_button_push_puzzle(void);

/**
 * @brief Handles keyboards inputs in puzzle mode.
 */
bool ctl_keyboard_puzzle(SDL_KeyCode key //!< The key.
);

/**
 * @brief Handles mouse clicks in puzzle mode.
 */
bool ctl_mouse_puzzle(int x, //!< Horizontal coordinate of the mouse.
                      int y  //!< Vertical coordinate of the mouse.
);

/*****************************/
/*+ Management of test mode +*/
/*****************************/

/**
 * @brief Initializes a new test game from the menu.
 */
void ctl_test_initial(bool puzzlemode //!< Whether to initialize in puzzle mode.
);

/**
 * @brief Generates a new test puzzle game within test mode.
 */
void ctl_test_puzzle(void);

/**
 * @brief Generates a new test classic game within test mode.
 */
void ctl_test_classic(void);

/**
 * @brief Loads a test game from a file within test mode.
 */
void ctl_test_load(const char *filepath);

/**
 * @brief Loads a game from a file (in the main menu).
 */
void ctl_load_game(const char *filepath);

/**
 * @brief Handles the keyboard on the test mode menu.
 */
bool ctl_keyboard_testmodemenu(SDL_KeyCode //!< The key.
);

/**
 * @brief Handles the mouse on the test mode menu.
 */
bool ctl_mouse_testmodemenu(int x, //!< Horizontal coordinate of the mouse.
                            int y  //!< Vertical coordinate of the mouse.
);

/**
 * @brief Handles a button push in test mode.
 */
void ctl_button_push_testmode(void);

/***********************/
/*+ Display functions +*/
/***********************/

/**
 * @brief Display.
 */
void ctl_display(void);

/****************/
/*+ Game state +*/
/****************/

/**
 * @brief Handles a solver turn in puzzle mode.
 */
void ctl_solver_turn(void);

/*******************/
/*+ Button pushes +*/
/*******************/

/**
 * @brief
 * Handles a button push.
 *
 * @details
 * Calls the appropriate function depending on the current mode.
 */
void ctl_button_push(void);

/***************************/
/*+ Enacting user actions +*/
/***************************/

/**
 * @brief Handles the mouse motion.
 */
void ctl_mouse_motion(SDL_Event * //!< The Event to get the mouse position.
);

/**
 * @brief Handles a mouse click.
 */
void ctl_mouse_click(SDL_Event * //!< The Event to get the mouse position.
);

/**
 * @brief Handles a mouse unclick.
 */
void ctl_mouse_unclick(SDL_Event * //!< The Event to get the mouse position.
);

/**
 * @brief Handles the keyboard input.
 */
void ctl_input(SDL_Event * //!< The Event to get the key used.
);

/**
 * @brief Handles the return key in edit fields.
 */
void ctl_handle_edit_return(void);

/***************/
/*+ Game loop +*/
/***************/

/**
 * @brief The game loop.
 */
void event_loop(void);

#endif // CONTROLLER_H_
