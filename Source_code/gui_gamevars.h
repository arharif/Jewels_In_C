/**
 * @file gui_gamevars.h
 * @brief
 * Global variables used by the interface. DO NOT MODIFY.
 *
 * @attention
 * This part is already implemented. It should not be modified.
 */

#ifndef GAMEVARS_H_
#define GAMEVARS_H_

/*   ____ _       _           _                   _       _     _            */
/*  / ___| | ___ | |__   __ _| | __   ____ _ _ __(_) __ _| |__ | | ___  ___  */
/* | |  _| |/ _ \| '_ \ / _` | | \ \ / / _` | '__| |/ _` | '_ \| |/ _ \/ __| */
/* | |_| | | (_) | |_) | (_| | |  \ V / (_| | |  | | (_| | |_) | |  __/\__ \ */
/*  \____|_|\___/|_.__/ \__,_|_|   \_/ \__,_|_|  |_|\__,_|_.__/|_|\___||___/ */

#include "game_ias.h"
#include "game_play.h"
#include "game_scores.h"
#include "game_solver.h"
#include "gui_textbuf.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>

/****************************/
/*+ Time counter functions +*/
/****************************/

/**
 * @brief
 * Initializes the time counter.
 *
 * @remark
 * This function is called by the interface. You should NOT call it yourself.
 */
void init_time_counter(void);

/**
 * @brief
 * Initializes the time counter with a specific start time.
 *
 * @param start
 * The time in milliseconds at which the counter should start.
 *
 * @remark
 * This function is called by the interface. You should NOT call it yourself.
 */
void init_time_counter_start(long start //!< The time in milliseconds at which the counter should start.
);
/**
 * @brief
 * Pauses the time counter.
 *
 * @remark
 * This function is called by the interface. You should NOT call it yourself.
 */
void pause_time_counter(void);

/**
 * @brief
 * Resumes the time counter.
 *
 * @remark
 * This function is called by the interface. You should NOT call it yourself.
 */
void resume_time_counter(void);

/**
 * @brief
 * Gets the time counter.
 *
 * @return
 * The time counter in milliseconds.
 *
 * @remark
 * This is function that you can call to retrieve the time a snake spent alive to fill a score table.
 */
int get_time_counter(void);

/***********/
/*+ Flags +*/
/***********/

/**
 * @brief Flags.
 */
typedef enum
{
    QUIT,
    HELP,
    PAUSE,
    NOGAMEINPUT,
    NOSCROLL,
    NOPLAYER,
    INMENU,
    PUSHED,
    MOUSEDOWN,
    KEYDOWN,
    NB_FLAGS
} ctl_flags;

/**
 * @brief Array of flags.
 */
extern bool flags[NB_FLAGS];

/**
 * @brief Activation of a flag.
 */
void set_flag(ctl_flags //!< The flag.
);

/**
 * @brief Deactivation of a flag.
 */
void remove_flag(ctl_flags //!< The flag
);

/*
 * @brief Inversion of a flag.
 */
void flip_flag(ctl_flags //!< The flag
);

/**
 * @brief Reset of all flags.
 */
void reset_flags(void);

/******************************/
/*+ Dimensions of the window +*/
/******************************/

/**
 * @brief Horizontal size of the window.
 */
extern int var_screen_width;

/**
 * @brief Vertical size of the window.
 */
extern int var_screen_height;

/**
 * @brief
 * Horizontal size of the information column displayed on the right.
 */
extern int var_column_width;

/**
 * @brief
 * Horizontal size of the play area (determined by the previous values).
 */
extern int var_playarea_width;

/*******************/
/*+ Mouse control +*/
/*******************/

/**
 * @brief Saves the horizontal coordinate of the mouse.
 */
extern int var_mousePosx;

/**
 * @brief Saves the vertical coordinate of the mouse.
 */
extern int var_mousePosy;

/**
 * @brief Records if the left mouse button is pushed (true) or not (false).
 */
extern bool var_leftMouseButtonDown;

/**
 * @brief Time at which the mouse was last clicked (used when the mouse button is released).
 */
extern long var_mouse_time;

/************************/
/*+ Zoom and Scrolling +*/
/************************/

/**
 * @brief
 * Current zoom level.
 *
 * @details
 * Can be 0 (no zoom), 1 (normal zoom) or 2 (double zoom).
 *
 * @remark
 * Default is 1.
 */
extern short var_render_zoom_level;

/**
 * @brief
 * The size of a single square tile at the current zoom level.
 *
 * @remark
 * Level 0 is 40 pixels, level 1 is 80 pixels and level 2 is 160 pixels.
 */
extern short var_tile_size;

/**
 * @brief
 * The vertical shift that needs to be applied in order to display a flamed gem at the current zoom level.
 *
 * @remark
 * Require since the sprite for flamed gems is not centered on the gem.
 */
extern short var_flamed_yshift;

/**
 * @brief
 * Initialization of the zoom level for a new game.
 */
void init_zoom_game(void);

/**
 * @brief
 * Horizontal scrolling offset.
 */
extern int var_scrollOffsetx;

/**
 * @brief
 * Vertical scrolling offset.
 */
extern int var_scrollOffsety;

/**
 * @brief
 * Horizontal coordinate of the first cell in the display area (in the board).
 */
extern int var_startx;

/**
 * @brief
 * Vertical coordinate of the first cell in the display area (in the board).
 */
extern int var_starty;

/**
 * @brief
 * Horizontal coordinate of the last cell in the display area (in the board).
 */
extern int var_endx;

/**
 * @brief
 * Vertical coordinate of the last cell in the display area (in the board).
 */
extern int var_endy;

/**
 * @brief
 * Horizontal coordinate of the first cell in the display area (on the screen).
 */
extern int var_topleftx;

/**
 * @brief
 * Vertical coordinate of the first cell in the display area (on the screen).
 */
extern int var_toplefty;

/**
 * @brief
 * Updates the variables that define the borders of the display area according
 * to the scrolling offsets.
 *
 * @details
 * The function updates the following variables according to the scrolling offsets:
 * var_startx, var_starty, var_endx, var_endy, var_topleftx and var_toplefty.
 */
void scroll_update_borders(void);

/**
 * @brief
 * Decrease the zoom level by one.
 */
void decrease_zoom(void);

/**
 * @brief
 * Increase the zoom level by one.
 */
void increase_zoom(void);

/**
 * @brief
 * Manual scrolling.
 */
void scroll_manual(int, //!< Horizontal scrolling.
                   int  //!< Vertical scrolling.
);

/**
 * @brief
 * Mouse scrolling.
 */
void scroll_with_mouse(int, //!< Horizontal scrolling.
                       int  //!< Vertical scrolling.
);

/**
 * @brief Centers the screen on a given board coordinate.
 */
void scroll_to_coords(int, //!< Horizontal coordinate.
                      int  //!< Vertical coordinate.
);

void scroll_to_center(int w, int h, int screen_width);

/*******************/
/*+ Keyboard text +*/
/*******************/

/**
 * @brief Internal names of the various values that can be edited.
 */
typedef enum
{
    // In Normal mode
    ED_WIDTH,          //!< Horizontal size.
    ED_TESTWIDTH,      //!< Horizontal size (test mode).
    ED_HEIGHT,         //!< Vertical size.
    ED_TESTHEIGHT,     //!< Vertical size (test mode).
    ED_SAVE,           //!< Save the current game.
    ED_LOAD,           //!< Load a saved game.
    ED_NAME,           //!< Enter player name for high scores.
    ED_CLEARSCORE,     //!< Score to clear the first level.
    ED_CLEARMULTI,     //!< Multiplier applied to the clear score after each level.
    ED_STORAGEINI,     //!< Initial storage size.
    ED_TESTSTORAGEINI, //!< Initial storage size (test mode).

    // In test mode
    ED_NBFLAMED,     //!< Initial number of flamed gems.
    ED_NBWRAPPED,    //!< Initial number of wrapped gems.
    ED_NBSTARRED,    //!< Initial number of starred gems.
    ED_NBHYPERCUBES, //!< Initial number of hypercubes.

    // Text
    ED_TEXT,

    ED_NONE, //!< Buffer is currently unused.
} edit_mode;

/**
 * @brief Global variable used to indicate the value which is currently being edited (ED_NONE if the buffer is unused).
 */
extern edit_mode var_edit_mode;

/**
 * @brief Global variable used for the buffer (NULL if the buffer is unused).
 */
extern text_buffer *var_editbuffer;

/**
 * @brief Initialization of the buffer
 */
void init_text_buffer(edit_mode //!< The value to edit.
);

/**
 * @brief Updates parameters from the buffer.
 */
void update_from_text_buffer(void);

/***********************************/
/*+ Game variables for generation +*/
/***********************************/

/**
 * @brief Horizontal size of the game board.
 */
extern int var_game_hsize;

/**
 * @brief Vertical size of the game board.
 */
extern int var_game_vsize;

/**
 * @brief Horizontal size of the game board (test mode).
 */
extern int var_test_game_hsize;

/**
 * @brief Vertical size of the game board (test mode).
 */
extern int var_test_game_vsize;

/**
 * @brief Score require to clear the first level (specific to classic mode).
 */
extern int var_game_clearscore;

/**
 * @brief Multiplier applied to the clear score after each level (specific to classic mode).
 */
extern int var_game_clearmulti;

/**
 * @brief Current elimination mode of the game (true: lines, false: SCCs).
 */
extern bool var_game_lines;

/**
 * @brief Flag indicating if nodelay mode is enabled (specific to classic mode).
 */
extern bool var_game_nodelay;

/**
 * @brief Flag indicating if easy mode is enabled (specific to classic mode).
 */
extern bool var_game_easy;

/**
 * @brief Flag indicating if wrap gems mode is enabled (specific to classic mode).
 */
extern bool var_game_wrap;

/**
 * @brief Flag indicating if bombs mode is enabled (specific to classic mode).
 */
extern bool var_game_bombs;

/**
 * @brief Flag indicating if gem upgrades are enabled (relevant in both modes).
 */
extern bool var_game_upgrades;

/**
 * @brief Refill mode used in puzzle mode (0: stores eliminated gems, 1: predetermined queue of rocks and bombs, 2: No refill).
 */
extern short var_game_refill_mode;

/**
 * @brief Flag indicating if balanced generation is enabled (specific to puzzle mode).
 */
extern bool var_game_balanced;

/**
 * @brief Initial size of the storage queue (only relevant in puzzle mode when refill_mode is 1).
 */
extern int var_game_storage_initial;

/**
 * @brief Refill mode used in puzzle mode (0: stores eliminated gems, 1: predetermined queue of rocks and bombs, 2: No refill) (test mode).
 */
extern short var_test_game_refill_mode;

/**
 * @brief Flag indicating if balanced generation is enabled (specific to puzzle mode) (test mode).
 */
extern bool var_test_game_balanced;

/**
 * @brief Initial size of the storage queue (only relevant in puzzle mode when refill_mode is 1) (test mode).
 */
extern int var_test_game_storage_initial;

/**
 * @brief Solver mode.
 */
extern solver var_game_solver_mode;

/**
 * @brief Flag indicating if the solver is currently implementing a solution.
 */
extern bool var_game_solver;

/**
 * @brief Path of the solution found by the solver.
 */
extern solver_move *var_game_solver_path;

/**
 * @brief Number of moves in the solution found by the solver.
 */
extern int var_game_solver_nb_moves;

/**
 * @brief Number of configurations explored by the solver.
 */
extern int var_game_solver_nb_configs;

/**
 * @brief Result of the solver (succesful or not).
 */
extern solver_result_type var_game_solver_result;

/**
 * @brief IA setting in the classic mode menu.
 */
extern bool var_game_ia_menu;

/**
 * @brief IA activation in the current game.
 */
extern bool var_game_ia;

/**
 * @brief IA type selected in the classic mode menu.
 */
extern ia_classic var_game_ia_choice_menu;

/**
 * @brief IA type in the current game.
 */
extern ia_classic var_game_ia_choice;

/**
 * @brief Countdown timer for the IA (in milliseconds) before performin another move.
 */
extern int var_ia_countdown;

/**
 * @brief Gravity mode (0: cannot change, 1: rotate clockwise, 2: free) in the puzzle setup menu.
 */
extern short var_gravity_mode;

/**
 * @brief The current game.
 *
 * @details
 * This variable is used to store the current game being played.
 */
extern game *var_game;

/**
 * @brief Backup of the current game.
 *
 * @details
 * This variable is used to store a backup of the current game being played (used in puzzle mode to restore the game after solving).
 */
extern game *var_game_backup;

/**
 * @brief Score table used to display high scores.
 */
extern score_table *var_score_table;

/**
 * @brief
 * Release of the score table when not needed anymore.
 */
void var_release_score_table(void);

/**
 * @brief Enumeration of the different modes for the GUI.
 */
typedef enum
{
    GUIMODE_MENUS,   //!< Menu window.
    GUIMODE_CLASSIC, //!< Classic game window.
    GUIMODE_PUZZLE,  //!< Puzzle game window.
    GUIMODE_TEST,    //!< Test game window.
    GUIMODE_SIZE,
} gui_mode;

/**
 * @brief
 * Global variable that stores the current GUI mode.
 */
extern gui_mode var_gui_mode;

/**
 * @brief Enumeration of the different menus.
 */
typedef enum
{
    MENU_MAIN,
    MENU_CLASSIC,
    MENU_PUZZLE,
    MENU_SIZE,
} gui_menu;

/**
 * @brief
 * Global variable that stores the current menu.
 */
extern gui_menu var_current_menu;

/**
 * @brief Enumeration of the modes for the classic screen.
 */
typedef enum
{
    CLASSIC_GAMEPLAY,
    CLASSIC_MENU,
    CLASSIC_ENDLEVEL,
    CLASSIC_LOST,
    CLASSIC_SIZE,
} gui_classic;

/**
 * @brief
 * Global variable that stores the current classic mode.
 */
extern gui_classic var_current_classic_mode;

/**
 * @brief Enumeration of the modes for the puzzle screen.
 */
typedef enum
{
    PUZZLE_GAMEPLAY,
    PUZZLE_MENU,
    PUZZLE_SOLVER,
    PUZZLE_SUMMARY,
    PUZZLE_SIZE,
} gui_puzzle;

/**
 * @brief
 * Global variable that stores the current puzzle mode.
 */
extern gui_puzzle var_current_puzzle_mode;

/**
 * @brief
 * Indicates whether the high scores screen is currently displayed in the main menu.
 */
extern bool var_high_scores;

/*****************************/
/*+ Handling animation mode +*/
/*****************************/

/**
 * @brief Current animation type.
 *
 * @details
 * This variable is used to determine which animation is currently being displayed.
 */
extern anim_type var_anim_type;

/**
 * @brief
 * Maximal number of cells that an object will traverse during a movement animation.
 *
 * @details
 * Initialized by var_init_movement() from the game board.
 */
extern int var_cell_movement;

/**
 * @brief
 * Number of cells that have been traversed in the current movement animation.
 *
 * @details
 * Initialized (to zero) by var_init_movement().
 */
extern int var_cell_movement_done;

/**
 * @brief
 * Initializes the idle mode.
 *
 * @details
 * This idle mode is used when the game is not performing any animation.
 */
void var_init_idle(void);

/**
 * @brief
 * Initializes the movement animation.
 *
 * @details
 * This function computes the maximum cell movement distance and initializes the
 * movement animation in the GUI.
 */
void var_init_movement(void);

/**
 * @brief
 * Initializes the gem kill animation.
 *
 * @details
 * This function initializes the kill animation in the GUI and restricts input
 * during the kill animation.
 */
void var_init_kill(void);

/**
 * @brief
 * Initializes the hint animation.
 *
 * @details
 * This function initializes the hint animation in the GUI and restricts input
 * during the hint animation.
 */
void var_init_hint(void);

/***************************/
/*+ Continuous animations +*/
/***************************/

#define NBFRAMES_MOVE 20
#define NBFRAMES_ROTATION 72

/**
 * @brief
 * Retrieves the current frame of the flamed gem animation.
 *
 * @return
 * The current frame of the flamed gem animation.
 */
short var_flame_get_frame(void);

/**
 * @brief
 * Retrieves the current frame of the starred gem animation.
 *
 * @return
 * The current frame of the starred gem animation.
 */
short var_star_get_frame(void);

/**
 * @brief
 * Retrieves the current frame of the star effect on starred gem animation.
 *
 * @return
 * The current frame of the star effect on starred gem animation.
 */
short var_effect_star_get_frame(void);

/**
 * @brief
 * Retrieves the current frame of the spinning gem animation.
 *
 * @return
 * The current frame of the spinning gem animation.
 */
short var_spin_get_frame(void);

/**
 * @brief
 * Retrieves the current frame of the hypercube animation.
 *
 * @return
 * The current frame of the hypercube animation.
 */
short var_hypercube_get_frame(void);

/**
 * @brief
 * Retrieves the current frame of the wrapped gem animation.
 *
 * @return
 * The current frame of the wrapped gem animation.
 */
short var_wrap_get_frame(void);

/**
 * @brief
 * Increments all the continuous animation frame counters.
 */
void var_increment_counters(void);

/**
 * @brief
 * Resets all the continuous animation frame counters.
 */
void var_reset_counters(void);

/**********************/
/*+ Timed animations +*/
/**********************/

/**
 * @brief
 * Retrieves the current frame of the move animation.
 *
 * @return
 * The current frame of the move animation.
 */
short var_move_get_frame(void);

/**
 * @brief
 * Increments the move animation frame counter.
 */
void var_move_increment(void);

/**
 * @brief
 * Resets the move animation frame counter.
 */
void var_move_reset(void);

/**
 * @brief
 * Retrieves the current frame of the gem kill animation.
 *
 * @return
 * The current frame of the gem kill animation.
 */
short var_kill_get_frame(void);

/**
 * @brief
 * Increments the gem kill animation frame counter.
 */
void var_kill_increment(void);

/**
 * @brief
 * Resets the gem kill animation frame counter.
 */
void var_kill_reset(void);

/**
 * @brief
 * Retrieves the current frame of the hint animation.
 *
 * @return
 * The current frame of the hint animation.
 */
short var_hint_get_frame(void);

/**
 * @brief
 * Increments the hint animation frame counter.
 */
void var_hint_increment(void);

/**
 * @brief
 * Resets the hint animation frame counter.
 */
void var_hint_reset(void);

/******* Scores */

extern int var_scores_page;
extern int var_scores_max_page;
extern int var_scores_per_page;

#endif
