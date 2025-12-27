#include "gui_gamevars.h"
#include "gui_init_graphic.h"
#include "project_parameters.h"

/****************************/
/*+ Time counter functions +*/
/****************************/

int time_start;
int time_counter;
bool time_paused = false;

void init_time_counter(void)
{
    time_counter = 0;
    time_paused = false;
    time_start = SDL_GetTicks();
}

void init_time_counter_start(long start)
{
    time_counter = start * 1000;
    time_paused = false;
    time_start = SDL_GetTicks();
}

void pause_time_counter(void)
{
    time_counter += SDL_GetTicks() - time_start;
    time_paused = true;
}
void resume_time_counter(void)
{
    time_paused = false;
    time_start = SDL_GetTicks();
}
int get_time_counter(void)
{
    if (time_paused)
    {
        return time_counter;
    }
    else
    {
        return time_counter + SDL_GetTicks() - time_start;
    }
}

/***********/
/*+ Flags +*/
/***********/

bool flags[NB_FLAGS];

void flip_flag(ctl_flags f)
{
    flags[f] = !flags[f];
}

void set_flag(ctl_flags f)
{
    flags[f] = true;
}

void remove_flag(ctl_flags f)
{
    flags[f] = false;
}

void reset_flags(void)
{
    for (int i = 0; i < NB_FLAGS; i++)
        flags[i] = false;
}

/******************************/
/*+ Dimensions of the window +*/
/******************************/

int var_screen_width, var_screen_height, var_playarea_width;

int var_column_width = 200; // Default width of the information column

/*******************/
/*+ Mouse control +*/
/*******************/

int var_mousePosx;
int var_mousePosy;
bool var_leftMouseButtonDown;
long var_mouse_time;

/************************/
/*+ Zoom and Scrolling +*/
/************************/

short var_render_zoom_level = 1;
short var_tile_size = 80;
short var_flamed_yshift = 12;

void init_zoom_game(void)
{
    var_render_zoom_level = 1;
    var_tile_size = 80;
    var_flamed_yshift = 12;
    scroll_update_borders();
}

int var_scrollOffsetx;
int var_scrollOffsety;
int var_startx;
int var_starty;
int var_endx;
int var_endy;
int var_topleftx;
int var_toplefty;

void scroll_update_borders(void)
{

    if (var_scrollOffsetx >= 0)
    {
        var_startx = 0;
        var_topleftx = var_scrollOffsetx;
        var_endx = var_startx + (var_screen_width - var_topleftx) / var_tile_size + 2;
    }
    else
    {
        var_topleftx = var_scrollOffsetx % var_tile_size;
        var_startx = (var_topleftx - var_scrollOffsetx) / var_tile_size;
        var_endx = var_startx + var_screen_width / var_tile_size + 2;
    }

    if (var_scrollOffsety >= 0)
    {
        var_starty = 0;
        var_toplefty = var_scrollOffsety;
        var_endy = var_starty + (var_screen_height - var_toplefty) / var_tile_size + 2;
    }
    else
    {
        var_starty = (-1 * var_scrollOffsety) / var_tile_size;
        var_toplefty = var_scrollOffsety % var_tile_size;
        var_endy = var_starty + var_screen_height / var_tile_size + 2;
    }
}

void decrease_zoom(void)
{

    if (var_render_zoom_level <= 0)
    {
        return;
    }
    else
    {
        var_render_zoom_level--;
        var_tile_size /= 2;
        var_flamed_yshift /= 2;
        var_scrollOffsetx = ((var_scrollOffsetx - var_screen_width / 2) / (var_render_zoom_level + 1)) * var_render_zoom_level + var_screen_width / 2;
        var_scrollOffsety = ((var_scrollOffsety - var_screen_height / 2) / (var_render_zoom_level + 1)) * var_render_zoom_level + var_screen_height / 2;
        scroll_update_borders();
    }
}
void increase_zoom(void)
{

    if (var_render_zoom_level >= 2)
    {
        return;
    }
    else
    {
        var_render_zoom_level++;
        var_tile_size *= 2;
        var_flamed_yshift *= 2;
        var_scrollOffsetx = ((var_scrollOffsetx - var_screen_width / 2) / (var_render_zoom_level - 1)) * var_render_zoom_level + var_screen_width / 2;
        var_scrollOffsety = ((var_scrollOffsety - var_screen_height / 2) / (var_render_zoom_level - 1)) * var_render_zoom_level + var_screen_height / 2;
        scroll_update_borders();
    }
}

void scroll_manual(int x, int y)
{
    var_scrollOffsetx += x;
    var_scrollOffsety += y;
    scroll_update_borders();
}

void scroll_with_mouse(int x, int y)
{
    var_scrollOffsetx += x - var_mousePosx;
    var_scrollOffsety += y - var_mousePosy;
    scroll_update_borders();
}

void scroll_to_coords(int x, int y)
{
    var_scrollOffsetx = -1 * x * var_tile_size + var_screen_width / 2;
    var_scrollOffsety = -1 * y * var_tile_size + var_screen_height / 2;
    scroll_update_borders();
}

void scroll_to_center(int w, int h, int screen_width)
{

    var_scrollOffsetx = screen_width / 2 - (w + 1) * var_tile_size / 2;

    var_scrollOffsety = var_screen_height / 2 - (h + 1) * var_tile_size / 2;
    scroll_update_borders();
}

/*******************/
/*+ Keyboard text +*/
/*******************/

edit_mode var_edit_mode = ED_NONE;
text_buffer *var_editbuffer = NULL;

void init_text_buffer(edit_mode mode)
{
    var_edit_mode = mode;
    var_editbuffer = create_text_buffer();
}

void update_from_text_buffer(void)
{
    char *end_str;
    int num = strtol(var_editbuffer->content, &end_str, 10);
    if (*end_str != '\0')
    {
        fprintf(stderr, "Error: malformed string\n");
        return;
    }
    switch (var_edit_mode)
    {
    case ED_WIDTH:
        var_game_hsize = num >= 4 ? num : 4;
        break;
    case ED_TESTWIDTH:
        var_test_game_hsize = num >= 4 ? num : 4;
        break;
    case ED_HEIGHT:
        var_game_vsize = num >= 4 ? num : 4;
        break;
    case ED_TESTHEIGHT:
        var_test_game_vsize = num >= 4 ? num : 4;
        break;
    case ED_CLEARSCORE:
        var_game_clearscore = num >= 1 ? num : 1;
        var_game_clearscore = MIN(var_game_clearscore, 999999999); // Ensure maximal clear score
        break;
    case ED_CLEARMULTI:
        var_game_clearmulti = num >= 0 ? num : 0;
        var_game_clearmulti = MIN(var_game_clearmulti, 10000); // Ensure maximal clear multi
        break;
    case ED_STORAGEINI:
        var_game_storage_initial = num >= 0 ? num : 0;
        break;
    case ED_TESTSTORAGEINI:
        var_test_game_storage_initial = num >= 0 ? num : 0;
        break;
    default:
        break;
    }
}

/********************/
/*+ Game variables +*/
/********************/

int var_game_hsize = GAME_WIDTH;
int var_game_vsize = GAME_HEIGHT;

int var_test_game_hsize = TEST_WIDTH;
int var_test_game_vsize = TEST_HEIGHT;

int var_game_clearscore = GAME_CLEARSCORE;
int var_game_clearmulti = GAME_CLEARMULTI;

bool var_game_lines = GAME_LINES;                    //!< Current mode of the game (lines or SCCs)
bool var_game_nodelay = GAME_NODELAY;                //!< Flag indicating if nodelay mode is enabled.
bool var_game_easy = GAME_EASY;                      //!< Flag indicating if easy mode is enabled.
bool var_game_wrap = GAME_WRAP;                      //!< Flag indicating if wrap gems are enabled.
bool var_game_bombs = GAME_BOMBS;                    //!< Flag indicating if bombs are enabled.
bool var_game_upgrades = GAME_UPGRADES;              //!< Flag indicating if gem upgrades are enabled.
short var_game_refill_mode = GAME_REFILL_MODE;       //!< Refill mode used in puzzle mode (0: stores eliminated gems, 1: predetermined queue of rocks and bombs, 2: No refill).
bool var_game_balanced = GAME_BALANCED;              //!< Flag indicating if balanced generation is enabled in puzzle mode.
int var_game_storage_initial = GAME_STORAGE_INITIAL; //!< Initial storage size in puzzle mode.

short var_test_game_refill_mode = TEST_REFILL_MODE;       //!< Refill mode used in puzzle mode (0: stores eliminated gems, 1: predetermined queue of rocks and bombs, 2: No refill).
bool var_test_game_balanced = TEST_BALANCED;              //!< Flag indicating if balanced generation is enabled in puzzle mode.
int var_test_game_storage_initial = TEST_STORAGE_INITIAL; //!< Initial storage size in puzzle mode.

bool var_game_ia_menu = false;
bool var_game_ia = false;
ia_classic var_game_ia_choice_menu = IA_RANDOM;
ia_classic var_game_ia_choice = IA_RANDOM;
int var_ia_countdown = 1500; //!< Countdown timer for IA moves (in milliseconds).
short var_gravity_mode = 1;  //!< Gravity mode (0: cannot change, 1: rotate clockwise, 2: free).

solver var_game_solver_mode = 0; //!< Solver mode (0: BFS, 1: DFS, 3: A*).
bool var_game_solver = false;
solver_move *var_game_solver_path = NULL;
int var_game_solver_nb_moves = 0;
int var_game_solver_nb_configs = 0;
solver_result_type var_game_solver_result = SOLVER_UNSOLVABLE;

game *var_game = NULL;        //!< The current game.
game *var_game_backup = NULL; //!< Backup of the current game.

score_table *var_score_table = NULL;

void var_release_score_table(void)
{
    if (var_score_table)
    {
        delete_score_table(var_score_table);
        var_score_table = NULL;
    }
}

gui_mode var_gui_mode = GUIMODE_MENUS;
gui_menu var_current_menu = MENU_MAIN;
gui_classic var_current_classic_mode = CLASSIC_GAMEPLAY;
gui_puzzle var_current_puzzle_mode = PUZZLE_GAMEPLAY;

bool var_high_scores = false;

/*****************************/
/*+ Handling animation mode +*/
/*****************************/
anim_type var_anim_type = ANIM_IDLE;
int var_cell_movement = 0;
int var_cell_movement_done = 0;

void var_init_idle(void)
{
    var_anim_type = ANIM_IDLE; // Set the animation type to idle
    remove_flag(NOGAMEINPUT);  // Allow user input
    if (var_game_ia)
    {
        var_ia_countdown = SDL_GetTicks64();
    }
}

void var_init_movement(void)
{
    if (!var_game)
    {
        return;
    }
    game *g = var_game;
    // Computing the maximum cell movement distance
    var_cell_movement = 0;
    var_cell_movement_done = 0;
    for (int i = 0; i < board_get_width(g->b); i++)
    {
        for (int j = 0; j < board_get_height(g->b); j++)
        {
            int distance;
            game_get_cell_move(g, i, j, &distance);
            if (distance > var_cell_movement)
            {
                var_cell_movement = distance; // Update the maximum movement distance
            }
        }
    }

    // If there are no movements, we end the movement animation
    if (var_cell_movement <= 0)
    {
        game_end_animation(g, game_anim_signal); // End the movement animation
        return;
    }
    // Otherwise, initialize the movement animation in the GUI
    var_anim_type = game_anim_signal; // Assuming movement is represented as a move animation
    var_move_reset();                 // Resets the move animation frame
    set_flag(NOGAMEINPUT);            // Restrict input during the movement animation
}

void var_init_kill(void)
{
    // Initialize the explosion animation in the GUI
    var_anim_type = ANIM_KILL;
    // Resets the explosion animation frame
    var_kill_reset();
    set_flag(NOGAMEINPUT); // Restrict input during the explosion animation
}

void var_init_hint(void)
{
    // Initialize the hint animation in the GUI
    var_anim_type = ANIM_HINT;
    // Resets the hint animation frame
    var_hint_reset();
    set_flag(NOGAMEINPUT); // Restrict input during the hint animation
}

/***************************/
/*+ Continuous animations +*/
/***************************/

static short var_flame_frame = 0;
static short var_star_frame = 0;
static short var_effect_star_frame = 0;
static short var_spin_frame = 0;
static short var_hypercube_frame = 0;
static short var_wrap_frame = 0;

short var_flame_get_frame(void)
{
    return var_flame_frame >> 2;
}
short var_star_get_frame(void)
{
    return var_star_frame >> 2;
}
short var_effect_star_get_frame(void)
{
    return var_effect_star_frame >> 3;
}
short var_spin_get_frame(void)
{
    return var_spin_frame >> 2;
}
short var_hypercube_get_frame(void)
{
    return var_hypercube_frame >> 2;
}
short var_wrap_get_frame(void)
{
    return var_wrap_frame >> 1;
}

void var_increment_counters(void)
{
    var_flame_frame = (var_flame_frame + 1) % (NBFRAMES_FLAMES << 2);
    var_star_frame = (var_star_frame + 1) % (NBFRAMES_STARS << 2);
    var_effect_star_frame = (var_effect_star_frame + 1) % (NBFRAMES_STAREFFECT << 3);
    var_spin_frame = (var_spin_frame + 1) % (NBFRAMES_GEMS << 2);
    var_hypercube_frame = (var_hypercube_frame + 1) % (NBFRAMES_HYPERCUBE << 2);
    var_wrap_frame = (var_wrap_frame + 1) % (NBFRAMES_ROTATION << 1);
}

void var_reset_counters(void)
{
    var_flame_frame = 0;
    var_star_frame = 0;
    var_effect_star_frame = 0;
    var_spin_frame = 0;
    var_hypercube_frame = 0;
    var_wrap_frame = 0;
}

/**********************/
/*+ Timed animations +*/
/**********************/

static short var_move_frame = 0;
static short var_kill_frame = 0;
static short var_hint_frame = 0;

short var_move_get_frame(void)
{
    return var_move_frame;
}
void var_move_increment(void)
{
    var_move_frame++;
}
void var_move_reset(void)
{
    var_move_frame = 0;
}

short var_kill_get_frame(void)
{
    return var_kill_frame >> 1;
}
void var_kill_increment(void)
{
    var_kill_frame++;
}

void var_kill_reset(void)
{
    var_kill_frame = 0;
}

short var_hint_get_frame(void)
{
    return var_hint_frame >> 2;
}
void var_hint_increment(void)
{
    var_hint_frame++;
}

void var_hint_reset(void)
{
    var_hint_frame = 0;
}

/******* Scores */

int var_scores_page = 0;
int var_scores_max_page = 0;
int var_scores_per_page = 14;
