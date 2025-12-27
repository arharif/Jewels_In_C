/**
 * @file gui_init_graphic.h
 * @brief
 * Initialization of the graphics module. DO NOT MODIFY.
 *
 * @attention
 * This part is already implemented. It should not be modified.
 */

#ifndef _GUI_INIT_GRAPHIC_H_
#define _GUI_INIT_GRAPHIC_H_

#include "gui_gamevars.h"
#include "gui_textbuf.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

/*  ___       _ _   _       _ _          _   _                      __   ____  ____  _      */
/* |_ _|_ __ (_) |_(_) __ _| (_)______ _| |_(_) ___  _ __     ___  / _| / ___||  _ \| |     */
/*  | || '_ \| | __| |/ _` | | |_  / _` | __| |/ _ \| '_ \   / _ \| |_  \___ \| | | | |     */
/*  | || | | | | |_| | (_| | | |/ / (_| | |_| | (_) | | | | | (_) |  _|  ___) | |_| | |___  */
/* |___|_| |_|_|\__|_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|  \___/|_|   |____/|____/|_____| */

/************/
/*+ Colors +*/
/************/

/**
 * @brief Black.
 */
static const SDL_Color BLACK = {0, 0, 0, 255};

/**
 * @brief White.
 */
static const SDL_Color WHITE = {255, 255, 255, 255};

/**
 * @brief Gray
 */
static const SDL_Color GRAY = {32, 32, 32, 200};

/**
 * @brief Light gray
 */
static const SDL_Color LIGHT_GRAY = {64, 64, 64, 200};

/**
 * @brief Blue
 */
static const SDL_Color BLUET = {25, 25, 252, 200};

/**
 * @brief Orange
 */
static const SDL_Color ORANGE = {239, 216, 7, 200};

/**
 * @brief Pale.
 */
static const SDL_Color PALE = {250, 235, 215, 200};

#define COLOR_TUPLE(color) ((color).r), ((color).g), ((color).b), ((color).a)

/***********/
/*+ Fonts +*/
/***********/

extern TTF_Font *Lunera18, *Lunera20, *Lunera24, *Lunera28, *Lunera36, *Lunera72, *Diamond, *Noto;

/**
 * @brief Chargement des fonts.
 */
void init_fonts(void);

/*************/
/*+ Sprites +*/
/*************/

/**
 * @brief Initializes the module that loads the PNG images.
 */
void init_image(void);

typedef enum
{
    COL_BLUE,
    COL_GREEN,
    COL_GRAY,
    COL_RED,
    COL_YELLOW,
    COL_SIZE
} ui_color;

extern SDL_Texture *txt_ui[COL_SIZE];
extern SDL_Texture *txt_cursors;
extern SDL_Texture *txt_icons;
extern SDL_Texture *txt_advanced;
extern SDL_Texture *txt_bigarrows;
extern SDL_Texture *txt_gems[3][7];
extern SDL_Texture *txt_gems_shadows[3][7];
extern SDL_Texture *txt_gems_flames[3][7];
extern SDL_Texture *txt_gems_stars[3][7];
extern SDL_Texture *txt_gems_effect_star[3];
extern SDL_Texture *txt_hypercube[3];
extern SDL_Texture *txt_selector[3];
extern SDL_Texture *txt_gems_explosion[3];
extern SDL_Texture *txt_gems_explosion_circle[3];
extern SDL_Texture *txt_gems_shatter[3];
extern SDL_Texture *txt_hint_circle[3];
extern SDL_Texture *txt_hint_arrows[3];
extern SDL_Texture *txt_rock;
extern SDL_Texture *txt_bomb;

#define NBFRAMES_GEMS 20
#define NBFRAMES_FLAMES 26
#define NBFRAMES_STARS 20
#define NBFRAMES_STAREFFECT 23
#define NBFRAMES_HYPERCUBE 60
#define NBFRAMES_EXPLODE 16
#define NBFRAMES_SHATTER 20
#define NBFRAMES_HINT 16

typedef enum
{
    GAUGE_LEFT,   //!< Gauge left.
    GAUGE_CENTER, //!< Gauge center.
    GAUGE_RIGHT,  //!< Gauge right.
    GAUGE_SIZE,   //!< Element size.
} GAUGEs;

typedef enum
{
    UITYPE_EDGE,
    UITYPE_PLAIN,
    UITYPE_DEMI,
    UITYPE_SHADE,
    UITYPE_LINE,
    UITYPE_SIZE
} ui_type;

typedef enum
{
    UICON_UP,
    UICON_CROSS,
    UICON_SIZE
} ui_icons;

typedef enum
{
    UICA_NW,
    UICA_N,
    UICA_NE,
    UICA_W,
    UICA_C,
    UICA_E,
    UICA_SW,
    UICA_S,
    UICA_SE,
    UICA_SW_OFF,
    UICA_S_OFF,
    UICA_SE_OFF,
    UICA_SIZE
} ui_cardinals;

typedef enum
{
    MISC_ARROW_EAST,
    MISC_ARROW_NORTH,
    MISC_ARROW_SOUTH,
    MISC_ARROW_WEST,
    MISC_CARROW_EAST,
    MISC_CARROW_NORTH,
    MISC_CARROW_SOUTH,
    MISC_CARROW_WEST,
    MISC_RBOX_COLOR,
    MISC_RBOX_GRAY,
    MISC_RBOX_GFILLED,
    MISC_RBOX_CFILLED,
    MISC_SBOX_EMPTY,
    MISC_SBOX_OK,
    MISC_SBOX_NO,
    MISC_SBOX_STOP,
    MISC_SBOX_GEMPTY,
    MISC_SBOX_GOK,
    MISC_SBOX_GNO,
    MISC_SBOX_GSTOP,
    MISC_SIZE
} ui_misc;

typedef enum
{
    CUR_RIGHT,
    CUR_UP,
    CUR_RIGHTUP,
    CUR_LEFTUP,
    CUR_DOWN,
    CUR_RIGHTDOWN,
    CUR_LEFTDOWN,
    CUR_LEFT,
    CUR_CROSS_BIG,
    CUR_CROSS_SMALL,
    CUR_HAND_BIG,
    CUR_RHAND_BIG,
    CUR_UHAND_BIG,
    CUR_HAND_SMALL,
    CUR_RHAND_SMALL,
    CUR_UHAND_SMALL,
    CUR_THAND,
    CUR_VTHAND,
    CUR_PLUS,
    CUR_MINUS,
    CUR_LOCK,
    CUR_UNLOCK,
    CUR_BANG,
    CUR_QUESTION,
    CUR_SELEC_BIG,
    CUR_SELEC_SMALL,
    CUR_REWIND_LEFT,
    CUR_REWIND_RIGHT,
    CUR_BOMB,
    CUR_ZOOM_NEUTRAL,
    CUR_ZOOM_IN,
    CUR_ZOOM_OUT,
    CUR_SIZE
} ui_cursors;

typedef enum
{
    CIRCLE_BROWN,
    CIRCLE_GREY,
    CIRCLE_WHITE,
    CIRCLE_SIZE,
} ui_circle;

typedef enum
{
    ADV_LIGHT_BROWN,
    ADV_DARK_BROWN,
    ADV_LIGHT_GREY,
    ADV_BLUE,
    ADV_DARK_GREY,
    ADV_GREEN,
    ADV_RED,
    ADV_FRAMES_SIZE,
} ui_advframe;

extern SDL_Rect clp_gauges[GAUGE_SIZE];
extern SDL_Rect clp_misc_large[MISC_SIZE];
extern SDL_Rect clp_misc_small[MISC_SIZE];
extern SDL_Rect clp_round_buttons[UITYPE_SIZE];
extern SDL_Rect clp_square_buttons[UITYPE_SIZE];
extern SDL_Rect clp_frames[UITYPE_SIZE][UICA_SIZE];
extern SDL_Rect clp_cursors[CUR_SIZE];
extern SDL_Rect clp_circles[CIRCLE_SIZE];
extern SDL_Rect clp_icons[UICON_SIZE];
extern SDL_Rect clp_advanced[ADV_FRAMES_SIZE][UICA_SW_OFF];
extern SDL_Rect clp_bigarrows[CUR_CROSS_BIG];
extern SDL_Rect clp_gems_frames[3][NBFRAMES_GEMS];
extern SDL_Rect clp_flame_frames[3][NBFRAMES_FLAMES];
extern SDL_Rect clp_star_frames[3][NBFRAMES_STARS];
extern SDL_Rect clp_star_effect_frames[3][NBFRAMES_STAREFFECT];
extern SDL_Rect clp_hypercube_frames[3][NBFRAMES_HYPERCUBE];
extern SDL_Rect clp_selector[3];
extern SDL_Rect clp_gems_explosion_frames[3][NBFRAMES_EXPLODE];
extern SDL_Rect clp_gems_explosion_circle[3];
extern SDL_Rect clp_gems_shatter_frames[3][NBFRAMES_SHATTER];
extern SDL_Rect clp_hint_circle[3][NBFRAMES_HINT];
extern SDL_Rect clp_hint_arrows[3][SIZE_DIRECTION];
extern SDL_Rect clp_rock;
extern SDL_Rect clp_bomb[12];

/**
 * @brief Initialization of the sprites.
 */
void init_load_sprites(void);

/*****************/
/*+ Fenêtre SDL +*/
/*****************/

extern SDL_Window *window;
extern SDL_Renderer *renderer;

/**
 * @brief Initialization of SDL.
 */
bool init_SDL(void);

/**
 * @brief Shutdown of SDL.
 */
void shutdown_SDL(void);

#endif // _GUI_INIT_GRAPHIC_H_
