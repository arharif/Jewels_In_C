#include "gui_controller.h"
#include "game.h"
#include "game_ias.h"
#include "game_save.h"
#include "game_solver.h"
#include "gui_menus.h"
#include "project_parameters.h"

/***********************/
/*+ General functions +*/
/***********************/

bool ctl_valid_coordinates(game *g, int i, int j) {
    return (g && i >= 0 && i < board_get_width(g->b) && j >= 0 &&
            j < board_get_height(g->b));
}

int ctl_valid_border_coordinates(game *g, int i, int j) {
    if (!g) {
        return INT_MAX;
    }
    switch (board_get_gravity(g->b)) {
    case UP:
        if (j == board_get_height(g->b) && i >= 0 &&
            i < board_get_width(g->b)) {
            return i;
        }
        break;
    case DOWN:
        if (j == -1 && i >= 0 && i < board_get_width(g->b)) {
            return i;
        }
        break;
    case LEFT:
        if (i == board_get_width(g->b) && j >= 0 &&
            j < board_get_height(g->b)) {
            return j;
        }
        break;
    case RIGHT:
        if (i == -1 && j >= 0 && j < board_get_height(g->b)) {
            return j;
        }
        break;
    default:
        break;
    }
    return INT_MAX;
}

bool ctl_check_game(game *g, bool messages) {
    if (!g) {
        if (messages) {
            fprintf(stderr, "Error: Game structure is invalid.\n");
        }
        return false;
    }

    if (!g->b) {
        if (messages) {
            fprintf(stderr, "Error: Game board is invalid.\n");
        }
        return false;
    }

    if (g->gravity_control < 0 || g->gravity_control > 2) {
        if (messages) {
            fprintf(stderr, "Error: Invalid gravity control mode.\n");
        }
        return false;
    }

    if (board_get_width(g->b) <= 4 || board_get_height(g->b) <= 4) {
        if (messages) {
            fprintf(stderr, "Error: Invalid game board dimensions.\n");
        }
        return false;
    }

    if (g->gravity_control < 0 || g->gravity_control > 2) {
        if (messages) {
            fprintf(stderr, "Error: Invalid gravity control mode.\n");
        }
        return false;
    }

    if (g->score == NULL) {
        if (messages) {
            fprintf(stderr, "Error: Score block is not initialized.\n");
        }
        return false;
    }

    // Check puzzle mode case.
    if (g->puzzle) {
        if (g->refill_mode < 0 || g->refill_mode > 2) {
            if (messages) {
                fprintf(stderr, "Error: Invalid refill mode in puzzle game.\n");
            }
            return false;
        }

        if (g->refill_mode != 2 && board_refill_size(g->b) < 0) {
            if (messages) {
                fprintf(stderr, "Error: Refill mode enabled but storage is not "
                                "initialized.\n");
            }
            return false;
        }

        if (g->refill_mode == 2 && board_refill_size(g->b) >= 0) {
            if (messages) {
                fprintf(stderr, "Error: Refill mode disabled but storage is "
                                "initialized.\n");
            }
            return false;
        }
    } // Check classic mode case.
    else {
        if (g->refill_mode != 2 || board_refill_size(g->b) >= 0) {
            if (messages) {
                fprintf(stderr, "Error: Refill should be disabled and storage "
                                "should not be initialized in classic game.\n");
            }
            return false;
        }
    }
    return true;
}

void ctl_get_params(game *g) {
    if (!g) {
        fprintf(stderr, "ctl_get_params: Invalid game structure.\n");
        return;
    }
    var_game_hsize       = board_get_width(g->b);
    var_game_vsize       = board_get_height(g->b);
    var_game_lines       = g->lines;
    var_game_nodelay     = g->nodelay;
    var_game_upgrades    = g->upgrades;
    var_game_easy        = g->easy;
    var_game_wrap        = g->gravity_gems;
    var_game_bombs       = g->bombs;
    var_game_upgrades    = g->upgrades;
    var_game_refill_mode = g->refill_mode;
    var_gravity_mode     = g->gravity_control;
}

void ctl_kill_game(void) {
    if (var_game) {
        game_release(var_game);
    }
    if (var_game_backup) {
        game_release(var_game_backup);
    }
    var_game        = NULL;
    var_game_backup = NULL;
    ctl_release_storage_buttons();
}

bool ctl_fillbuttons_used                 = false;
bool *ctl_fillbuttons[SIZE_DIRECTION]     = {NULL, NULL, NULL, NULL};
int ctl_fillbuttons_count[SIZE_DIRECTION] = {0, 0, 0, 0};

void ctl_init_storage_buttons(int w, int h) {
    ctl_fillbuttons_count[UP]    = w;
    ctl_fillbuttons_count[DOWN]  = w;
    ctl_fillbuttons_count[LEFT]  = h;
    ctl_fillbuttons_count[RIGHT] = h;
    CREATE_ZERO_MEMORY(ctl_fillbuttons[UP], w);
    CREATE_ZERO_MEMORY(ctl_fillbuttons[DOWN], w);
    CREATE_ZERO_MEMORY(ctl_fillbuttons[LEFT], h);
    CREATE_ZERO_MEMORY(ctl_fillbuttons[RIGHT], h);
    ctl_fillbuttons_used = true;
}

void ctl_release_storage_buttons(void) {
    free(ctl_fillbuttons[UP]);
    free(ctl_fillbuttons[DOWN]);
    free(ctl_fillbuttons[LEFT]);
    free(ctl_fillbuttons[RIGHT]);
    ctl_fillbuttons[UP]    = NULL;
    ctl_fillbuttons[DOWN]  = NULL;
    ctl_fillbuttons[LEFT]  = NULL;
    ctl_fillbuttons[RIGHT] = NULL;
    ctl_fillbuttons_used   = false;
}

void ctl_update_game_timer(void) {
    if (var_game && var_game->score) {
        var_game->score->scores[SC_TIME] =
            get_time_counter() / 1000; // Save time in seconds
    }
}

void ctl_keyboard_general_menu(SDL_KeyCode key) {
    switch (key) {
    case SDLK_UP:
        var_selected_button = var_buttons[var_selected_button].up;
        return;
        break;
    case SDLK_DOWN:
        var_selected_button = var_buttons[var_selected_button].down;
        return;
        break;
    case SDLK_LEFT:
        var_selected_button = var_buttons[var_selected_button].left;
        return;
        break;
    case SDLK_RIGHT:
        var_selected_button = var_buttons[var_selected_button].right;
        return;
        break;
    case SDLK_RETURN:
        var_push_button(var_selected_button);
        return;
        break;

    default:
        break;
    }
}

void ctl_handle_anim_signals(void) {
    switch (game_anim_signal) {
    case ANIM_IDLE:
        var_init_idle();
        break;
    case ANIM_MOVE_BADSWAP:
    case ANIM_MOVE_GOODSWAP:
    case ANIM_MOVE_ROLLBACK:
    case ANIM_MOVE_FALL:
        var_init_movement();
        break;
    case ANIM_KILL:
        var_init_kill();
        break;
    case ANIM_HINT:
        var_init_hint();
        break;
    default:
        break;
    }
    game_anim_signal = ANIM_SIZE; // Resetting the signal
}

void ctl_handle_end_signals(void) {
    // These signals are ignored in menu and test modes
    if (var_gui_mode == GUIMODE_MENUS || var_gui_mode == GUIMODE_TEST) {
        game_end_signal = SIGNAL_SIZE; // Resetting the signal
        return;
    }

    switch (game_end_signal) {
    case SIGNAL_END_LEVEL:
        if (var_gui_mode == GUIMODE_CLASSIC) {
            ctl_end_classic(false);
        }
        break;
    case SIGNAL_END_CLGAME:
        if (var_gui_mode == GUIMODE_CLASSIC) {
            ctl_end_classic(true);
        }
        break;
    case SIGNAL_END_PUZZLE:
        if (var_gui_mode == GUIMODE_PUZZLE) {
            ctl_end_puzzle();
        }
        break;
    default:
        break;
    }
    game_end_signal = SIGNAL_SIZE; // Resetting the signal
}

/********************************/
/*+ Management of score screen +*/
/********************************/

bool ctl_mouse_score_screen(int x, int y) {
    for (int i = BT_SCGLOBAL; i <= BT_SCTIME; i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }
    if (var_scores_page > 0) {
        if (mouse_capture_button(x, y, BT_PREVSCORE)) {
            return true;
        }
    }
    if (var_scores_page < var_scores_max_page - 1) {
        if (mouse_capture_button(x, y, BT_NEXTSCORE)) {
            return true;
        }
    }
    return false;
}

void ctl_button_push_score(void) {
    switch (var_pushed_button) {
    case BT_SCGLOBAL:
        sort_score_table(var_score_table, SC_GLOBAL);
        break;
    case BT_SCLEVEL:
        sort_score_table(var_score_table, SC_LEVEL);
        break;
    case BT_SCSWAPS:
        sort_score_table(var_score_table, SC_SWAPS);
        break;
    case BT_SCCONSUMED:
        sort_score_table(var_score_table, SC_CONSUMED);
        break;
    case BT_SCFLAMED:
        sort_score_table(var_score_table, SC_FLAMED);
        break;
    case BT_SCSTARRED:
        sort_score_table(var_score_table, SC_STARRED);
        break;
    case BT_SCHYPER:
        sort_score_table(var_score_table, SC_HYPERCUBES);
        break;
    case BT_SCGRAVITY:
        sort_score_table(var_score_table, SC_GRAVITY);
        break;
    case BT_SCTIME:
        sort_score_table(var_score_table, SC_TIME);
        break;
    case BT_PREVSCORE:
        if (var_scores_page > 0) {
            var_scores_page--;
        }
        break;
    case BT_NEXTSCORE:
        if (var_scores_page < var_scores_max_page - 1) {
            var_scores_page++;
        }
        break;
    default:
        break;
    }
}

/************************/
/*+ Management of zoom +*/
/************************/

bool ctl_keyboard_zoom(SDL_KeyCode key) {
    if (flags[NOSCROLL]) {
        return false;
    }
    switch (key) {
    case SDLK_KP_PLUS:
    case SDLK_EQUALS:
    case SDLK_PLUS:
        increase_zoom();
        return true;
        break;
    case SDLK_KP_MINUS:
    case SDLK_6:
    case SDLK_MINUS:
    case SDLK_1:
        decrease_zoom();
        return true;
        break;
    default:
        break;
    }
    return false;
}

/*********************************/
/*+ Management of gravity wheel +*/
/*********************************/

bool ctl_keyboard_gravitywheel(SDL_KeyCode key) {
    if (!var_game || flags[NOGAMEINPUT] || flags[NOPLAYER] ||
        var_game->gravity_control == 0) {
        return false;
    }

    if (var_game->gravity_control == 1 && key == SDLK_g) {
        var_push_button(BT_GRAVITY);
        return true;
    } else if (var_game->gravity_control == 2) {
        switch (key) {
        case SDLK_UP:
            var_push_button(BT_GUP);
            return true;
            break;
        case SDLK_RIGHT:
            var_push_button(BT_GRIGHT);
            return true;
            break;
        case SDLK_DOWN:
            var_push_button(BT_GDOWN);
            return true;
            break;
        case SDLK_LEFT:
            var_push_button(BT_GLEFT);
            return true;
            break;
        case SDLK_g:
            var_push_button(BT_GRAVITY);
            return true;
            break;
        default:
            break;
        }
    }
    return false;
}

bool ctl_mouse_gravitywheel(int x, int y) {
    if (!var_game || flags[NOGAMEINPUT] || flags[NOPLAYER] ||
        var_game->gravity_control == 0) {
        return false;
    }

    if (var_game->gravity_control == 1) {
        return mouse_capture_button(x, y, BT_GRAVITY);
    } else if (var_game->gravity_control == 2) {
        return mouse_capture_button(x, y, BT_GRAVITY) ||
               mouse_capture_button(x, y, BT_GUP) ||
               mouse_capture_button(x, y, BT_GRIGHT) ||
               mouse_capture_button(x, y, BT_GDOWN) ||
               mouse_capture_button(x, y, BT_GLEFT);
    }
    return false;
}

void ctl_button_push_gravitywheel(void) {
    if (!var_game || flags[NOGAMEINPUT] || flags[NOPLAYER] ||
        var_game->gravity_control == 0) {
        return;
    }

    switch (var_pushed_button) {
    case BT_GRAVITY:
        game_cycle_gravity(var_game);
        break;
    case BT_GUP:
        game_set_gravity(var_game, UP);
        break;
    case BT_GRIGHT:
        game_set_gravity(var_game, RIGHT);
        break;
    case BT_GDOWN:
        game_set_gravity(var_game, DOWN);
        break;
    case BT_GLEFT:
        game_set_gravity(var_game, LEFT);
        break;
    default:
        break;
    }
}

/*****************************/
/*+ Management of main menu +*/
/*****************************/

/**
 * @brief Initializes the demo game.
 */
void ctl_init_demo_game(void) {
    reset_flags();
    ctl_kill_game();

    // Initialization of the zoom level for the game.
    init_zoom_game();

    // Creation of the game itself.
    int h = var_screen_height / var_tile_size - 2;
    int w = var_screen_width / var_tile_size - 2;
    var_game =
        game_create_classic(w, h, true, -1, 1, true, true, true, false, false);
    if (!ctl_check_game(var_game, false)) {
        fprintf(stderr, "Error: Creation of the demo game failed.\n");
        game_release(var_game);
        var_game = NULL;
        return;
    }
    if (var_game->puzzle) {
        fprintf(stderr, "Error: Created demo game is not in classic mode.\n");
        game_release(var_game);
        var_game = NULL;
        return;
    }

    // Initialization of the GUI elements.
    var_init_idle();
    var_gui_mode = GUIMODE_MENUS;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b), var_screen_width);
    var_game_ia        = true;
    var_game_ia_choice = IA_RANDOM;
    set_flag(NOPLAYER);
    set_flag(NOSCROLL);
}

void ctl_keyboard_mainmenu(SDL_KeyCode key) {
    if (var_high_scores) {
        return;
    }
    ctl_keyboard_general_menu(key);
    return;
}

bool ctl_mouse_mainmenu(int x, int y) {
    if (flags[PUSHED]) {
        return false;
    } else if (var_high_scores) {
        return mouse_capture_button(x, y, BT_SCOREBACK) ||
               ctl_mouse_score_screen(x, y);
    }
    for (int i = BT_GAME; i <= BT_QUIT; i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }
    return false;
}

bool ctl_mouse_classic_setup(int x, int y) {
    if (flags[PUSHED]) {
        return false; // Ignore mouse clicks if a button is already pushed
    }

    for (int i = BT_SETWIDTH; i <= BT_SETMENU; i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }

    for (int i = BT_SETNODELAY; i <= (var_game_ia_menu ? BT_SETIAR : BT_SETCTR);
         i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }

    return false;
}

bool ctl_mouse_puzzle_setup(int x, int y) {
    if (flags[PUSHED]) {
        return false; // Ignore mouse clicks if a button is already pushed
    }

    for (int i = BT_SETWIDTH; i <= BT_SETMENU; i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }

    for (int i = BT_SETBALANCED; i <= BT_SETGRAVITYR; i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }

    if (var_game_refill_mode != 2 &&
        mouse_capture_button(x, y, BT_SETINITIAL)) {
        return true;
    }

    return false;
}

void ctl_enter_menu(bool initdemo) {
    var_high_scores = false;
    var_release_score_table();
    var_gui_mode        = GUIMODE_MENUS;
    var_current_menu    = MENU_MAIN;
    var_selected_button = BT_GAME;
    var_reset_buttons();

    // Initilization of the demo game.
    if (initdemo) {
        ctl_init_demo_game();
        set_flag(NOPLAYER);
        set_flag(NOSCROLL);
    }
}

void ctl_enter_setupclassic(void) {
    var_gui_mode        = GUIMODE_MENUS;
    var_current_menu    = MENU_CLASSIC;
    var_selected_button = BT_SETSTART;
    var_reset_buttons();
}

void ctl_enter_setuppuzzle(void) {
    var_gui_mode        = GUIMODE_MENUS;
    var_current_menu    = MENU_PUZZLE;
    var_selected_button = BT_SETSTART;
    var_reset_buttons();
}

void ctl_enter_highscores(void) {
    // Loads the score table (with the current player's score if any)
    var_release_score_table();
    var_score_table = load_score_table("./scores/scores", NULL);

    if (var_score_table) {
        // Initializes the score table parameters
        var_scores_page     = 0;
        var_scores_max_page = var_score_table->size / var_scores_per_page;
        if (var_score_table->size % var_scores_per_page > 0) {
            var_scores_max_page++;
        }
        sort_score_table(var_score_table, SC_GLOBAL);
    }
    var_high_scores = true;
    var_reset_buttons();
}

void ctl_button_push_mainmenu(void) {
    if (!var_high_scores) {
        switch (var_pushed_button) {
        case BT_GAME:
            ctl_enter_setupclassic();
            break;
        case BT_PUZZLE:
            ctl_enter_setuppuzzle();
            break;
        case BT_LOAD:
            init_text_buffer(ED_LOAD);
            break;
        case BT_TEST:
            ctl_test_initial(TEST_PUZZLE);
            break;
        case BT_SCORESCREEN:
            ctl_enter_highscores();
            break;
        case BT_QUIT:
            set_flag(QUIT);
            break;
        default:
            break;
        }
    } else {
        ctl_button_push_score();
        if (var_pushed_button == BT_SCOREBACK) {
            var_release_score_table();
            ctl_enter_menu(false);
        }
    }
}

void ctl_button_push_setupclassic(void) {
    switch (var_pushed_button) {
    case BT_SETHEIGHT:
        init_text_buffer(ED_HEIGHT);
        break;
    case BT_SETWIDTH:
        init_text_buffer(ED_WIDTH);
        break;
    case BT_SETSELECMODEL:
        var_game_lines = true;
        break;
    case BT_SETSELECMODER:
        var_game_lines = false;
        break;
    case BT_SETNODELAY:
        var_game_nodelay = !var_game_nodelay;
        break;
    case BT_SETEASY:
        var_game_easy = !var_game_easy;
        break;
    case BT_SETWRAP:
        var_game_wrap = !var_game_wrap;
        break;
    case BT_SETBOMB:
        var_game_bombs = !var_game_bombs;
        break;
    case BT_SETCLEAR:
        init_text_buffer(ED_CLEARSCORE);
        break;
    case BT_SETCLEARMULT:
        init_text_buffer(ED_CLEARMULTI);
        break;
    case BT_SETCTL:
        var_game_ia_menu = false;
        break;
    case BT_SETCTR:
        var_game_ia_menu = true;
        break;
    case BT_SETIAL:
        var_game_ia_choice_menu =
            (var_game_ia_choice_menu + IA_CLASSIC_SIZE - 1) % IA_CLASSIC_SIZE;
        break;
    case BT_SETIAR:
        var_game_ia_choice_menu =
            (var_game_ia_choice_menu + 1) % IA_CLASSIC_SIZE;
        break;
    case BT_SETMENU:
        ctl_enter_menu(false);
        break;
    case BT_SETSTART:
        ctl_init_classic();
        break;
    default:
        break;
    }
}

void ctl_button_push_setuppuzzle(void) {
    switch (var_pushed_button) {
    case BT_SETHEIGHT:
        init_text_buffer(ED_HEIGHT);
        break;
    case BT_SETWIDTH:
        init_text_buffer(ED_WIDTH);
        break;
    case BT_SETINITIAL:
        init_text_buffer(ED_STORAGEINI);
        break;
    case BT_SETSELECMODEL:
        var_game_lines = true;
        break;
    case BT_SETSELECMODER:
        var_game_lines = false;
        break;
    case BT_SETNODELAY:
        var_game_nodelay = !var_game_nodelay;
        break;
    case BT_SETBALANCED:
        var_game_balanced = !var_game_balanced;
        break;
    case BT_SETUPGRADES:
        var_game_upgrades = !var_game_upgrades;
        break;
    case BT_SETSTOREREFILL:
        var_game_refill_mode = 0;
        break;
    case BT_SETSTOREBOMB:
        var_game_refill_mode = 1;
        break;
    case BT_SETSTORENONE:
        var_game_refill_mode = 2;
        break;
    case BT_SETGRAVITYL:
        var_gravity_mode = 0;
        break;
    case BT_SETGRAVITYC:
        var_gravity_mode = 1;
        break;
    case BT_SETGRAVITYR:
        var_gravity_mode = 2;
        break;
    case BT_SETMENU:
        ctl_enter_menu(false);
        break;
    case BT_SETSTART:
        ctl_init_puzzle();
        break;
    default:
        break;
    }
}

/********************************/
/*+ Management of classic mode +*/
/********************************/

void ctl_init_classic(void) {
    // Resets all flags.
    reset_flags();

    // Killing all eventual previous games.
    ctl_kill_game();
    // Initialization of the zoom level for the game.
    init_zoom_game();

    // Creation of the game itself
    var_game = game_create_classic(
        var_game_hsize, var_game_vsize, var_game_lines, var_game_clearscore,
        var_game_clearmulti, var_game_nodelay, var_game_upgrades, var_game_easy,
        var_game_wrap, var_game_bombs);
    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Creation of a classic game failed.\n");
        ctl_enter_menu(true);
        return;
    }
    if (var_game->puzzle) {
        fprintf(stderr, "Error: Created game is not in classic mode.\n");
        ctl_enter_menu(true);
        return;
    }

    // Setting the gui mode to gameplay.
    var_gui_mode             = GUIMODE_CLASSIC;
    var_current_classic_mode = CLASSIC_GAMEPLAY;

    // Initialization of the IA settings.
    if (var_game_ia_menu) {
        set_flag(NOPLAYER);
        var_game_ia        = true;
        var_game_ia_choice = var_game_ia_choice_menu;
    } else {
        remove_flag(NOPLAYER);
        var_game_ia = false;
    }

    // Initialization of the GUI elements.
    var_selected_button = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b), var_uiboxes[BX_GRAVITY].x);
    var_init_idle();
    init_time_counter();
}

void ctl_newlevel_classic(void) {
    var_release_score_table();

    // Advances to the next level.
    game_next_level(var_game);

    // Checking the game.
    if (!ctl_check_game(var_game, true)) {
        fprintf(
            stderr,
            "Error: Creation of a classic game for the next level failed.\n");
        ctl_enter_menu(true);
        return;
    }
    if (var_game->puzzle) {
        fprintf(stderr, "Error: Next game is not in classic mode.\n");
        ctl_enter_menu(true);
        return;
    }

    // Initialization of the GUI elements.
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b), var_uiboxes[BX_GRAVITY].x);
    var_init_idle();
    resume_time_counter();

    // Setting the flags
    var_current_classic_mode = CLASSIC_GAMEPLAY;
    remove_flag(INMENU);
    remove_flag(NOGAMEINPUT);
    remove_flag(NOSCROLL);
}

void ctl_end_classic(bool lost) {
    // Preventing game inputs
    set_flag(INMENU);
    set_flag(NOSCROLL);

    // Updates the time counter of the game
    pause_time_counter();
    var_game->score->scores[SC_TIME] = get_time_counter() / 1000;

    // Loads the score table (with the current player's score if any)
    var_release_score_table();
    var_score_table = load_score_table(
        "./scores/scores", var_game ? var_game->score->scores : NULL);

    // Initializes the score table parameters
    if (var_score_table) {
        var_scores_page     = 0;
        var_scores_max_page = var_score_table->size / var_scores_per_page;
        if (var_score_table->size % var_scores_per_page > 0) {
            var_scores_max_page++;
        }
        sort_score_table(var_score_table, SC_GLOBAL);
    }

    // Updates the GUI mode
    var_gui_mode             = GUIMODE_CLASSIC;
    var_current_classic_mode = lost ? CLASSIC_LOST : CLASSIC_ENDLEVEL;
}

void ctl_menu_classic(void) {
    // Preventing game inputs
    set_flag(INMENU);
    set_flag(NOSCROLL);

    // Updates the time counter of the game
    pause_time_counter();
    var_game->score->scores[SC_TIME] = get_time_counter() / 1000;

    var_gui_mode                     = GUIMODE_CLASSIC;
    var_current_classic_mode         = CLASSIC_MENU;
}

void ctl_gameplay_classic(void) {
    if (var_current_classic_mode == CLASSIC_LOST) {
        return;
    }
    remove_flag(INMENU);
    remove_flag(NOSCROLL);
    resume_time_counter();
    var_gui_mode             = GUIMODE_CLASSIC;
    var_current_classic_mode = CLASSIC_GAMEPLAY;
}

bool ctl_keyboard_classicmode(SDL_KeyCode key) {
    // Only process inputs in gameplay mode.
    if (var_current_classic_mode != CLASSIC_GAMEPLAY || flags[PUSHED]) {
        return false;
    }

    // First handle zoom keys (can be used anytime provided that the no scroll
    // flag is not set).
    if (ctl_keyboard_zoom(key)) {
        return true;
    }

    // When in no game input mode, only the escape key is processed.
    if (flags[NOGAMEINPUT] || flags[NOPLAYER]) {
        if (key == SDLK_ESCAPE) {
            var_push_button(BT_MENU);
            return true;
        }
        return false;
    }

    // Otherwise, process the keys normally.
    switch (key) {
    case SDLK_d:
        if (!var_game->nodelay) {
            var_push_button(BT_DETONATE);
            return true;
        }
        break;
    case SDLK_s:
        var_push_button(BT_SAVE);
        return true;
        break;
    case SDLK_h:
        var_push_button(BT_HINT);
        return true;
        break;
    case SDLK_ESCAPE:
        var_push_button(BT_MENU);
        return true;
        break;
    default:
        break;
    }
    return false;
}

bool ctl_mouse_classicmode(int x, int y) {
    if (flags[PUSHED]) {
        return false; // Ignore mouse clicks if a button is already pushed
    }

    if (var_current_classic_mode == CLASSIC_GAMEPLAY) {
        if (flags[NOGAMEINPUT] || flags[NOPLAYER]) {
            return mouse_capture_button(x, y, BT_MENU);
        } else {
            return mouse_capture_button(x, y, BT_SAVE) ||
                   mouse_capture_button(x, y, BT_HINT) ||
                   mouse_capture_button(x, y, BT_MENU) ||
                   (!var_game->nodelay &&
                    mouse_capture_button(x, y, BT_DETONATE));
        }
    }

    switch (var_current_classic_mode) {
    case CLASSIC_MENU:
        return mouse_capture_button(x, y, BT_CLASSICCONTINUE) ||
               mouse_capture_button(x, y, BT_CLASSICRESTART) ||
               mouse_capture_button(x, y, BT_CLASSICQUIT);
        break;
    case CLASSIC_ENDLEVEL:
        return mouse_capture_button(x, y, BT_SCORECONTINUE) ||
               mouse_capture_button(x, y, BT_SCORESAVE) ||
               mouse_capture_button(x, y, BT_SCOREMENU) ||
               ctl_mouse_score_screen(x, y);
    case CLASSIC_LOST:
        return mouse_capture_button(x, y, BT_SCORESAVE) ||
               mouse_capture_button(x, y, BT_SCOREMENU) ||
               ctl_mouse_score_screen(x, y);
    default:
        break;
    }
    return false;
}

void ctl_button_push_classic(void) {
    if (var_current_classic_mode == CLASSIC_GAMEPLAY) {
        switch (var_pushed_button) {
        case BT_SAVE:
            ctl_update_game_timer();
            init_text_buffer(ED_SAVE);
            break;
        case BT_HINT:
            game_handle_hint_request(var_game);
            break;
        case BT_MENU:
            ctl_menu_classic();
            break;
        case BT_DETONATE:
            if (var_game && !var_game->nodelay) {
                game_handle_detonation(var_game);
            }
        default:
            break;
        }
        return;
    } else if (var_current_classic_mode == CLASSIC_MENU) {
        switch (var_pushed_button) {
        case BT_CLASSICCONTINUE:
            ctl_gameplay_classic();
            break;
        case BT_CLASSICRESTART:
            ctl_init_classic();
            break;
        case BT_CLASSICQUIT:
            ctl_enter_menu(true);
            break;
        default:
            break;
        }
        return;
    } else if (var_current_classic_mode == CLASSIC_ENDLEVEL ||
               var_current_classic_mode == CLASSIC_LOST) {
        ctl_button_push_score();
        switch (var_pushed_button) {
        case BT_SCORECONTINUE:
            var_release_score_table();
            ctl_newlevel_classic();
            break;
        case BT_SCORESAVE:
            init_text_buffer(ED_NAME);
            break;
        case BT_SCOREMENU:
            var_release_score_table();
            ctl_enter_menu(true);
            break;
        default:
            break;
        }
        return;
    }
}
/*******************************/
/*+ Management of puzzle mode +*/
/*******************************/

void ctl_init_puzzle(void) {
    // Resets all flags.
    reset_flags();

    // Killing all eventual previous games.
    ctl_kill_game();
    // Initialization of the zoom level for the game.
    init_zoom_game();

    // Creation of the game itself
    var_game = game_create_puzzle(
        var_game_hsize, var_game_vsize, var_game_lines, var_game_upgrades,
        var_game_refill_mode, var_game_storage_initial, var_game_balanced,
        var_gravity_mode);
    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Creation of a puzzle game failed.\n");
        ctl_enter_menu(true);
        return;
    }
    if (!var_game->puzzle) {
        fprintf(stderr, "Error: Created game is not in puzzle mode.\n");
        ctl_enter_menu(true);
        return;
    }

    // Creation of the refill buttons if refill mode is activated.
    if (var_game_refill_mode != 2) {
        ctl_init_storage_buttons(board_get_width(var_game->b),
                                 board_get_height(var_game->b));
    }

    // Creates a clone of the game for restoration after solving.
    var_game_backup         = game_clone(var_game);

    // NO IA in puzzle mode.
    var_game_ia             = false;

    // Setting the gui mode to puzzle mode and gameplay.
    var_gui_mode            = GUIMODE_PUZZLE;
    var_current_puzzle_mode = PUZZLE_GAMEPLAY;

    // Initialization of the GUI elements.
    var_selected_button     = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b), var_uiboxes[BX_GRAVITY].x);
    var_init_idle();
    init_time_counter();
}

void ctl_restore_puzzle(void) {
    if (var_game_backup) {
        game *temp = var_game;
        var_game   = game_clone(var_game_backup);
        game_release(temp);
        var_gui_mode            = GUIMODE_PUZZLE;
        var_current_puzzle_mode = PUZZLE_GAMEPLAY;
        init_time_counter();
        reset_flags();
    }
}

void ctl_end_puzzle(void) {
    // Preventing game inputs
    set_flag(INMENU);
    set_flag(NOSCROLL);

    // Updates the time counter of the game
    pause_time_counter();
    var_game->score->scores[SC_TIME] = get_time_counter() / 1000;

    var_gui_mode                     = GUIMODE_PUZZLE;
    var_current_puzzle_mode          = PUZZLE_SUMMARY;
}

void ctl_menu_puzzle(void) {
    // Preventing game inputs
    set_flag(INMENU);
    set_flag(NOSCROLL);

    // Updates the time counter of the game
    pause_time_counter();
    var_game->score->scores[SC_TIME] = get_time_counter() / 1000;

    var_gui_mode                     = GUIMODE_PUZZLE;
    var_current_puzzle_mode          = PUZZLE_MENU;
}

void ctl_gameplay_puzzle(void) {
    if (var_current_puzzle_mode == PUZZLE_SUMMARY) {
        return;
    }
    remove_flag(INMENU);
    remove_flag(NOSCROLL);
    resume_time_counter();
    var_gui_mode            = GUIMODE_PUZZLE;
    var_current_puzzle_mode = PUZZLE_GAMEPLAY;
}

void ctl_solver_puzzle(solver mode) {
    if (!var_game) {
        return;
    }
    if (mode < 0 || mode > SOLVER_ASTAR) {
        fprintf(stderr, "ctl_start_solver: Invalid solver mode %d\n", mode);
        exit(EXIT_FAILURE);
    }

    pause_time_counter();
    var_game->score->scores[SC_TIME] = get_time_counter() / 1000;

    var_game_solver_path             = solve_puzzle(
        var_game, mode, SOLVER_MAX_CONFIGS, &var_game_solver_result,
        &var_game_solver_nb_moves, &var_game_solver_nb_configs);

    // Preventing game inputs
    set_flag(INMENU);
    set_flag(NOSCROLL);

    // Updates the time counter of the game

    var_gui_mode            = GUIMODE_PUZZLE;
    var_current_puzzle_mode = PUZZLE_SOLVER;
}

void ctl_implement_solution_puzzle(void) {
    if (var_game_solver_path == NULL ||
        var_game_solver_result != SOLVER_SOLVED) {
        return;
    }
    var_game_solver = true;
    ctl_gameplay_puzzle();
    set_flag(NOPLAYER);
}

void ctl_button_push_puzzle(void) {
    if (var_current_puzzle_mode == PUZZLE_GAMEPLAY) {
        ctl_button_push_gravitywheel();
        switch (var_pushed_button) {
        case BT_SAVE:
            ctl_update_game_timer();
            init_text_buffer(ED_SAVE);
            break;
        case BT_SOLCL:
            var_game_solver_mode = (var_game_solver_mode + 5) % 3;
            break;
        case BT_SOLCR:
            var_game_solver_mode = (var_game_solver_mode + 1) % 3;
            break;
        case BT_SOLVER:
            ctl_solver_puzzle(var_game_solver_mode);
            break;
        case BT_MENU:
            ctl_menu_puzzle();
            break;
        default:
            break;
        }
        return;
    }

    switch (var_pushed_button) {
    case BT_PUZZLEQUIT:
        ctl_enter_menu(true);
        break;
    case BT_PUZZLERETRY:
        ctl_restore_puzzle();
        break;
    case BT_PUZZLEREGEN:
        ctl_init_puzzle();
        break;
    case BT_PUZZLEBACKMENU:
        ctl_gameplay_puzzle();
        break;
    case BT_PUZZLEBACKSOLVER:
        while (var_game_solver_path) {
            solver_move *next = var_game_solver_path->next;
            free(var_game_solver_path);
            var_game_solver_path = next;
        }
        ctl_gameplay_puzzle();
        break;
    case BT_PUZZLEIMPLEMENT:
        ctl_implement_solution_puzzle();
        break;
    default:
        break;
    }
}

bool ctl_keyboard_puzzle(SDL_KeyCode key) {
    // Only process inputs in gameplay mode.
    if (var_current_puzzle_mode != PUZZLE_GAMEPLAY || flags[PUSHED]) {
        return false;
    }

    // First handle zoom keys (can be used anytime provided that the no scroll
    // flag is not set).
    if (ctl_keyboard_zoom(key)) {
        return true;
    }

    // When in no game input mode, only the escape key is processed.
    if (flags[NOGAMEINPUT] || flags[NOPLAYER]) {
        if (key == SDLK_ESCAPE) {
            var_push_button(BT_MENU);
            return true;
        }
        return false;
    }

    // Otherwise, process the keys normally.
    switch (key) {
    case SDLK_l:
        var_push_button(BT_SOLVER);
        return true;
        break;
    case SDLK_s:
        var_push_button(BT_SAVE);
        return true;
        break;
    case SDLK_ESCAPE:
        var_push_button(BT_MENU);
        return true;
        break;
    default:
        break;
    }
    // Only the gravity wheel keys remain to be processed.
    return ctl_keyboard_gravitywheel(key);
}

bool ctl_mouse_puzzle(int x, int y) {
    if (flags[PUSHED]) {
        return false; // Ignore mouse clicks if a button is already pushed
    }

    // Mouse in gameplay mode.
    if (var_current_puzzle_mode == PUZZLE_GAMEPLAY) {
        // When in no game input mode, only the menu button is processed.
        if (flags[NOGAMEINPUT] || flags[NOPLAYER]) {
            return mouse_capture_button(x, y, BT_MENU);
        }
        // Otherwise, check other HUD buttons.
        return mouse_capture_button(x, y, BT_SAVE) ||
               mouse_capture_button(x, y, BT_SOLVER) ||
               mouse_capture_button(x, y, BT_SOLCL) ||
               mouse_capture_button(x, y, BT_SOLCR) ||
               mouse_capture_button(x, y, BT_MENU) ||
               ctl_mouse_gravitywheel(x, y);
    }

    // We turn to mouse in other puzzle modes.
    switch (var_current_puzzle_mode) {
    case PUZZLE_SOLVER:
        return mouse_capture_button(x, y, BT_PUZZLEBACKSOLVER) ||
               (var_game_solver_result == SOLVER_SOLVED &&
                mouse_capture_button(x, y, BT_PUZZLEIMPLEMENT));
        break;
    case PUZZLE_MENU:
        return mouse_capture_button(x, y, BT_PUZZLEQUIT) ||
               mouse_capture_button(x, y, BT_PUZZLERETRY) ||
               mouse_capture_button(x, y, BT_PUZZLEREGEN) ||
               mouse_capture_button(x, y, BT_PUZZLEBACKMENU);
        break;
    case PUZZLE_SUMMARY:
        return mouse_capture_button(x, y, BT_PUZZLEQUIT) ||
               mouse_capture_button(x, y, BT_PUZZLERETRY) ||
               mouse_capture_button(x, y, BT_PUZZLEREGEN);
        break;
    default:
        break;
    }
    return false;
}

/*****************************/
/*+ Management of test mode +*/
/*****************************/

void ctl_test_initial(bool puzzlemode) {
    reset_flags();
    ctl_kill_game();
    // Reinitializing the zoom level.
    init_zoom_game();

    if (puzzlemode) {
        var_game = game_create_puzzle(
            var_test_game_vsize, var_test_game_hsize, TEST_LINES, TEST_UPGRADES,
            var_test_game_refill_mode, var_test_game_storage_initial,
            var_test_game_balanced, 2);
    } else {
        var_game = game_create_classic(
            var_test_game_hsize, var_test_game_vsize, TEST_LINES, -1, 1,
            TEST_NODELAY, TEST_UPGRADES, TEST_EASY, TEST_WRAP, TEST_BOMBS);
    }

    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Creation of a test game failed.\n");
        ctl_enter_menu(true);
        return;
    }

    // Creation of the refill buttons if refill mode is activated.
    if (var_game->puzzle && var_game->refill_mode != 2) {
        ctl_init_storage_buttons(board_get_width(var_game->b),
                                 board_get_height(var_game->b));
    }

    // Enforces that the gravity control is free.
    var_game->gravity_control = 2;

    // Switch to test mode.
    var_gui_mode              = GUIMODE_TEST;

    // Initialization of the GUI elements.
    var_init_idle();
    var_selected_button = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b),
                     var_uiboxes[BX_TESTREGEN].x);

    init_time_counter();
    var_game_ia = false;
}

void ctl_test_puzzle(void) {
    // If the old game was not in puzzle mode, just reinitialize everything.
    if (!var_game || !var_game->puzzle) {
        ctl_test_initial(true);
        return;
    }

    reset_flags();
    // Reinitializing the zoom level.
    init_zoom_game();

    // The future new game (we pick the modifiable parameters from the old one).
    game *g = game_create_puzzle(
        var_test_game_hsize, var_test_game_vsize, var_game->lines,
        var_game->upgrades, var_test_game_refill_mode,
        var_test_game_storage_initial, var_test_game_balanced, 2);
    ctl_kill_game();
    var_game = g;

    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Creation of the new test game failed.\n");
        ctl_enter_menu(true);
        return;
    }
    if (!var_game->puzzle) {
        fprintf(stderr, "Error: Created test game is not in puzzle mode.\n");
        ctl_enter_menu(true);
        return;
    }
    var_game->gravity_control = 2;
    if (var_game_refill_mode != 2) {
        ctl_init_storage_buttons(board_get_width(var_game->b),
                                 board_get_height(var_game->b));
    }

    // Switch to test mode.
    var_gui_mode = GUIMODE_TEST;

    // Initialization of the GUI elements.
    var_init_idle();
    var_selected_button = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b),
                     var_uiboxes[BX_TESTREGEN].x);
    var_game_ia = false;
    init_time_counter();
}

void ctl_test_classic(void) {
    // If the old game was in puzzle mode, just reinitialize everything.
    if (!var_game || var_game->puzzle) {
        ctl_test_initial(false);
        return;
    }

    reset_flags();
    // Reinitializing the zoom level.
    init_zoom_game();

    // The future new game (we pick the modifiable parameters from the old one).
    game *g = game_create_classic(var_test_game_hsize, var_test_game_vsize,
                                  var_game->lines, -1, 1, var_game->nodelay,
                                  var_game->upgrades, var_game->easy,
                                  var_game->gravity_gems, var_game->bombs);
    ctl_kill_game();
    var_game = g;

    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Creation of the new test game failed.\n");
        ctl_enter_menu(true);
        return;
    }
    if (var_game->puzzle) {
        fprintf(stderr, "Error: Created test game is not in classic mode.\n");
        ctl_enter_menu(true);
        return;
    }

    var_game->gravity_control = 2;

    // Switch to test mode.
    var_gui_mode              = GUIMODE_TEST;

    // Initialization of the GUI elements.
    var_init_idle();
    var_selected_button = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b),
                     var_uiboxes[BX_TESTREGEN].x);
    var_game_ia = false;
    init_time_counter();
}

void ctl_test_load(const char *filepath) {
    // Killing all eventual previous games and reinitializing the zoom level.
    ctl_kill_game();
    init_zoom_game();
    // Load the game from the specified file.
    var_game = load_game(filepath);
    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Failed to load test game from %s\n", filepath);
        ctl_enter_menu(true);
        return;
    }

    // Enforces that the gravity control is free.
    var_game->gravity_control = 2;

    if (var_game->puzzle && var_game->refill_mode != 2) {
        ctl_init_storage_buttons(board_get_width(var_game->b),
                                 board_get_height(var_game->b));
    }

    // Switch to test mode.
    var_gui_mode = GUIMODE_TEST;

    var_init_idle();
    var_selected_button = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b),
                     var_uiboxes[BX_TESTREGEN].x);
    reset_flags();
    init_time_counter();
}

void ctl_load_game(const char *filepath) {
    reset_flags();
    // Killing all eventual previous games.
    ctl_kill_game();
    // Initialization of the zoom level for the game.
    init_zoom_game();

    // Creation of the game itself
    var_game = load_game(filepath);

    if (!ctl_check_game(var_game, true)) {
        fprintf(stderr, "Error: Failed to load the game.\n");
        ctl_enter_menu(true);
        return;
    }

    if (var_game->puzzle) {
        var_gui_mode            = GUIMODE_PUZZLE;
        var_current_puzzle_mode = PUZZLE_GAMEPLAY;
        if (var_game->refill_mode != 2) {
            ctl_init_storage_buttons(board_get_width(var_game->b),
                                     board_get_height(var_game->b));
        }

        // Creates a clone of the game for restoration after solving.
        var_game_backup = game_clone(var_game);
    } else {
        var_gui_mode             = GUIMODE_CLASSIC;
        var_current_classic_mode = CLASSIC_GAMEPLAY;
    }

    var_selected_button = BT_NONE;
    scroll_to_center(board_get_width(var_game->b),
                     board_get_height(var_game->b), var_uiboxes[BX_GRAVITY].x);
    var_init_idle();
    if (var_game->score) {
        init_time_counter_start(var_game->score->scores[SC_TIME]);
    } else {
        init_time_counter();
    }

    var_game_ia = false;
}

bool ctl_keyboard_testmodemenu(SDL_KeyCode key) {
    if (flags[PUSHED]) {
        return false; // Ignore mouse clicks if a button is already pushed
    }

    if (flags[NOGAMEINPUT]) {
        if (key == SDLK_ESCAPE) {
            var_push_button(BT_MENU); // Only menu button is active
            return true;
        }
        return false;
    }

    // keyboard_gameplay(key);
    switch (key) {
    case SDLK_ESCAPE:
        var_push_button(BT_MENU);
        return true;
        break;
    case SDLK_h:
        if (!var_game->puzzle) {
            var_push_button(BT_HINT);
            return true;
        }
        break;
    case SDLK_s:
        var_push_button(BT_SAVE);
        return true;
        break;
    case SDLK_d:
        if (!var_game->puzzle && !var_game->nodelay) {
            var_push_button(BT_DETONATE);
            return true;
        }
        break;
    default:
        break;
    }

    return ctl_keyboard_gravitywheel(key);
}

bool ctl_mouse_testmodemenu(int x, int y) {
    if (flags[PUSHED]) {
        return false; // Ignore mouse clicks if a button is already pushed
    }

    for (int i = BT_TESTWIDTH; i <= BT_UPGRADES; i++) {
        if (mouse_capture_button(x, y, i)) {
            return true;
        }
    }

    if (var_test_game_refill_mode == 1) {
        if (mouse_capture_button(x, y, BT_TESTSTINIT)) {
            return true;
        }
    }

    if (var_game && !var_game->puzzle) {
        for (int i = BT_NODELAY; i <= BT_BOMB; i++) {
            if (mouse_capture_button(x, y, i)) {
                return true;
            }
        }
    } else {
        for (int i = BT_TESTGENPUZZLE; i <= BT_TESTBALANCED; i++) {
            if (mouse_capture_button(x, y, i)) {
                return true;
            }
        }
    }

    if (flags[NOGAMEINPUT]) {
        return mouse_capture_button(x, y,
                                    BT_MENU); // Only menu button is active
    }

    // Otherwise, we check all buttons.

    // Gravity wheel buttons
    if (ctl_mouse_gravitywheel(x, y)) {
        return true;
    }

    // Save and menu buttons
    if (mouse_capture_button(x, y, BT_MENU) ||
        mouse_capture_button(x, y, BT_SAVE)) {
        return true;
    }

    // Buttons depending on game mode
    if (!var_game->puzzle) {
        if (mouse_capture_button(x, y, BT_HINT)) {
            return true;
        }

        if (!var_game->nodelay && mouse_capture_button(x, y, BT_DETONATE)) {
            return true;
        }
    }

    return false;
}

void ctl_button_push_testmode(void) {
    switch (var_pushed_button) {
    case BT_SELECMODEL:
        var_game->lines = true;
        break;
    case BT_SELECMODER:
        var_game->lines = false;
        break;
    case BT_NODELAY:
        var_game->nodelay = !var_game->nodelay;
        break;
    case BT_EASY:
        var_game->easy = !var_game->easy;
        break;
    case BT_WRAP:
        var_game->gravity_gems = !var_game->gravity_gems;
        break;
    case BT_BOMB:
        var_game->bombs = !var_game->bombs;
        break;
    case BT_UPGRADES:
        var_game->upgrades = !var_game->upgrades;
        break;
    case BT_TESTGENCLASSIC:
        ctl_test_classic();
        break;
    case BT_TESTGENPUZZLE:
        ctl_test_puzzle();
        break;
    case BT_TESTBALANCED:
        var_test_game_balanced = !var_test_game_balanced;
        break;
    case BT_TESTSTGEMS:
        var_test_game_refill_mode = 0;
        break;
    case BT_TESTSTBOMBS:
        var_test_game_refill_mode = 1;
        break;
    case BT_TESTSTNONE:
        var_test_game_refill_mode = 2;
        break;
    case BT_TESTHEIGHT:
        init_text_buffer(ED_TESTHEIGHT);
        break;
    case BT_TESTWIDTH:
        init_text_buffer(ED_TESTWIDTH);
        break;
    case BT_TESTLOAD:
        init_text_buffer(ED_LOAD);
        break;
    case BT_TESTSTINIT:
        init_text_buffer(ED_TESTSTORAGEINI);
        break;
    case BT_DETONATE:
        game_handle_detonation(var_game);
        break;
    case BT_HINT:
        game_handle_hint_request(var_game);
        break;
    case BT_SAVE:
        ctl_update_game_timer();
        init_text_buffer(ED_SAVE);
        break;
    case BT_MENU:
        ctl_enter_menu(true);
        break;

    default:
        break;
    }
    ctl_button_push_gravitywheel();
}

/****************/
/*+ Game state +*/
/****************/

void ctl_solver_turn(void) {
    if (!var_game || !var_game_solver || !var_game_solver_path) {
        var_game_solver = false;
        return;
    }
    game_implement_solver_move(var_game, var_game_solver_path);
    solver_move *temp    = var_game_solver_path;
    var_game_solver_path = var_game_solver_path->next;
    free(temp);
    if (!var_game_solver_path) {
        var_game_solver = false;
    }
}

/***********************/
/*+ Display functions +*/
/***********************/

static void ctl_display_graylayer(void) {
    // Displays a gray layer over the demo game.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 127);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
}

void ctl_display(void) {
    render_game(var_game);
    switch (var_gui_mode) {
    case GUIMODE_MENUS:
        ctl_display_graylayer();
        switch (var_current_menu) {
        case MENU_MAIN:
            render_mainmenu();
            break;
        case MENU_CLASSIC:
            render_classic_setup();
            break;
        case MENU_PUZZLE:
            render_puzzle_setup();
            break;
        default:
            fprintf(stderr, "Unknown GUI menu: %d\n", var_current_menu);
            exit(EXIT_FAILURE);
        }
        break;
    case GUIMODE_CLASSIC:
        render_classicmode();
        break;
    case GUIMODE_PUZZLE:
        render_puzzle();
        break;
    case GUIMODE_TEST:
        render_testmodemenu();
        break;
    default:
        fprintf(stderr, "Unknown GUI mode: %d\n", var_gui_mode);
        exit(EXIT_FAILURE);
        break;
    }
}

/*******************/
/*+ Button pushes +*/
/*******************/

void ctl_button_push(void) {
    if (var_gui_mode == GUIMODE_MENUS) {
        switch (var_current_menu) {
        case MENU_MAIN:
            ctl_button_push_mainmenu();
            break;
        case MENU_CLASSIC:
            ctl_button_push_setupclassic();
            break;
        case MENU_PUZZLE:
            ctl_button_push_setuppuzzle();
            break;
        default:
            fprintf(stderr, "Unknown GUI menu: %d\n", var_current_menu);
            exit(EXIT_FAILURE);
        }
    } else {
        switch (var_gui_mode) {
        case GUIMODE_CLASSIC:
            ctl_button_push_classic();
            break;
        case GUIMODE_PUZZLE:
            ctl_button_push_puzzle();
            break;
        case GUIMODE_TEST:
            ctl_button_push_testmode();
            break;
        default:
            fprintf(stderr, "Unknown GUI mode: %d\n", var_gui_mode);
            exit(EXIT_FAILURE);
        }
    }
    var_pushed_button = BT_NONE;
}

/*********************************/
/*+ Prise en compte des actions **/
/*********************************/

void ctl_mouse_motion(SDL_Event *theevent) {
    if (flags[MOUSEDOWN] && !flags[NOSCROLL]) {
        scroll_with_mouse(theevent->motion.x, theevent->motion.y);
    }
    var_mousePosx = theevent->motion.x;
    var_mousePosy = theevent->motion.y;
}

static bool ctl_mouse_button_action(int x, int y) {
    if (var_gui_mode == GUIMODE_MENUS) {
        switch (var_current_menu) {
        case MENU_MAIN:
            return ctl_mouse_mainmenu(x, y);
            break;
        case MENU_CLASSIC:
            return ctl_mouse_classic_setup(x, y);
            break;
        case MENU_PUZZLE:
            return ctl_mouse_puzzle_setup(x, y);
            break;
        default:
            break;
        }
    } else {
        switch (var_gui_mode) {
        case GUIMODE_CLASSIC:
            return ctl_mouse_classicmode(x, y);
            break;
        case GUIMODE_PUZZLE:
            return ctl_mouse_puzzle(x, y);
            break;
        case GUIMODE_TEST:
            return ctl_mouse_testmodemenu(x, y);
            break;
        default:
            break;
        }
    }
    return false;
}

void ctl_mouse_click(SDL_Event *theevent) {
    if (!flags[MOUSEDOWN] && theevent->button.button == SDL_BUTTON_LEFT) {
        set_flag(MOUSEDOWN);
    }
    if (ctl_mouse_button_action(theevent->button.x, theevent->button.y) ||
        flags[NOGAMEINPUT] || flags[NOPLAYER]) {
        return;
    } else {
        var_mouse_time = SDL_GetTicks64();
    }
}

void ctl_mouse_unclick(SDL_Event *theevent) {
    if (flags[MOUSEDOWN] && theevent->button.button == SDL_BUTTON_LEFT) {
        remove_flag(MOUSEDOWN);
    }

    if (flags[NOGAMEINPUT] || flags[NOPLAYER]) {
        return;
    }

    if (SDL_GetTicks64() - var_mouse_time < 200) {
        int i, j;
        render_coordinates_screen_to_board(theevent->button.x,
                                           theevent->button.y, &i, &j);

        // If these are valid board coordinates, we handle the click.
        if (ctl_valid_coordinates(var_game, i, j)) {
            game_handle_mouse_click(var_game, i, j);
            return;
        }

        // If there is a refill queue and the click is on the border, we handle
        // it.
        int ind = ctl_valid_border_coordinates(var_game, i, j);
        if (var_game->refill_mode != 2 && ind != INT_MAX) {
            ctl_fillbuttons[(board_get_gravity(var_game->b) + 2) % 4][ind] =
                true;
            var_button_countdown = SDL_GetTicks();
            set_flag(PUSHED);
            game_handle_refill(var_game, ind);
        }
    }
}

void ctl_input(SDL_Event *theevent) {
    switch (theevent->key.keysym.sym) {
    case SDLK_F1:
        ctl_enter_menu(true);
        break;
    case SDLK_F4:
        set_flag(QUIT);
        break;
    default:
        break;
    }

    if (var_gui_mode == GUIMODE_MENUS) {
        switch (var_current_menu) {
        case MENU_MAIN:
            ctl_keyboard_mainmenu(theevent->key.keysym.sym);
            break;
        case MENU_CLASSIC:
        case MENU_PUZZLE:
            ctl_keyboard_general_menu(theevent->key.keysym.sym);
            break;
        default:
            break;
        }
        return;
    } else {
        switch (var_gui_mode) {
        case GUIMODE_CLASSIC:
            ctl_keyboard_classicmode(theevent->key.keysym.sym);
            break;
        case GUIMODE_PUZZLE:
            ctl_keyboard_puzzle(theevent->key.keysym.sym);
            break;
        case GUIMODE_TEST:
            ctl_keyboard_testmodemenu(theevent->key.keysym.sym);
            break;
        default:
            break;
        }
    }
}

static char filepath[256];
void ctl_handle_edit_return(void) {
    switch (var_edit_mode) {
    case ED_SAVE:
        sprintf(filepath, "./saves/%s", var_editbuffer->content);
        save_game(var_game, filepath);
        break;
    case ED_LOAD:
        sprintf(filepath, "./saves/%s", var_editbuffer->content);
        if (var_gui_mode == GUIMODE_MENUS) {
            ctl_load_game(filepath);
        } else if (var_gui_mode == GUIMODE_TEST) {
            ctl_test_load(filepath);
        }
        break;
    case ED_NAME:
        if (!var_score_table) {
            fprintf(stderr, "No score table initialized.\n");
            exit(EXIT_FAILURE);
            break;
        }
        if (var_score_table->player_index >= 0 &&
            var_score_table->player_index < var_score_table->size) {
            save_score_table(var_score_table, "./scores/scores",
                             var_editbuffer->content);
        } else {
            fprintf(stderr, "Invalid player index in score table: %d\n",
                    var_score_table->player_index);
            exit(EXIT_FAILURE);
        }
        ctl_enter_menu(true);
        break;
    default:
        update_from_text_buffer();
        break;
    }
    free_text_buffer(var_editbuffer);
    var_editbuffer = NULL;
    var_edit_mode  = ED_NONE;
}

/*******************/
/*+ Boucle de jeu **/
/*******************/

void event_loop() {
    // game* testcb = load_original("test10.bpz");
    // save_game(testcb, "./saves/mytest");
    // game_release(testcb);

    // The loop always starts in menu mode.
    ctl_enter_menu(true);

    // ctl_init_classic();

    // Déclaration de l'événement
    SDL_Event event;

    // int toto = 0;

    // Main game loop
    while (!flags[QUIT]) {
        // Handling animations
        switch (var_anim_type) {
        case ANIM_KILL:
            var_kill_increment();
            if (var_kill_get_frame() >= 32) {
                game_end_animation(var_game, ANIM_KILL);
                remove_flag(NOGAMEINPUT);
            }
            break;
        case ANIM_MOVE_BADSWAP:
        case ANIM_MOVE_FALL:
        case ANIM_MOVE_GOODSWAP:
        case ANIM_MOVE_ROLLBACK:
            var_move_increment();
            // If the move animation has reached its maximum frame count.
            if (var_move_get_frame() >= NBFRAMES_MOVE) {
                var_cell_movement_done++;
                if (var_cell_movement_done >= var_cell_movement) {
                    game_end_animation(var_game, var_anim_type);
                    remove_flag(NOGAMEINPUT);
                } else {
                    var_move_reset();
                }
            }
            break;
        case ANIM_HINT:
            var_hint_increment();
            if (var_hint_get_frame() >= NBFRAMES_HINT) {
                game_end_animation(var_game, ANIM_HINT);
                remove_flag(NOGAMEINPUT);
            }
            break;
        case ANIM_IDLE:
            remove_flag(NOGAMEINPUT);
        default:
            break;
        }
        // Handling signals sent by the game implementation after an eventual
        // animation end.
        ctl_handle_anim_signals();
        ctl_handle_end_signals();

        // Handling events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                set_flag(QUIT);
                break;
            }

            // Locks the keyboard until a key is released.
            if ((flags[KEYDOWN] && event.type != SDL_KEYUP)) {
                continue;
            }
            if (flags[KEYDOWN] && event.type == SDL_KEYUP) {
                flags[KEYDOWN] = false;
            }

            // Text input mode
            if (var_edit_mode != ED_NONE) {
                switch (event.type) {
                case SDL_TEXTINPUT:
                    if (var_edit_mode == ED_NAME &&
                        (event.text.text[0] == ',' ||
                         event.text.text[0] == ';')) {
                        break;
                    }
                    append_to_text_buffer(var_editbuffer, event.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_BACKSPACE:
                        if (var_editbuffer->length > 0)
                            remove_char(var_editbuffer);
                        break;
                    case SDLK_RETURN:
                    case SDLK_KP_ENTER:
                        ctl_handle_edit_return();
                        break;
                    case SDLK_ESCAPE:
                        free_text_buffer(var_editbuffer);
                        var_editbuffer = NULL;
                        var_edit_mode  = ED_NONE;
                        break;
                    default:
                        break;
                    }
                default:
                    break;
                }
                continue;
            }

            // Mouse handling
            switch (event.type) {
            case SDL_MOUSEMOTION:
                ctl_mouse_motion(&event);
                break;
            case SDL_MOUSEBUTTONDOWN:
                ctl_mouse_click(&event);
                break;
            case SDL_MOUSEBUTTONUP:
                ctl_mouse_unclick(&event);
                break;

            default:
                break;
            }

            // Keyboard handling
            if (event.type == SDL_KEYDOWN) {
                set_flag(KEYDOWN);
                ctl_input(&event);
            }
        }

        // Handling signals sent by the game implementation
        ctl_handle_anim_signals();

        // Reset of the buttons and handling of pushes (if necessary)
        if (flags[PUSHED] && SDL_GetTicks() - var_button_countdown >= 100) {
            var_reset_buttons();
            for (int d = 0; d < SIZE_DIRECTION; d++) {
                if (ctl_fillbuttons[d]) {
                    for (int i = 0; i < ctl_fillbuttons_count[d]; i++) {
                        ctl_fillbuttons[d][i] = false;
                    }
                }
            }
            remove_flag(PUSHED);
            ctl_button_push();
        }

        // Handling signals sent by the game implementation
        ctl_handle_anim_signals();

        // Handling the case when the IA is activated.
        if (var_game_ia && !flags[NOGAMEINPUT] && !flags[INMENU] &&
            SDL_GetTicks64() - var_ia_countdown >= 1000) {
            var_ia_countdown = SDL_GetTicks64();
            ia_functions[var_game_ia_choice](var_game);
        }

        if (var_game_solver && !flags[NOGAMEINPUT] && !flags[INMENU]) {
            ctl_solver_turn();
        }
        ctl_handle_anim_signals();
        ctl_handle_end_signals();

        // Displaying the game
        SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
        SDL_RenderClear(renderer);

        ctl_display();

        var_increment_counters();

        SDL_RenderPresent(renderer);
        SDL_Delay(5); // 5
    }
}
