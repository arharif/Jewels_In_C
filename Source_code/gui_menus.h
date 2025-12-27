/**
 * @file gui_menus.h
 * @brief
 * Rendering of the game menus and definitions of the buttons. DO NOT MODIFY.
 *
 * @attention
 * This part is already implemented. It should not be modified.
 */

/*  __  __                       */
/* |  \/  | ___ _ __  _   _ ___  */
/* | |\/| |/ _ \ '_ \| | | / __| */
/* | |  | |  __/ | | | |_| \__ \ */
/* |_|  |_|\___|_| |_|\__,_|___/ */

#ifndef _GUI_MENUS_H_
#define _GUI_MENUS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <dirent.h>

#include "gui_gamevars.h"
#include "gui_init_graphic.h"
#include "project_tools.h"

// Score menu
#define SCORE_BT_WIDTH 105   // Button width
#define SCORE_BT_SEP 15      // Separation between buttons
#define SCORE_NAME_WIDTH 160 // Width of the name column

#define SCORE_BT_TOP 200    // Top of the buttons
#define SCORE_TI_TOP 80     // Top of the title
#define SCORE_WI_TOP 160    // Top of the window
#define SCORE_NB_TOP 240    // Top of the scores
#define SCORE_NB_STEP 30    // Step between scores
#define SCORE_BOT_SPACE 200 // Space at the bottom

/*************/
/*+ Anchors +*/
/*************/

/**
 * @brief
 * Anchor positions for buttons and frames.
 */
typedef enum
{
    ANC_NW, //!< North-West.
    ANC_NC, //!< North-Centre.
    ANC_NE, //!< North-East.
    ANC_CW, //!< Centre-West.
    ANC_CC, //!< Centre-Centre.
    ANC_CE, //!< Centre-East.
    ANC_SW, //!< South-West.
    ANC_SC, //!< South-Centre.
    ANC_SE  //!< South-East.
} anchor;

/**
 * @brief
 * Shifts the coordinates from the anchor to top left.
 */
void shift_to_anchor(int *, //!< Horizontal coordinate.
                     int *, //!< Vertical coordinate.
                     int,   //!< Width.
                     int,   //!< Height.
                     anchor //!< Anchor.
);

/*************/
/*+ Buttons +*/
/*************/

/**
 * @brief
 * Various frames used to display menus.
 */
typedef enum
{
    // Whole screen
    BX_SCREEN, //!< Screen.

    // HUD
    BX_LEVEL,    //!< Level.
    BX_SCORE,    //!< Score.
    BX_PUZZLE,   //!< Puzzle.
    BX_GRAVITY,  //!< Gravity.
    BX_DETONATE, //!< Detonate.

    // Test Mode
    BX_TESTPARSCLASSIC,
    BX_TESTPARSPUZZLE,
    BX_TESTREGEN,

    // Score
    BX_SCORESCREEN,

    // Puzzle end
    BX_PUZZLESUMMARY,
    BX_PUZZLESUMMARYLARGE,

    // Game setup
    BX_CLDIMENSIONS,
    BX_CLPARAMS,
    BX_CLPARAMSP,
    BX_CLCONTROL,
    BX_CLCHOICES,

    // Save/Load
    BX_FILENAME,

    BX_SIZE
} uibox;

/**
 * @brief Array of frames.
 */
extern SDL_Rect var_uiboxes[BX_SIZE];

/**
 * @brief
 * Internal names for all the buttons in the game.
 */
typedef enum
{

    // Main Menu
    BT_GAME,   //!< Classic game.
    BT_PUZZLE, //!< Puzzle game.
    BT_LOAD,   //!< Load game.
    BT_TEST,
    BT_SCORESCREEN, //!< Scores.
    BT_QUIT,        //!< Quit.

    // Hud
    BT_GRAVITY,
    BT_GUP,
    BT_GRIGHT,
    BT_GDOWN,
    BT_GLEFT,

    BT_MENU,
    BT_SAVE,
    BT_DETONATE,
    BT_HINT,
    BT_SOLVER,
    BT_SOLCL,
    BT_SOLCR,

    // Test mode
    BT_TESTHEIGHT,
    BT_TESTWIDTH,
    BT_TESTGENCLASSIC,
    BT_TESTGENPUZZLE,
    BT_TESTBALANCED,
    BT_TESTSTGEMS,
    BT_TESTSTBOMBS,
    BT_TESTSTNONE,
    BT_TESTLOAD,
    BT_SELECMODEL,
    BT_SELECMODER,
    BT_UPGRADES, //!< Upgrades.

    BT_NODELAY, //!< Nodelay mode.
    BT_EASY,    //!< Easy mode.
    BT_WRAP,    //!< Wrap gems.
    BT_BOMB,    //!< Bombs.

    BT_TESTSTINIT,

    BT_TESTSTDUMMY,

    // Save/Load
    BT_FILENAME,

    // Scores
    BT_SCOREBACK,
    BT_SCORECONTINUE,
    BT_SCORESAVE,
    BT_SCOREMENU,
    BT_SCGLOBAL,
    BT_SCLEVEL,
    BT_SCSWAPS,
    BT_SCCONSUMED,
    BT_SCFLAMED,
    BT_SCSTARRED,
    BT_SCHYPER,
    BT_SCGRAVITY,
    BT_SCTIME,
    BT_PREVSCORE,
    BT_NEXTSCORE,

    // Puzzle Menus
    BT_PUZZLEQUIT,
    BT_PUZZLEREGEN,
    BT_PUZZLERETRY,

    BT_PUZZLEBACKMENU,
    BT_PUZZLEBACKSOLVER,
    BT_PUZZLEIMPLEMENT,

    // Classic Menu

    BT_CLASSICQUIT,
    BT_CLASSICCONTINUE,
    BT_CLASSICRESTART,

    // Setup
    BT_SETWIDTH,
    BT_SETHEIGHT,
    BT_SETSELECMODEL,
    BT_SETSELECMODER,
    BT_SETSTART,
    BT_SETMENU,

    BT_SETNODELAY,
    BT_SETEASY,
    BT_SETWRAP,
    BT_SETBOMB,
    BT_SETCLEAR,
    BT_SETCLEARMULT,

    BT_SETCTL,
    BT_SETCTR,
    BT_SETIAL,
    BT_SETIAR,

    BT_SETBALANCED,
    BT_SETUPGRADES,
    BT_SETSTOREREFILL,
    BT_SETSTOREBOMB,
    BT_SETSTORENONE,
    BT_SETGRAVITYL,
    BT_SETGRAVITYC,
    BT_SETGRAVITYR,

    BT_SETINITIAL,

    BT_SIZE,
    BT_NONE,
} button;

/**
 * @brief
 * Counter for the button animation.
 */
extern int var_button_countdown;

/**
 * @brief
 * Currently selected button.
 */
extern button var_selected_button;

/**
 * @brief
 * The last pushed button.
 */
extern button var_pushed_button;

/**
 * @brief
 * Array indicating which buttons are currently being pushed (for animations).
 */
extern bool var_buttons_flags[BT_SIZE];

/**
 * @brief
 * Resets the buttons.
 */
void var_reset_buttons(void);

/**
 * @brief
 * Pushes a button.
 */
void var_push_button(button);

/**
 * @brief
 * Releases a button.
 */
void var_release_button(button);

/**
 * @brief
 * Data structure representing a single button.
 */
typedef struct
{
    uibox parent_box; //!< Parent box of the button.
    int x;
    int y;
    int w;
    int h;
    int t_x;
    int t_y;
    button up;
    button left;
    button down;
    button right;
} button_data;

/**
 * @brief
 * Data for all buttons.
 */
extern button_data var_buttons[BT_SIZE];

/******************************************/
/*+ Initialization of frames and buttons +*/
/******************************************/

/**
 * @brief Initialization of all the UI boxes in the program.
 */
void init_uiboxes(void);

/**
 * @brief
 * Initialization of a single button.
 */
void init_one_button(button bt, //!< The button's internal name.
                     int x,     //!< Horizontal coordinate.
                     int y,     //!< Vertical coordinate.
                     int w,     //!< Width.
                     int h,     //!< Height.
                     anchor a,  //!< Anchor.
                     uibox pbox //!< Parent box.
);

/**
 * @brief
 * Initialization of a Boolean toggle button.
 *
 * @remark
 * Anchor is fixed to NW.
 */
void init_boolean_toggle(button bt, //!< The button's internal name.
                         int x,     //!< Horizontal coordinate.
                         int y,     //!< Vertical coordinate.
                         uibox pbox //!< Parent box.
);

/**
 * @brief
 * Initialization of double choice buttons.
 */
void init_double_choice_buttons(button lb, //!< Left button.
                                button rb, //!< Right button.
                                int x,     //!< Horizontal coordinate.
                                int y,     //!< Vertical coordinate.
                                int tw,    //!< Width of the text next to each button.
                                anchor a,  //!< Anchor.
                                uibox pbox //!< Parent box.
);

/**
 * @brief
 * Initialization of triple choice buttons.
 */
void init_triple_choice_buttons(button lb, //!< Left button.
                                button mb, //!< Middle button.
                                button rb, //!< Right button.
                                int x,     //!< Horizontal coordinate.
                                int y,     //!< Vertical coordinate.
                                int tw,    //!< Width of the text next to each button.
                                anchor a,  //!< Anchor.
                                uibox pbox //!< Parent box.
);

/**
 * @brief Initialization an editor button.
 */
void init_editor_button(button bt, //!< The button.
                        int x,     //!< Horizontal coordinate.
                        int y,     //!< Vertical coordinate.
                        int lw,    //!< Label width.
                        int tw,    //!< Editor window width.
                        anchor a,  //!< Anchor.
                        uibox pbox //!< Parent box.
);

/**
 * @brief
 * Initialization of selector buttons.
 */
void init_selector_buttons(button lb, //!< Left button.
                           button rb, //!< Right button.
                           int x,     //!< Horizontal coordinate.
                           int y,     //!< Vertical coordinate.
                           int tw,    //!< Width.
                           anchor a,  //!< Anchor.
                           uibox pbox //!< Parent box.
);

/**
 * @brief
 * Initialization of all buttons.
 */
void init_all_buttons(void);

/**
 * @brief
 * Capture the mouse on a button.
 */
bool mouse_capture_button(int x,    //!< Horizontal coordinate of the mouse.
                          int y,    //!< Vertical coordinate of the mouse.
                          button bt //!< The button.
);

/*********************/
/*+ Displaying text **/
/*********************/

/**
 * @brief Displays text.
 */
int render_text(int,          //!< Horizontal coordinate.
                int,          //!< Vertical coordinate.
                const char *, //!< Text to display.
                TTF_Font *,   //!< Font to use.
                SDL_Color,    //!< Text color.
                anchor        //!< Anchor.
);

/**
 * @brief Displays text truncates the end if too long.
 */
int render_text_truncate(int,           //!< Horizontal coordinate.
                         int,           //!< Vertical coordinate.
                         int max_width, //!< Maximum width of the text.
                         const char *,  //!< Text to display.
                         TTF_Font *,    //!< Font to use.
                         SDL_Color,     //!< Text color.
                         anchor         //!< Anchor.
);

/**
 * @brief Displays text in an edition buffer.
 */
void print_text_buffer_center(void);

/***********************************/
/*+ Displaying frames and buttons +*/
/***********************************/

/**
 * @brief
 * Renders a standard button.
 */
void display_button(button bt,         //!< The button.
                    const char *label, //!< The label of the button.
                    TTF_Font *font,    //!< The font to use.
                    ui_color color,    //!< The color of the button.
                    ui_type type,      //!< The type of the button.
                    SDL_Color textcol  //!< The color of the text.
);

/**
 * @brief
 * Displays a Boolean toggle button.
 */
void display_boolean_toggle(button bt,        //!< The button.
                            const char *text, //!< The text to display.
                            bool checked,     //!< Whether the toggle is checked or not.
                            ui_color color    //!< The color of the button.
);

/**
 * @brief
 * Displays a single choice button.
 */
void display_one_choice_button(button bt,        //!< The button.
                               const char *text, //!< The text to display.
                               bool checked,     //!< Whether the choice is selected or not.
                               ui_color color    //!< The color of the button.
);

/**
 * @brief
 * Displays double choice buttons.
 */
void display_double_choice_buttons(button lb,         //!< Left button.
                                   const char *ltext, //< Text for the left button.
                                   button rb,         //!< Right button.
                                   const char *rtext, //!< Text for the right button.
                                   const char *label, //!< Label displayed above the choice buttons (NULL if not desired).
                                   bool left,         //!< Whether the left button is selected.
                                   ui_color color     //!< The color of the buttons.
);

/**
 * @brief
 * Displays triple choice buttons.
 */
void display_triple_choice_buttons(button lb,         //!< Left button.
                                   const char *ltext, //!< Text for the left button.
                                   button mb,         //!< Middle button.
                                   const char *mtext, //!< Text for the middle button.
                                   button rb,         //!< Right button.
                                   const char *rtext, //!< Text for the right button.
                                   const char *label, //!< Label displayed above the choice buttons (NULL if not desired).
                                   int n,             //!< Index of the selected button (0: left, 1: middle, 2: right).
                                   ui_color color     //!< The color of the buttons.
);

/**
 * @brief
 * Displays an editor button.
 */
void display_editor_button(button bt,             //!< The button.
                           const char *label,     //!< The label of the button.
                           const char *string,    //!< The text inside the editor.
                           ui_color color,        //!< The color of the button.
                           SDL_Color label_color, //!< The color of the label.
                           SDL_Color edit_color   //!< The color of the text inside the editor.
);

/**
 * @brief
 * Displays selector buttons.
 */
void display_selector_buttons(button lb,        //!< Left button.
                              button rb,        //!< Right button.
                              const char *text, //!< The text to display.
                              ui_color color    //!< The color of the buttons.
);

/**
 * @brief
 * Renders an advanced box.
 */
void render_ui_box_advanced(uibox box,        //!< The box.
                            ui_advframe frame //!< The frame to use.
);

/**
 * @brief
 * Displays a colored frame.
 */
void render_color_frame(int x,          //!< Horizontal coordinate.
                        int y,          //!< Vertical coordinate.
                        int w,          //!< Width.
                        int h,          //!< Height.
                        ui_color color, //!< Frame color (COL_BLUE, COL_GREEN, COL_GRAY, COL_RED or COL_YELLOW).
                        ui_type type,   //!< Type of frame (UITYPE_EDGE, UITYPE_PLAIN, UITYPE_DEMI, UITYPE_SHADE or UITYPE_LINE).
                        bool on,        //!< Whether the frame is pushed or not (used for buttons).
                        anchor anc      //!< Anchor for the placement.
);

/**
 * @brief
 * Displays an advanced frame.
 */
void render_advanced_frame(int x,             //!< Horizontal coordinate.
                           int y,             //!< Vertical coordinate.
                           int w,             //!< Width.
                           int h,             //!< Height.
                           ui_advframe frame, //!< Type of frame (ADV_LIGHT_BROWN, ADV_DARK_BROWN, ADV_LIGHT_GREY, ADV_BLUE, ADV_DARK_GREY or ADV_GREEN).
                           anchor anc         //!< Anchor for the placement.
);

/****************/
/*+ Main menus +*/
/****************/

/**
 * @brief Displays the main menu.
 */
void render_mainmenu(void);

/******************/
/*+ Score screen +*/
/******************/

/**
 * @brief Displays the score screen.
 */
void render_score_screen(short mode //!< Display mode (0: level, 1: ending, 2: high scores).
);

/*******************/
/*+ Gravity Wheel +*/
/*******************/

/**
 * @brief Displays the gravity control wheel (buttons depend on the on-going game).
 */
void render_gravity_wheel(void);

/******************/
/*+ Classic Mode +*/
/******************/

/**
 * @brief Displays the classic setup menu.
 */
void render_classic_setup(void);

/**
 * @brief Displays the classic mode overlay.
 */
void render_classicmode(void);

/*****************/
/*+ Puzzle Mode +*/
/*****************/

/**
 * @brief Displays the puzzle setup menu.
 */
void render_puzzle_setup(void);

/**
 * @brief Displays the puzzle mode menu.
 */
void render_puzzle(void);

/********************/
/*+ Test Mode menu +*/
/********************/

/**
 * @brief Displays the test mode menu.
 */
void render_testmodemenu(void);

/***************/
/*+ Save/Load +*/
/***************/

/**
 * @brief Displays the save/load menu.
 */
void render_textbox_menu(const char *message,     //!< Message to display.
                         ui_advframe backframe,   //!< Background frame.
                         SDL_Color message_color, //!< Color of the message.
                         ui_color box_color,      //!< Color of the box.
                         SDL_Color edit_color     //!< Color of the editor text.
);

#endif // _GUI_MENUS_H_
