#include "gui_menus.h"
#include "game_solver.h"
#include "gui_controller.h"
#include "gui_init_graphic.h"
#include "gui_rendering.h"
#include "project_tools.h"
char menus_buffer[128];

/*************/
/*+ Anchors +*/
/*************/

void shift_to_anchor(int *x, int *y, int w, int h, anchor anc)
{
    switch (anc)
    {
    case ANC_NW:
        break;
    case ANC_NC:
        *x -= w / 2;
        break;
    case ANC_NE:
        *x -= w;
        break;
    case ANC_CW:
        *y -= h / 2;
        break;
    case ANC_CC:
        *x -= w / 2;
        *y -= h / 2;
        break;
    case ANC_CE:
        *x -= w;
        *y -= h / 2;
        break;
    case ANC_SW:
        *y -= h;
        break;
    case ANC_SC:
        *x -= w / 2;
        *y -= h;
        break;
    case ANC_SE:
        *x -= w;
        *y -= h;
        break;
    default:
        break;
    }
}

SDL_Rect var_uiboxes[BX_SIZE];

int var_button_countdown = 0;
button var_selected_button = BT_NONE;
bool var_buttons_flags[BT_SIZE];

button var_pushed_button = BT_NONE;

void var_reset_buttons(void)
{
    for (int i = 0; i < BT_SIZE; i++)
    {
        var_buttons_flags[i] = false;
    }
}

void var_push_button(button bt)
{
    var_pushed_button = bt;
    var_buttons_flags[bt] = true;
    var_button_countdown = SDL_GetTicks();
    set_flag(PUSHED);
}

void var_release_button(button bt)
{
    var_buttons_flags[bt] = false;
}

button_data var_buttons[BT_SIZE];

/******************************************/
/*+ Initialization of frames and buttons +*/
/******************************************/

#define RCOLWIDTH 320

void init_uiboxes(void)
{

    // The screen box
    var_uiboxes[BX_SCREEN].x = 0;
    var_uiboxes[BX_SCREEN].y = 0;
    var_uiboxes[BX_SCREEN].w = var_screen_width;
    var_uiboxes[BX_SCREEN].h = var_screen_height;

    uint hud_base = 8;

    // The hud boxes

    var_uiboxes[BX_PUZZLE].x = var_screen_width - RCOLWIDTH - 16;
    var_uiboxes[BX_PUZZLE].y = 8;
    var_uiboxes[BX_PUZZLE].w = RCOLWIDTH;
    var_uiboxes[BX_PUZZLE].h = 182;

    var_uiboxes[BX_LEVEL].x = var_screen_width - RCOLWIDTH - 16;
    var_uiboxes[BX_LEVEL].y = hud_base;
    var_uiboxes[BX_LEVEL].w = RCOLWIDTH;
    var_uiboxes[BX_LEVEL].h = 72;

    hud_base += var_uiboxes[BX_LEVEL].h + 8;

    var_uiboxes[BX_SCORE].x = var_screen_width - RCOLWIDTH - 16;
    var_uiboxes[BX_SCORE].y = hud_base;
    var_uiboxes[BX_SCORE].w = RCOLWIDTH;
    var_uiboxes[BX_SCORE].h = 102;

    hud_base += var_uiboxes[BX_SCORE].h + 8;

    var_uiboxes[BX_GRAVITY].x = var_screen_width - RCOLWIDTH - 16;
    var_uiboxes[BX_GRAVITY].y = hud_base;
    var_uiboxes[BX_GRAVITY].w = RCOLWIDTH;
    var_uiboxes[BX_GRAVITY].h = 360;

    hud_base += var_uiboxes[BX_GRAVITY].h + 8;

    var_uiboxes[BX_DETONATE].x = var_screen_width - RCOLWIDTH - 16;
    var_uiboxes[BX_DETONATE].y = hud_base;
    var_uiboxes[BX_DETONATE].w = RCOLWIDTH;
    var_uiboxes[BX_DETONATE].h = 222 + 62;

    // The test mode box
    // hud_base += var_uiboxes[BX_DETONATE].h + 8;
    int test_base = 8;

    var_uiboxes[BX_TESTREGEN].x = var_screen_width - 2 * RCOLWIDTH - 32;
    var_uiboxes[BX_TESTREGEN].y = test_base;
    var_uiboxes[BX_TESTREGEN].w = RCOLWIDTH;
    var_uiboxes[BX_TESTREGEN].h = 464;

    test_base += var_uiboxes[BX_TESTREGEN].h + 8;

    var_uiboxes[BX_TESTPARSCLASSIC].x = var_screen_width - 2 * RCOLWIDTH - 32;
    var_uiboxes[BX_TESTPARSCLASSIC].y = test_base;
    var_uiboxes[BX_TESTPARSCLASSIC].w = RCOLWIDTH;
    var_uiboxes[BX_TESTPARSCLASSIC].h = 282;

    var_uiboxes[BX_TESTPARSPUZZLE].x = var_screen_width - 2 * RCOLWIDTH - 32;
    var_uiboxes[BX_TESTPARSPUZZLE].y = test_base;
    var_uiboxes[BX_TESTPARSPUZZLE].w = RCOLWIDTH;
    var_uiboxes[BX_TESTPARSPUZZLE].h = 120;

    // Save/Load
    var_uiboxes[BX_FILENAME].x = (var_screen_width - 768) / 2;
    var_uiboxes[BX_FILENAME].y = (var_screen_height - 128) / 2 - 64;

    var_uiboxes[BX_FILENAME].w = 768;
    var_uiboxes[BX_FILENAME].h = 128;

    // Score
    var_uiboxes[BX_SCORESCREEN].x = 256 / 2;
    var_uiboxes[BX_SCORESCREEN].y = 256 / 2 - 64;

    var_uiboxes[BX_SCORESCREEN].w = var_screen_width - 256;
    var_uiboxes[BX_SCORESCREEN].h = var_screen_height - 256 + 64;

    // Puzzle summary
    var_uiboxes[BX_PUZZLESUMMARY].x = 1024 / 2;
    var_uiboxes[BX_PUZZLESUMMARY].y = 768 / 2 - 64;
    var_uiboxes[BX_PUZZLESUMMARY].w = var_screen_width - 1024;
    var_uiboxes[BX_PUZZLESUMMARY].h = var_screen_height - 768 + 64;

    var_uiboxes[BX_PUZZLESUMMARYLARGE].x = 1024 / 2;
    var_uiboxes[BX_PUZZLESUMMARYLARGE].y = 768 / 2 - 64;
    var_uiboxes[BX_PUZZLESUMMARYLARGE].w = var_screen_width - 1024;
    var_uiboxes[BX_PUZZLESUMMARYLARGE].h = var_screen_height - 768 + 128;

    // Game setup
    var_uiboxes[BX_CLDIMENSIONS].x = var_screen_width / 2 - 560 - 16;
    var_uiboxes[BX_CLDIMENSIONS].y = 128;
    var_uiboxes[BX_CLDIMENSIONS].w = 560;
    var_uiboxes[BX_CLDIMENSIONS].h = 144;

    var_uiboxes[BX_CLPARAMS].x = var_screen_width / 2 - 560 - 16;
    var_uiboxes[BX_CLPARAMS].y = var_uiboxes[BX_CLDIMENSIONS].y + var_uiboxes[BX_CLDIMENSIONS].h + 16;
    var_uiboxes[BX_CLPARAMS].w = 560;
    var_uiboxes[BX_CLPARAMS].h = 504;

    var_uiboxes[BX_CLPARAMSP].x = var_uiboxes[BX_CLPARAMS].x;
    var_uiboxes[BX_CLPARAMSP].y = var_uiboxes[BX_CLPARAMS].y;
    var_uiboxes[BX_CLPARAMSP].w = var_uiboxes[BX_CLPARAMS].w;
    var_uiboxes[BX_CLPARAMSP].h = 444;

    var_uiboxes[BX_CLCONTROL].x = var_screen_width / 2 + 16;
    var_uiboxes[BX_CLCONTROL].y = 128;
    var_uiboxes[BX_CLCONTROL].w = 560;
    var_uiboxes[BX_CLCONTROL].h = 196;

    var_uiboxes[BX_CLCHOICES].x = var_screen_width / 2 + 16;
    var_uiboxes[BX_CLCHOICES].y = var_uiboxes[BX_CLCONTROL].y + var_uiboxes[BX_CLCONTROL].h + 16;
    var_uiboxes[BX_CLCHOICES].w = 560;
    var_uiboxes[BX_CLCHOICES].h = 114;
}

#define SBUTTON_BWIDTH 60
#define SBUTTON_HEIGHT 32
#define LBUTTON_BWIDTH 80
#define LBUTTON_HEIGHT 48
#define TEXTEDIT_HEIGHT 48

void init_one_button(button bt, int x, int y, int w, int h, anchor a, uibox pbox)
{
    var_buttons[bt].parent_box = pbox;
    var_buttons[bt].x = x;
    var_buttons[bt].y = y;
    var_buttons[bt].w = w;
    var_buttons[bt].h = h;
    shift_to_anchor(&var_buttons[bt].x, &var_buttons[bt].y, w, h, a);
    var_buttons[bt].x += var_uiboxes[pbox].x;
    var_buttons[bt].y += var_uiboxes[pbox].y;
    var_buttons[bt].up = bt;
    var_buttons[bt].left = bt;
    var_buttons[bt].down = bt;
    var_buttons[bt].right = bt;
}

void init_boolean_toggle(button bt, int x, int y, uibox pbox)
{
    var_buttons[bt].x = x;
    var_buttons[bt].y = y;
    var_buttons[bt].w = 32;
    var_buttons[bt].h = 32;
    var_buttons[bt].x += var_uiboxes[pbox].x;
    var_buttons[bt].y += var_uiboxes[pbox].y;
    var_buttons[bt].up = bt;
    var_buttons[bt].left = bt;
    var_buttons[bt].down = bt;
    var_buttons[bt].right = bt;
    var_buttons[bt].parent_box = pbox;
}

void init_double_choice_buttons(button lb, button rb, int x, int y, int tw, anchor a, uibox pbox)
{
    int h = 32;
    int w = 2 * tw + 64; // 32 for each side

    var_buttons[lb].x = x;
    var_buttons[lb].y = y;
    shift_to_anchor(&var_buttons[lb].x, &var_buttons[lb].y, w, h, a);
    var_buttons[lb].parent_box = pbox;
    var_buttons[lb].x += var_uiboxes[pbox].x;
    var_buttons[lb].y += var_uiboxes[pbox].y;
    var_buttons[lb].w = 32;
    var_buttons[lb].h = 32;
    var_buttons[lb].up = lb;
    var_buttons[lb].right = rb;
    var_buttons[lb].down = lb;
    var_buttons[lb].left = lb;

    var_buttons[rb].x = x + w / 2;
    var_buttons[rb].y = y;
    shift_to_anchor(&var_buttons[rb].x, &var_buttons[rb].y, w, h, a);
    var_buttons[rb].parent_box = pbox;
    var_buttons[rb].x += var_uiboxes[pbox].x;
    var_buttons[rb].y += var_uiboxes[pbox].y;
    var_buttons[rb].w = 32;
    var_buttons[rb].h = 32;
    var_buttons[rb].up = rb;
    var_buttons[rb].right = rb;
    var_buttons[rb].down = rb;
    var_buttons[rb].left = lb;
}

void init_triple_choice_buttons(button lb, button mb, button rb, int x, int y, int tw, anchor a, uibox pbox)
{
    int h = 32;
    int w = 3 * tw + 96; // 32 for each side

    var_buttons[lb].x = x;
    var_buttons[lb].y = y;
    shift_to_anchor(&var_buttons[lb].x, &var_buttons[lb].y, w, h, a);
    var_buttons[lb].parent_box = pbox;
    var_buttons[lb].x += var_uiboxes[pbox].x;
    var_buttons[lb].y += var_uiboxes[pbox].y;
    var_buttons[lb].w = 32;
    var_buttons[lb].h = 32;
    var_buttons[lb].up = lb;
    var_buttons[lb].right = mb;
    var_buttons[lb].down = lb;
    var_buttons[lb].left = lb;

    var_buttons[mb].x = x + 32 + tw;
    var_buttons[mb].y = y;
    shift_to_anchor(&var_buttons[mb].x, &var_buttons[mb].y, w, h, a);
    var_buttons[mb].parent_box = pbox;
    var_buttons[mb].x += var_uiboxes[pbox].x;
    var_buttons[mb].y += var_uiboxes[pbox].y;
    var_buttons[mb].w = 32;
    var_buttons[mb].h = 32;
    var_buttons[mb].up = mb;
    var_buttons[mb].right = rb;
    var_buttons[mb].down = lb;
    var_buttons[mb].left = mb;

    var_buttons[rb].x = x + 64 + tw * 2;
    var_buttons[rb].y = y;
    shift_to_anchor(&var_buttons[rb].x, &var_buttons[rb].y, w, h, a);
    var_buttons[rb].parent_box = pbox;
    var_buttons[rb].x += var_uiboxes[pbox].x;
    var_buttons[rb].y += var_uiboxes[pbox].y;
    var_buttons[rb].w = 32;
    var_buttons[rb].h = 32;
    var_buttons[rb].up = rb;
    var_buttons[rb].right = rb;
    var_buttons[rb].down = rb;
    var_buttons[rb].left = mb;
}

void init_selector_buttons(button lb, button rb, int x, int y, int tw, anchor a, uibox pbox)
{
    int h = 32;
    int w = tw + 64; // 32 for each side

    var_buttons[lb].x = x;
    var_buttons[lb].y = y;
    shift_to_anchor(&var_buttons[lb].x, &var_buttons[lb].y, w, h, a);
    var_buttons[lb].parent_box = pbox;
    var_buttons[lb].x += var_uiboxes[pbox].x;
    var_buttons[lb].y += var_uiboxes[pbox].y;
    var_buttons[lb].w = 32;
    var_buttons[lb].h = 32;
    var_buttons[lb].up = lb;
    var_buttons[lb].right = rb;
    var_buttons[lb].down = lb;
    var_buttons[lb].left = lb;

    var_buttons[rb].x = x + 32 + tw;
    var_buttons[rb].y = y;
    shift_to_anchor(&var_buttons[rb].x, &var_buttons[rb].y, w, h, a);
    var_buttons[rb].parent_box = pbox;
    var_buttons[rb].x += var_uiboxes[pbox].x;
    var_buttons[rb].y += var_uiboxes[pbox].y;
    var_buttons[rb].w = 32;
    var_buttons[rb].h = 32;
    var_buttons[rb].up = rb;
    var_buttons[rb].right = rb;
    var_buttons[rb].down = rb;
    var_buttons[rb].left = lb;
}

void init_editor_button(button bt, int x, int y, int lw, int tw, anchor a, uibox pbox)
{
    int w = lw + tw;
    shift_to_anchor(&x, &y, w, TEXTEDIT_HEIGHT, a);
    var_buttons[bt].t_x = x + var_uiboxes[pbox].x;
    x += lw;
    var_buttons[bt].parent_box = pbox;
    var_buttons[bt].x = x + var_uiboxes[pbox].x;
    var_buttons[bt].y = y + var_uiboxes[pbox].y;
    var_buttons[bt].w = tw;
    var_buttons[bt].h = TEXTEDIT_HEIGHT;
    var_buttons[bt].up = BT_NONE;
    var_buttons[bt].right = BT_NONE;
    var_buttons[bt].down = BT_NONE;
    var_buttons[bt].left = BT_NONE;
}

// Main menu
#define MAIN_TOP_BT 310
#define MAIN_STEP 75    // Separation between buttons
#define MAIN_BWIDTH 280 // Button width
#define MAIN_BHEIGHT 58 // Button height

#define HUD_BHEIGHT 46
#define HUD_BWIDTH 276

#define CHECKSKIP 40 // Check box width
#define MENUSKIP 60

void init_all_buttons(void)
{

    init_one_button(BT_GAME, var_screen_width >> 1, MAIN_TOP_BT, MAIN_BWIDTH, MAIN_BHEIGHT, ANC_CC, BX_SCREEN);
    init_one_button(BT_PUZZLE, var_screen_width >> 1, MAIN_TOP_BT + MAIN_STEP, MAIN_BWIDTH, MAIN_BHEIGHT, ANC_CC, BX_SCREEN);
    init_one_button(BT_LOAD, var_screen_width >> 1, MAIN_TOP_BT + 2 * MAIN_STEP, MAIN_BWIDTH, MAIN_BHEIGHT, ANC_CC, BX_SCREEN);
    init_one_button(BT_TEST, var_screen_width >> 1, MAIN_TOP_BT + 3 * MAIN_STEP, MAIN_BWIDTH, MAIN_BHEIGHT, ANC_CC, BX_SCREEN);
    init_one_button(BT_SCORESCREEN, var_screen_width >> 1, MAIN_TOP_BT + 4 * MAIN_STEP, MAIN_BWIDTH, MAIN_BHEIGHT, ANC_CC, BX_SCREEN);
    init_one_button(BT_QUIT, var_screen_width >> 1, MAIN_TOP_BT + 5 * MAIN_STEP, MAIN_BWIDTH, MAIN_BHEIGHT, ANC_CC, BX_SCREEN);
    // Main menu
    for (int bt = BT_GAME; bt <= BT_QUIT; bt++)
    {
        var_buttons[bt].up = bt == BT_GAME ? BT_QUIT : (button)(bt - 1);
        var_buttons[bt].right = bt;
        var_buttons[bt].down = bt == BT_QUIT ? BT_GAME : (button)(bt + 1);
        var_buttons[bt].left = bt;
    }

    // Hud

    init_one_button(BT_GRAVITY, RCOLWIDTH >> 1, 315, HUD_BWIDTH, HUD_BHEIGHT, ANC_CC, BX_GRAVITY);
    init_one_button(BT_GUP, RCOLWIDTH >> 1, 71, 52, 52, ANC_CC, BX_GRAVITY);
    init_one_button(BT_GRIGHT, 81 + (RCOLWIDTH >> 1), 152, 52, 52, ANC_CC, BX_GRAVITY);
    init_one_button(BT_GDOWN, RCOLWIDTH >> 1, 233, 52, 52, ANC_CC, BX_GRAVITY);
    init_one_button(BT_GLEFT, -81 + (RCOLWIDTH >> 1), 152, 52, 52, ANC_CC, BX_GRAVITY);

    init_one_button(BT_DETONATE, RCOLWIDTH >> 1, 54, HUD_BWIDTH, HUD_BHEIGHT, ANC_CC, BX_DETONATE);
    init_one_button(BT_SOLVER, RCOLWIDTH >> 1, 54, HUD_BWIDTH, HUD_BHEIGHT, ANC_CC, BX_DETONATE);
    init_selector_buttons(BT_SOLCL, BT_SOLCR, RCOLWIDTH >> 1, 112, 160, ANC_CC, BX_DETONATE);
    init_one_button(BT_HINT, RCOLWIDTH >> 1, 54 + HUD_BHEIGHT + 16, HUD_BWIDTH, HUD_BHEIGHT, ANC_CC, BX_DETONATE);
    init_one_button(BT_SAVE, RCOLWIDTH >> 1, 54 + 2 * HUD_BHEIGHT + 32, HUD_BWIDTH, HUD_BHEIGHT, ANC_CC, BX_DETONATE);
    init_one_button(BT_MENU, RCOLWIDTH >> 1, 54 + 3 * HUD_BHEIGHT + 48, HUD_BWIDTH, HUD_BHEIGHT, ANC_CC, BX_DETONATE);
    // var_buttons[BT_GRAVITY].y = 372;

    for (int bt = BT_GRAVITY; bt <= BT_MENU; bt++)
    {
        var_buttons[bt].up = bt == BT_GRAVITY ? BT_MENU : (button)(bt - 1);
        var_buttons[bt].down = bt == BT_MENU ? BT_GRAVITY : (button)(bt + 1);
    }

    // Test mode
    int tcheck = 24;
    init_double_choice_buttons(BT_SELECMODEL, BT_SELECMODER, var_uiboxes[BX_TESTPARSCLASSIC].w >> 1, tcheck, 100, ANC_NC, BX_TESTPARSCLASSIC);
    int lcheck = var_buttons[BT_SELECMODEL].x - var_uiboxes[BX_TESTPARSCLASSIC].x;

    tcheck += CHECKSKIP;
    init_boolean_toggle(BT_UPGRADES, lcheck, tcheck, BX_TESTPARSCLASSIC);
    tcheck += CHECKSKIP;
    init_boolean_toggle(BT_NODELAY, lcheck, tcheck, BX_TESTPARSCLASSIC);
    tcheck += CHECKSKIP;
    init_boolean_toggle(BT_EASY, lcheck, tcheck, BX_TESTPARSCLASSIC);
    tcheck += CHECKSKIP;
    init_boolean_toggle(BT_WRAP, lcheck, tcheck, BX_TESTPARSCLASSIC);
    tcheck += CHECKSKIP;
    init_boolean_toggle(BT_BOMB, lcheck, tcheck, BX_TESTPARSCLASSIC);

    tcheck = 20;
    init_editor_button(BT_TESTWIDTH, 16, tcheck, 48, 90, ANC_NW, BX_TESTREGEN);
    init_editor_button(BT_TESTHEIGHT, var_uiboxes[BX_TESTREGEN].w / 2, tcheck, 48, 90, ANC_NW, BX_TESTREGEN);
    tcheck += TEXTEDIT_HEIGHT + 16;
    init_one_button(BT_TESTGENCLASSIC, var_uiboxes[BX_TESTREGEN].w / 2, tcheck, HUD_BWIDTH, HUD_BHEIGHT, ANC_NC, BX_TESTREGEN);
    tcheck += HUD_BHEIGHT + 16;
    init_one_button(BT_TESTGENPUZZLE, var_uiboxes[BX_TESTREGEN].w / 2, tcheck, HUD_BWIDTH, HUD_BHEIGHT, ANC_NC, BX_TESTREGEN);
    tcheck += HUD_BHEIGHT + 8;
    init_boolean_toggle(BT_TESTBALANCED, lcheck, tcheck, BX_TESTREGEN);
    tcheck += CHECKSKIP;
    init_double_choice_buttons(BT_TESTSTDUMMY, BT_TESTSTNONE, var_uiboxes[BX_TESTREGEN].w >> 1, tcheck + 8, 100, ANC_NC, BX_TESTREGEN);
    // init_boolean_toggle(BT_TESTSTORAGE, lcheck, tcheck, BX_TESTREGEN);
    tcheck += HUD_BHEIGHT + 8;
    init_double_choice_buttons(BT_TESTSTGEMS, BT_TESTSTBOMBS, var_uiboxes[BX_TESTREGEN].w >> 1, tcheck, 100, ANC_NC, BX_TESTREGEN);
    tcheck += TEXTEDIT_HEIGHT - 4;
    init_editor_button(BT_TESTSTINIT, 160, tcheck, 48, 90, ANC_NW, BX_TESTREGEN);
    tcheck += HUD_BHEIGHT + 16;
    init_one_button(BT_TESTLOAD, var_uiboxes[BX_TESTREGEN].w / 2, tcheck, HUD_BWIDTH, HUD_BHEIGHT, ANC_NC, BX_TESTREGEN);

    // Save/Load
    init_editor_button(BT_FILENAME, var_uiboxes[BX_FILENAME].w / 2, 60, 0, 720, ANC_NC, BX_FILENAME);

    // Scores
    init_one_button(BT_SCOREBACK, var_uiboxes[BX_SCORESCREEN].w / 2, var_uiboxes[BX_SCORESCREEN].h - 96, 256, 64, ANC_NC, BX_SCORESCREEN);
    init_one_button(BT_SCORECONTINUE, var_uiboxes[BX_SCORESCREEN].w / 2 - 128 - 8, var_uiboxes[BX_SCORESCREEN].h - 96, 256, 64, ANC_NE, BX_SCORESCREEN);
    init_one_button(BT_SCORESAVE, var_uiboxes[BX_SCORESCREEN].w / 2, var_uiboxes[BX_SCORESCREEN].h - 96, 256, 64, ANC_NC, BX_SCORESCREEN);
    init_one_button(BT_SCOREMENU, var_uiboxes[BX_SCORESCREEN].w / 2 + 128 + 8, var_uiboxes[BX_SCORESCREEN].h - 96, 256, 64, ANC_NW, BX_SCORESCREEN);

    int total_width = SC_SIZE * (SCORE_BT_SEP + SCORE_BT_WIDTH) + SCORE_NAME_WIDTH;
    int start = var_uiboxes[BX_SCORESCREEN].w / 2 - total_width / 2;
    start += SCORE_NAME_WIDTH + SCORE_BT_SEP;

    init_one_button(BT_SCGLOBAL, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCLEVEL, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCSWAPS, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCCONSUMED, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCFLAMED, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCSTARRED, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCHYPER, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCGRAVITY, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);
    start += SCORE_BT_WIDTH + SCORE_BT_SEP;
    init_one_button(BT_SCTIME, start, SCORE_BT_TOP - 48, SCORE_BT_WIDTH, 48, ANC_SW, BX_SCORESCREEN);

    init_one_button(BT_PREVSCORE, var_uiboxes[BX_SCORESCREEN].w / 2 - 64, var_uiboxes[BX_SCORESCREEN].h - 176, 220, 48, ANC_NE, BX_SCORESCREEN);
    init_one_button(BT_NEXTSCORE, var_uiboxes[BX_SCORESCREEN].w / 2 + 64, var_uiboxes[BX_SCORESCREEN].h - 176, 220, 48, ANC_NW, BX_SCORESCREEN);

    // Puzzle menus
    init_one_button(BT_PUZZLERETRY, var_uiboxes[BX_PUZZLESUMMARY].w / 2 - 120, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 204, 48, ANC_CE, BX_PUZZLESUMMARY);
    init_one_button(BT_PUZZLEREGEN, var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 204, 48, ANC_CC, BX_PUZZLESUMMARY);
    init_one_button(BT_PUZZLEQUIT, var_uiboxes[BX_PUZZLESUMMARY].w / 2 + 120, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 204, 48, ANC_CW, BX_PUZZLESUMMARY);
    init_one_button(BT_PUZZLEBACKMENU, var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].h + 16, 204, 48, ANC_CC, BX_PUZZLESUMMARY);
    init_one_button(BT_PUZZLEBACKSOLVER, var_uiboxes[BX_PUZZLESUMMARY].w / 2 + 8, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 300, 48, ANC_CW, BX_PUZZLESUMMARY);
    init_one_button(BT_PUZZLEIMPLEMENT, var_uiboxes[BX_PUZZLESUMMARY].w / 2 - 8, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 300, 48, ANC_CE, BX_PUZZLESUMMARY);

    // Classic menu
    init_one_button(BT_CLASSICCONTINUE, var_uiboxes[BX_PUZZLESUMMARY].w / 2 - 120, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 204, 48, ANC_CE, BX_PUZZLESUMMARY);
    init_one_button(BT_CLASSICRESTART, var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 204, 48, ANC_CC, BX_PUZZLESUMMARY);
    init_one_button(BT_CLASSICQUIT, var_uiboxes[BX_PUZZLESUMMARY].w / 2 + 120, var_uiboxes[BX_PUZZLESUMMARY].h - 48, 204, 48, ANC_CW, BX_PUZZLESUMMARY);

    // Classic setup

    init_editor_button(BT_SETWIDTH, 32, 72, 110, 130, ANC_NW, BX_CLDIMENSIONS);
    init_editor_button(BT_SETHEIGHT, var_uiboxes[BX_CLDIMENSIONS].w / 2 + 12, 72, 110, 130, ANC_NW, BX_CLDIMENSIONS);

    tcheck = 72;

    init_double_choice_buttons(BT_SETSELECMODEL, BT_SETSELECMODER, var_uiboxes[BX_CLPARAMS].w >> 1, tcheck, 220, ANC_NC, BX_CLPARAMS);
    lcheck = var_buttons[BT_SETSELECMODEL].x - var_uiboxes[BX_CLPARAMS].x;

    tcheck += MENUSKIP;
    init_boolean_toggle(BT_SETNODELAY, lcheck, tcheck, BX_CLPARAMS);
    init_boolean_toggle(BT_SETBALANCED, lcheck, tcheck, BX_CLPARAMS);
    tcheck += MENUSKIP;
    init_boolean_toggle(BT_SETEASY, lcheck, tcheck, BX_CLPARAMS);
    init_boolean_toggle(BT_SETUPGRADES, lcheck, tcheck, BX_CLPARAMS);
    tcheck += MENUSKIP;
    init_boolean_toggle(BT_SETWRAP, lcheck, tcheck, BX_CLPARAMS);
    tcheck += MENUSKIP;
    init_triple_choice_buttons(BT_SETSTOREREFILL, BT_SETSTOREBOMB, BT_SETSTORENONE, var_uiboxes[BX_CLPARAMS].w >> 1, tcheck, 136, ANC_NC, BX_CLPARAMS);
    // init_double_choice_buttons(BT_SETSTOREL, BT_SETSTORER, var_uiboxes[BX_CLPARAMS].w >> 1, tcheck, 220, ANC_NC, BX_CLPARAMS);
    init_boolean_toggle(BT_SETBOMB, lcheck, tcheck, BX_CLPARAMS);
    tcheck += MENUSKIP;
    init_editor_button(BT_SETCLEAR, lcheck, tcheck, 320, 190, ANC_NW, BX_CLPARAMS);
    init_editor_button(BT_SETINITIAL, lcheck, tcheck, 320, 190, ANC_NW, BX_CLPARAMS);
    tcheck += MENUSKIP;
    init_editor_button(BT_SETCLEARMULT, lcheck, tcheck, 320, 190, ANC_NW, BX_CLPARAMS);
    tcheck += MENUSKIP;

    tcheck = 72;

    init_double_choice_buttons(BT_SETCTL, BT_SETCTR, var_uiboxes[BX_CLCONTROL].w >> 1, tcheck, 220, ANC_NC, BX_CLCONTROL);
    init_triple_choice_buttons(BT_SETGRAVITYL, BT_SETGRAVITYC, BT_SETGRAVITYR, var_uiboxes[BX_CLCONTROL].w >> 1, tcheck, 136, ANC_NC, BX_CLCONTROL);
    tcheck += MENUSKIP;
    init_selector_buttons(BT_SETIAL, BT_SETIAR, var_uiboxes[BX_CLCONTROL].w >> 1, tcheck, 320, ANC_NC, BX_CLCONTROL);

    init_one_button(BT_SETSTART, var_uiboxes[BX_CLCHOICES].w / 4 + 8 + 30, 32, 190, MAIN_BHEIGHT, ANC_NC, BX_CLCHOICES);
    init_one_button(BT_SETMENU, 3 * (var_uiboxes[BX_CLCHOICES].w) / 4 - 8 + 30, 32, 190, MAIN_BHEIGHT, ANC_NC, BX_CLCHOICES);

    var_buttons[BT_SETSTART].up = BT_SETSTART;
    var_buttons[BT_SETSTART].right = BT_SETMENU;
    var_buttons[BT_SETSTART].down = BT_SETSTART;
    var_buttons[BT_SETSTART].left = BT_SETSTART;

    var_buttons[BT_SETMENU].up = BT_SETSTART;
    var_buttons[BT_SETMENU].right = BT_SETMENU;
    var_buttons[BT_SETMENU].down = BT_SETSTART;
    var_buttons[BT_SETMENU].left = BT_SETSTART;
    return;
}

bool mouse_capture_button(int x, int y, button b)
{
    if (x < var_buttons[b].x || x > var_buttons[b].x + var_buttons[b].w)
    {
        return false;
    }
    if (y < var_buttons[b].y || y > var_buttons[b].y + var_buttons[b].h)
    {
        return false;
    }
    var_push_button(b);
    set_flag(PUSHED);
    return true;
}

/*****************/
/*+ Render text +*/
/*****************/

// Affichage de texte
int render_text(int x, int y, const char *text, TTF_Font *font, SDL_Color textcol, anchor anc)
{
    if (font == Lunera28)
    {
        y -= 7;
    }
    else if (font == Lunera36)
    {
        y -= 8;
    }
    else if (font == Noto)
    {
        y -= 8;
    }

    SDL_Surface *surf_text = TTF_RenderUTF8_Blended(font, text, textcol);
    if (!surf_text)
    {
        fprintf(stderr, "Error creating surface for text: %s\n", TTF_GetError());
        return 0;
    }
    SDL_Texture *text_text = SDL_CreateTextureFromSurface(renderer, surf_text);
    SDL_Rect pos;
    shift_to_anchor(&x, &y, surf_text->w, surf_text->h, anc);
    pos.x = x;
    pos.y = y;
    pos.h = surf_text->h;
    pos.w = surf_text->w;
    SDL_RenderCopy(renderer, text_text, NULL, &pos);
    SDL_FreeSurface(surf_text);
    SDL_DestroyTexture(text_text);
    return pos.w;
}

// Affichage de texte
int render_text_truncate(int x, int y, int max_width, const char *text, TTF_Font *font, SDL_Color textcol, anchor anc)
{

    int len = strlen(text);
    if (max_width <= 0 || len <= max_width)
    {
        return render_text(x, y, text, font, textcol, anc);
    }

    char truncated[15];
    strncpy(truncated, text, sizeof(truncated) - 1);
    truncated[sizeof(truncated) - 4] = '.';
    truncated[sizeof(truncated) - 3] = '.';
    truncated[sizeof(truncated) - 2] = '.';
    truncated[sizeof(truncated) - 1] = '\0';
    return render_text(x, y, truncated, font, textcol, anc);
}

void print_text_buffer_center(void)
{
    if (SDL_GetTicks() % 800 <= 400)
    {
        sprintf(menus_buffer, "%s_", var_editbuffer->content);
    }
    else
    {
        sprintf(menus_buffer, "%s   ", var_editbuffer->content);
    }
}

/***********************************/
/*+ Displaying frames and buttons +*/
/***********************************/

void display_button(button b, const char *label, TTF_Font *font, ui_color color, ui_type type, SDL_Color textcol)
{

    render_color_frame(var_buttons[b].x, var_buttons[b].y, var_buttons[b].w, var_buttons[b].h, color, type, var_buttons_flags[b], ANC_NW);

    int x = var_buttons[b].x + var_buttons[b].w / 2;
    int y = var_buttons[b].y + var_buttons[b].h / 2;
    if (var_buttons_flags[b])
    {
        y += 8; // Offset for pressed buttons
    }
    if (font == Lunera24)
    {
        y -= 7;
    }

    render_text(x, y, label, font, textcol, ANC_CC);

    if (var_selected_button == b)
    {
        SDL_Rect pos;
        pos.h = 64;
        pos.w = 64;
        pos.x = var_buttons[b].x - pos.w - 4;
        pos.y = var_buttons[b].y - 16;
        SDL_RenderCopy(renderer, txt_cursors, &clp_cursors[CUR_RHAND_SMALL], &pos);
    }
}

void display_boolean_toggle(button bt, const char *text, bool checked, ui_color color)
{
    SDL_Rect pos;
    pos.h = 32;
    pos.w = 32;
    pos.x = var_buttons[bt].x;
    pos.y = var_buttons[bt].y;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_misc_small[checked ? MISC_SBOX_GOK : MISC_SBOX_GEMPTY], &pos);
    render_text(pos.x + 40, pos.y + 12, text, Lunera28, WHITE, ANC_NW);
}

void display_one_choice_button(button bt, const char *text, bool checked, ui_color color)
{
    SDL_Rect pos;
    pos.h = 32;
    pos.w = 32;
    pos.x = var_buttons[bt].x;
    pos.y = var_buttons[bt].y;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_misc_small[checked ? MISC_SBOX_GSTOP : MISC_SBOX_GEMPTY], &pos);
    render_text(pos.x + 40, pos.y + 12, text, Lunera28, WHITE, ANC_NW);
}

void display_double_choice_buttons(button lb, const char *ltext, button rb, const char *rtext, const char *label, bool left, ui_color color)
{
    display_one_choice_button(lb, ltext, left, color);
    display_one_choice_button(rb, rtext, !left, color);

    if (label)
    {
        render_text(var_buttons[lb].x, var_buttons[lb].y - 44, label, Lunera28, WHITE, ANC_NW);
    }
}

void display_triple_choice_buttons(button lb, const char *ltext, button mb, const char *mtext, button rb, const char *rtext, const char *label, int n, ui_color color)
{
    display_one_choice_button(lb, ltext, n == 0, color);
    display_one_choice_button(mb, mtext, n == 1, color);
    display_one_choice_button(rb, rtext, n == 2, color);

    if (label)
    {
        render_text(var_buttons[lb].x, var_buttons[lb].y - 44, label, Lunera28, WHITE, ANC_NW);
    }
}

void display_selector_buttons(button lb, button rb, const char *text, ui_color color)
{

    SDL_Rect pos;
    pos.h = 32;
    pos.w = 32;
    pos.x = var_buttons[lb].x;
    pos.y = var_buttons[lb].y;
    SDL_RenderCopy(renderer, var_buttons_flags[lb] ? txt_ui[COL_GRAY] : txt_ui[color], &clp_misc_small[MISC_ARROW_WEST], &pos);
    render_text((var_buttons[lb].x + 32 + var_buttons[rb].x) / 2, pos.y + 10, text, Lunera28, WHITE, ANC_NC);

    pos.x = var_buttons[rb].x;
    pos.y = var_buttons[rb].y;
    SDL_RenderCopy(renderer, var_buttons_flags[rb] ? txt_ui[COL_GRAY] : txt_ui[color], &clp_misc_small[MISC_ARROW_EAST], &pos);
}

void display_editor_button(button bt, const char *label, const char *string, ui_color color, SDL_Color label_color, SDL_Color edit_color)
{

    render_color_frame(var_buttons[bt].x, var_buttons[bt].y, var_buttons[bt].w, var_buttons[bt].h, color, UITYPE_PLAIN, true, ANC_NW);
    render_text(var_buttons[bt].x + var_buttons[bt].w / 2, var_buttons[bt].y + 20, string, Lunera28, edit_color, ANC_NC);
    render_text(var_buttons[bt].x - 8, var_buttons[bt].y + 20, label, Lunera28, label_color, ANC_NE);
}

void render_ui_box_advanced(uibox box, ui_advframe frame)
{
    int x = var_uiboxes[box].x;
    int y = var_uiboxes[box].y;
    int w = var_uiboxes[box].w;
    int h = var_uiboxes[box].h;
    render_advanced_frame(x, y, w, h, frame, ANC_NW);
}

void render_color_frame(int x, int y, int w, int h, ui_color color, ui_type type, bool on, anchor anc)
{

    h = MAX(h, 32);
    w = MAX(w, 32);

    shift_to_anchor(&x, &y, w, h, anc);

    if (!on)
    {
        y -= 8;
    }

    SDL_Rect pos;

    pos.y = y;

    int midh = h - 32;
    int midw = w - 32;

    // Top line
    pos.x = x;
    pos.y = y;
    pos.h = 16;
    pos.w = 16;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][UICA_NW], &pos);
    pos.x += pos.w;
    if (midw > 0)
    {
        pos.w = midw;
        SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][UICA_N], &pos);
        pos.x += midw;
    }
    pos.w = 16;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][UICA_NE], &pos);

    // Middle line
    pos.x = x;
    pos.y += 16;
    pos.h = midh;
    pos.w = 16;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][UICA_W], &pos);
    pos.x += pos.w;
    if (midw > 0)
    {
        pos.w = midw;
        SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][UICA_C], &pos);
        pos.x += midw;
    }
    pos.w = 16;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][UICA_E], &pos);

    // Bottom line
    pos.x = x;
    pos.y += midh;
    pos.h = on ? 16 : 24;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][on ? UICA_SW : UICA_SW_OFF], &pos);
    pos.x += pos.w;
    if (midw > 0)
    {
        pos.w = midw;
        SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][on ? UICA_S : UICA_S_OFF], &pos);
        pos.x += midw;
    }
    pos.w = 16;
    SDL_RenderCopy(renderer, txt_ui[color], &clp_frames[type][on ? UICA_SE : UICA_SE_OFF], &pos);
}

static void render_ui_box_color(uibox box, ui_color color, ui_type type)
{
    int x = var_uiboxes[box].x;
    int y = var_uiboxes[box].y;
    int w = var_uiboxes[box].w;
    int h = var_uiboxes[box].h;
    render_color_frame(x, y, w, h, color, type, true, ANC_NW);
}

void render_advanced_frame(int x, int y, int w, int h, ui_advframe frame, anchor anc)
{
    h = MAX(h, 64);
    w = MAX(w, 64);

    shift_to_anchor(&x, &y, w, h, anc);

    SDL_Rect pos;
    pos.y = y;
    int midh = h - 64;
    int midw = w - 64;

    // Top line
    pos.x = x;
    pos.y = y;
    pos.h = 32;
    pos.w = 32;
    SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_NW], &pos);
    pos.x += pos.w;
    if (midw > 0)
    {
        pos.w = midw;
        SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_N], &pos);
        pos.x += midw;
    }
    pos.w = 32;
    SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_NE], &pos);

    // Middle line
    pos.x = x;
    pos.y += 32;
    pos.h = midh;
    pos.w = 32;
    SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_W], &pos);
    pos.x += pos.w;
    if (midw > 0)
    {
        pos.w = midw;
        SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_C], &pos);
        pos.x += midw;
    }
    pos.w = 32;
    SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_E], &pos);

    // Bottom line
    pos.x = x;
    pos.y += midh;
    pos.h = 32;
    SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_SW], &pos);
    pos.x += pos.w;
    if (midw > 0)
    {
        pos.w = midw;
        SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_S], &pos);
        pos.x += midw;
    }
    pos.w = 32;
    SDL_RenderCopy(renderer, txt_advanced, &clp_advanced[frame][UICA_SE], &pos);
}

/***************/
/*+ Main menu +*/
/***************/

void render_mainmenu(void)
{
    int y = 120;
    render_color_frame(var_screen_width / 2, y, 860, 110, COL_RED, UITYPE_SHADE, true, ANC_CC);
    // render_frame(var_screen_width / 2, y, 860, 100, NULL, ANC_CC);
    render_text(var_screen_width / 2, y - 12, "Algorithms and gems", Diamond, WHITE, ANC_CC);

    display_button(BT_GAME, "Classic game", Lunera36, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_PUZZLE, "Puzzle game", Lunera36, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_LOAD, "Load game", Lunera36, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_TEST, "Test mode", Lunera36, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_SCORESCREEN, "Scores", Lunera36, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_QUIT, "Quit", Lunera36, COL_RED, UITYPE_SHADE, WHITE);

    if (var_high_scores)
    {
        render_score_screen(2);
    }

    if (var_edit_mode == ED_LOAD)
    {
        render_textbox_menu("Enter the filename to load a game.", ADV_DARK_GREY, WHITE, COL_RED, WHITE);
    }
}

/******************/
/*+ Score screen +*/
/******************/

void render_score_screen(short mode)
{

    if (mode < 0 || mode > 2)
    {
        fprintf(stderr, "Invalid score screen mode: %d\n", mode);
        exit(EXIT_FAILURE);
    }

    int middle = var_uiboxes[BX_SCORESCREEN].x + var_uiboxes[BX_SCORESCREEN].w / 2;

    render_ui_box_color(BX_SCORESCREEN, COL_YELLOW, UITYPE_EDGE);

    // Top message on the score screen.
    switch (mode)
    {
    case 0:
        sprintf(menus_buffer, "Level %d cleared!", var_game->score->level - 1);
        display_button(BT_SCORECONTINUE, "Continue", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
        display_button(BT_SCORESAVE, "Save and Exit", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
        display_button(BT_SCOREMENU, "Exit", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
        break;
    case 1:
        sprintf(menus_buffer, "No more moves left! Final level: %d.", var_game->score->level);
        display_button(BT_SCORECONTINUE, "Continue", Lunera28, COL_GRAY, UITYPE_SHADE, BLACK);
        display_button(BT_SCORESAVE, "Save and Exit", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
        display_button(BT_SCOREMENU, "Exit", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
        break;
    case 2:
        sprintf(menus_buffer, "High scores");
        display_button(BT_SCOREBACK, "Close", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
        break;
    default:
        break;
    }

    render_text(var_uiboxes[BX_SCORESCREEN].x + var_uiboxes[BX_SCORESCREEN].w / 2, var_uiboxes[BX_SCORESCREEN].y + 48, menus_buffer, Lunera36, BLACK, ANC_NC);

    if (!var_score_table)
    {
        render_text(middle, SCORE_WI_TOP + 50, "Scores are not implemented", Lunera28, BLACK, ANC_CC);
        return;
    }

    display_button(BT_SCGLOBAL, "Score", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCLEVEL, "Levels", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCSWAPS, "Swaps", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCCONSUMED, "Gems", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCFLAMED, "Flames", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCSTARRED, "Stars", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCHYPER, "Hypers", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCGRAVITY, "Gravity", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    display_button(BT_SCTIME, "Time", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);

    if (var_scores_page > 0)
    {
        display_button(BT_PREVSCORE, "Previous Page", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    }
    else
    {
        display_button(BT_PREVSCORE, "Previous Page", Lunera24, COL_GRAY, UITYPE_PLAIN, BLACK);
    }

    sprintf(menus_buffer, "%d/%d", var_scores_page + 1, var_scores_max_page);
    render_text(middle, var_uiboxes[BX_SCORESCREEN].y + var_uiboxes[BX_SCORESCREEN].h - 172, menus_buffer, Lunera24, BLACK, ANC_NC);
    if (var_scores_page < var_scores_max_page - 1)
    {
        display_button(BT_NEXTSCORE, "Next Page", Lunera24, COL_YELLOW, UITYPE_PLAIN, BLACK);
    }
    else
    {
        display_button(BT_NEXTSCORE, "Next Page", Lunera24, COL_GRAY, UITYPE_PLAIN, BLACK);
    }

    int top = SCORE_NB_TOP;

    int total_width = SC_SIZE * (SCORE_BT_SEP + SCORE_BT_WIDTH) + SCORE_NAME_WIDTH;

    if (var_scores_page >= var_scores_max_page || var_scores_page < 0)
    {
        var_scores_page = 0; // Reset page if out of range
    }

    // for (int i = var_score_page * var_scores_per_page; i < MIN(var_score_table->size, (var_score_page + 1) * var_scores_per_page); i++) {
    for (int i = var_scores_page * var_scores_per_page; i < MIN(var_score_table->size, (var_scores_page + 1) * var_scores_per_page); i++)
    {
        int start = middle - (total_width / 2);
        sprintf(menus_buffer, "*%d*", i + 1);
        render_text(start - 10, top, menus_buffer, Lunera20, BLACK, ANC_CE);
        int k = var_score_table->permutation[i];
        if (k == var_score_table->player_index)
        {
            render_text_truncate(start, top, 18, var_score_table->names[k], Lunera20, BLUET, ANC_CW);
        }
        else
        {
            render_text_truncate(start, top, 18, var_score_table->names[k], Lunera20, BLACK, ANC_CW);
        }

        start += SCORE_NAME_WIDTH + SCORE_BT_SEP + SCORE_BT_WIDTH / 2;

        for (int j = 0; j < SC_SIZE - 1; j++)
        {
            sprintf(menus_buffer, "%d", var_score_table->scores[k][j]);
            render_text(start, top, menus_buffer, Lunera20, BLACK, ANC_CC);
            start += SCORE_BT_WIDTH + SCORE_BT_SEP;
        }
        sprintf(menus_buffer, "%dm %ds", var_score_table->scores[k][SC_TIME] / 60, var_score_table->scores[k][SC_TIME] % 60);
        render_text(start, top, menus_buffer, Lunera20, BLACK, ANC_CC);
        top += SCORE_NB_STEP;
    }

    if (var_edit_mode == ED_NAME)
    {
        render_textbox_menu("Enter your name to save your score.", ADV_DARK_BROWN, WHITE, COL_YELLOW, BLACK);
    }
}

/**************/
/*+ Game Hud +*/
/**************/

void render_gravity_wheel(void)
{

    if (!var_game)
    {
        return;
    }

    // Gravity box
    render_ui_box_color(BX_GRAVITY, COL_BLUE, UITYPE_LINE);
    SDL_Rect pos;
    pos.x = var_uiboxes[BX_GRAVITY].x + 32; // var_screen_width - 48;
    pos.y = var_uiboxes[BX_GRAVITY].y + 20; // +80;
    pos.h = clp_circles[CIRCLE_BROWN].h;
    pos.w = clp_circles[CIRCLE_BROWN].w;
    SDL_RenderCopy(renderer, txt_advanced, &clp_circles[CIRCLE_GREY], &pos);
    // pos.h = 192;
    // pos.w = 192;
    // pos.x += 32;
    // pos.y += 32;
    pos.h = 128;
    pos.w = 128;
    pos.x += 64;
    pos.y += 64;

    switch (board_get_gravity(var_game->b))
    {
    case UP:
        SDL_RenderCopy(renderer, txt_bigarrows, &clp_bigarrows[CUR_UP], &pos);
        break;
    case DOWN:
        SDL_RenderCopy(renderer, txt_bigarrows, &clp_bigarrows[CUR_DOWN], &pos);
        break;
    case LEFT:
        SDL_RenderCopy(renderer, txt_bigarrows, &clp_bigarrows[CUR_LEFT], &pos);
        break;
    case RIGHT:
        SDL_RenderCopy(renderer, txt_bigarrows, &clp_bigarrows[CUR_RIGHT], &pos);
        break;
    default:
        break;
    }

    ui_color col = flags[NOPLAYER] ? COL_GRAY : COL_BLUE;

    if (var_game->gravity_control > 0)
    {
        display_button(BT_GRAVITY, "Cycle Gravity", Lunera28, col, UITYPE_SHADE, WHITE);
    }
    else
    {
        display_button(BT_GRAVITY, "Cycle Gravity", Lunera28, COL_GRAY, UITYPE_PLAIN, WHITE);
    }

    if (var_game->gravity_control > 1)
    {
        display_button(BT_GUP, "⇧", Noto, col, UITYPE_SHADE, WHITE);
        display_button(BT_GDOWN, "⇩", Noto, col, UITYPE_SHADE, WHITE);
        display_button(BT_GLEFT, "⇦", Noto, col, UITYPE_SHADE, WHITE);
        display_button(BT_GRIGHT, "⇨", Noto, col, UITYPE_SHADE, WHITE);
    }
    else
    {
        display_button(BT_GUP, "⇧", Noto, COL_GRAY, UITYPE_SHADE, WHITE);
        display_button(BT_GDOWN, "⇩", Noto, COL_GRAY, UITYPE_SHADE, WHITE);
        display_button(BT_GLEFT, "⇦", Noto, COL_GRAY, UITYPE_SHADE, WHITE);
        display_button(BT_GRIGHT, "⇨", Noto, COL_GRAY, UITYPE_SHADE, WHITE);
    }
}

/******************/
/*+ Classic mode +*/
/******************/

static void render_dimensions_box(void)
{
    render_ui_box_advanced(BX_CLDIMENSIONS, ADV_DARK_GREY);

    render_text(var_uiboxes[BX_CLDIMENSIONS].x + var_uiboxes[BX_CLDIMENSIONS].w / 2, var_uiboxes[BX_CLDIMENSIONS].y + 32, "Dimensions of the board", Lunera28, WHITE, ANC_NC);
    if (var_edit_mode == ED_WIDTH)
    {
        print_text_buffer_center();
    }
    else
    {
        sprintf(menus_buffer, "%d", var_game_hsize);
    }
    display_editor_button(BT_SETWIDTH, "Width:", menus_buffer, COL_RED, WHITE, WHITE);

    if (var_edit_mode == ED_HEIGHT)
    {
        print_text_buffer_center();
    }
    else
    {
        sprintf(menus_buffer, "%d", var_game_vsize);
    }
    display_editor_button(BT_SETHEIGHT, "Height:", menus_buffer, COL_RED, WHITE, WHITE);
}

void render_classic_setup(void)
{

    // Dimensions
    render_dimensions_box();

    // Parameters
    render_ui_box_advanced(BX_CLPARAMS, ADV_DARK_GREY);
    render_text(var_uiboxes[BX_CLPARAMS].x + var_uiboxes[BX_CLPARAMS].w / 2, var_uiboxes[BX_CLPARAMS].y + 32, "Game parameters", Lunera28, WHITE, ANC_NC);
    display_double_choice_buttons(BT_SETSELECMODEL, "Lines mode", BT_SETSELECMODER, "SCCs mode", NULL, var_game_lines, COL_RED);
    display_boolean_toggle(BT_SETNODELAY, "No delay mode", var_game_nodelay, COL_RED);
    display_boolean_toggle(BT_SETEASY, "Easy mode", var_game_easy, COL_RED);
    display_boolean_toggle(BT_SETWRAP, "Gravity gems", var_game_wrap, COL_RED);
    display_boolean_toggle(BT_SETBOMB, "Bombs", var_game_bombs, COL_RED);

    if (var_edit_mode == ED_CLEARSCORE)
    {
        print_text_buffer_center();
    }
    else
    {
        sprintf(menus_buffer, "%d", var_game_clearscore);
    }
    display_editor_button(BT_SETCLEAR, "Initial clear score:", menus_buffer, COL_RED, WHITE, WHITE);
    if (var_edit_mode == ED_CLEARMULTI)
    {
        print_text_buffer_center();
    }
    else
    {
        sprintf(menus_buffer, "%d%%", var_game_clearmulti);
    }
    display_editor_button(BT_SETCLEARMULT, "Clear score increase:", menus_buffer, COL_RED, WHITE, WHITE);

    // Control
    render_ui_box_advanced(BX_CLCONTROL, ADV_DARK_GREY);
    render_text(var_uiboxes[BX_CLCONTROL].x + var_uiboxes[BX_CLCONTROL].w / 2, var_uiboxes[BX_CLCONTROL].y + 32, "Game control", Lunera28, WHITE, ANC_NC);

    display_double_choice_buttons(BT_SETCTL, "Player", BT_SETCTR, "IA", NULL, !var_game_ia_menu, COL_RED);
    if (var_game_ia_menu)
    {
        display_selector_buttons(BT_SETIAL, BT_SETIAR, ia_names[var_game_ia_choice_menu], COL_RED);
    }

    // Start
    render_ui_box_advanced(BX_CLCHOICES, ADV_DARK_GREY);

    display_button(BT_SETSTART, "Start game", Lunera28, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_SETMENU, "Menu", Lunera28, COL_RED, UITYPE_SHADE, WHITE);
}

static void render_classic_menu(void)
{
    render_ui_box_color(BX_PUZZLESUMMARY, COL_YELLOW, UITYPE_EDGE);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 48, "Classic game in progress. Statistics:", Lunera36, BLACK, ANC_NC);
    sprintf(menus_buffer, "Swaps: %d      Score: %d", var_game->score->scores[SC_SWAPS], var_game->score->scores[SC_GLOBAL]);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 104, menus_buffer, Lunera28, BLACK, ANC_NC);
    sprintf(menus_buffer, "Time: %dm %ds", var_game->score->scores[SC_TIME] / 60, var_game->score->scores[SC_TIME] % 60);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 152, menus_buffer, Lunera28, BLACK, ANC_NC);
    display_button(BT_CLASSICCONTINUE, "Continue", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_CLASSICRESTART, "Restart", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_CLASSICQUIT, "Menu", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
}

static void render_classic_hud(void)
{
    render_ui_box_color(BX_LEVEL, COL_BLUE, UITYPE_PLAIN);
    sprintf(menus_buffer, "Level: %d", var_game->score->level);
    render_text(var_uiboxes[BX_LEVEL].x + var_uiboxes[BX_LEVEL].w / 2, var_uiboxes[BX_LEVEL].y + 24, menus_buffer, Lunera36, WHITE, ANC_NC);

    render_ui_box_color(BX_SCORE, COL_BLUE, UITYPE_PLAIN);

    sprintf(menus_buffer, "%d", var_game->score->scores[SC_GLOBAL]);
    // Score
    render_text(var_uiboxes[BX_SCORE].x + var_uiboxes[BX_SCORE].w / 2, var_uiboxes[BX_SCORE].y + 24, "Score:", Lunera36, WHITE, ANC_NC);
    render_text(var_uiboxes[BX_SCORE].x + var_uiboxes[BX_SCORE].w / 2, var_uiboxes[BX_SCORE].y + 64, menus_buffer, Lunera36, WHITE, ANC_NC);

    // Other buttons
    render_ui_box_color(BX_DETONATE, COL_BLUE, UITYPE_LINE);
    if (var_game->nodelay || flags[NOPLAYER] || flags[NOGAMEINPUT])
    {
        display_button(BT_DETONATE, "Detonate", Lunera28, COL_GRAY, UITYPE_SHADE, WHITE);
    }
    else
    {
        display_button(BT_DETONATE, "Detonate", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
    }
    if (flags[NOPLAYER] || flags[NOGAMEINPUT])
    {
        display_button(BT_HINT, "Hint", Lunera28, COL_GRAY, UITYPE_SHADE, WHITE);
        display_button(BT_SAVE, "Save", Lunera28, COL_GRAY, UITYPE_SHADE, WHITE);
    }
    else
    {
        display_button(BT_HINT, "Hint", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
        display_button(BT_SAVE, "Save", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
    }

    display_button(BT_MENU, "Menu", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
}

void render_classicmode(void)
{
    render_classic_hud();
    render_gravity_wheel();

    render_progress_bar(var_uiboxes[BX_GRAVITY].x / 2, var_screen_height - 128, ((double)(var_game->score->level_score)) / var_game->score->target_score);

    if (var_current_classic_mode != CLASSIC_GAMEPLAY)
    {
        // Rendering the gray layer.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 127);
        SDL_RenderFillRect(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));

        switch (var_current_classic_mode)
        {
        case CLASSIC_ENDLEVEL:
            render_score_screen(0);
            break;
        case CLASSIC_LOST:
            render_score_screen(1);
            break;
        case CLASSIC_MENU:
            render_classic_menu();
            break;
        default:
            break;
        }
    }
    if (var_edit_mode == ED_SAVE)
    {
        render_textbox_menu("Enter the filename to save the game.", ADV_DARK_GREY, WHITE, COL_RED, WHITE);
    }
}

/*****************/
/*+ Puzzle Mode +*/
/*****************/

void render_puzzle_setup(void)
{

    // Dimensions
    render_dimensions_box();

    // Parameters
    render_ui_box_advanced(BX_CLPARAMSP, ADV_DARK_GREY);
    render_text(var_uiboxes[BX_CLPARAMSP].x + var_uiboxes[BX_CLPARAMSP].w / 2, var_uiboxes[BX_CLPARAMSP].y + 32, "Game parameters", Lunera28, WHITE, ANC_NC);
    display_double_choice_buttons(BT_SETSELECMODEL, "Lines mode", BT_SETSELECMODER, "SCCs mode", NULL, var_game_lines, COL_RED);
    display_boolean_toggle(BT_SETBALANCED, "Balanced start (multiples of 3)", var_game_balanced, COL_RED);
    display_boolean_toggle(BT_SETUPGRADES, "Upgrades", var_game_upgrades, COL_RED);
    display_triple_choice_buttons(BT_SETSTOREREFILL, "Gems", BT_SETSTOREBOMB, "Bombs", BT_SETSTORENONE, "None", "Storage mode:", var_game_refill_mode, COL_RED);

    if (var_game_refill_mode == 1)
    {
        if (var_edit_mode == ED_STORAGEINI)
        {
            print_text_buffer_center();
        }
        else
        {
            sprintf(menus_buffer, "%d", var_game_storage_initial);
        }
        display_editor_button(BT_SETINITIAL, "Storage size:", menus_buffer, COL_RED, WHITE, WHITE);
    }

    // Control
    render_ui_box_advanced(BX_CLCONTROL, ADV_DARK_GREY);

    render_text(var_uiboxes[BX_CLCONTROL].x + var_uiboxes[BX_CLCONTROL].w / 2, var_uiboxes[BX_CLCONTROL].y + 32, "Gravity control", Lunera28, WHITE, ANC_NC);
    display_triple_choice_buttons(BT_SETGRAVITYL, "None", BT_SETGRAVITYC, "Cycle", BT_SETGRAVITYR, "Full", NULL, var_gravity_mode, COL_RED);

    // Start
    render_ui_box_advanced(BX_CLCHOICES, ADV_DARK_GREY);

    display_button(BT_SETSTART, "Start game", Lunera28, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_SETMENU, "Menu", Lunera28, COL_RED, UITYPE_SHADE, WHITE);
}

static void render_puzzle_hud(void)
{
    render_ui_box_color(BX_PUZZLE, COL_BLUE, UITYPE_PLAIN);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w / 2, var_uiboxes[BX_PUZZLE].y + 24, "Puzzle game", Lunera28, WHITE, ANC_NC);
    sprintf(menus_buffer, "%d", var_game->score->scores[SC_SWAPS] + var_game->score->scores[SC_GRAVITY] + var_game->score->refills);
    render_text(var_uiboxes[BX_PUZZLE].x + 16, var_uiboxes[BX_PUZZLE].y + 100, "Moves:", Lunera28, WHITE, ANC_CW);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w - 16, var_uiboxes[BX_PUZZLE].y + 100, menus_buffer, Lunera28, WHITE, ANC_CE);
    sprintf(menus_buffer, "%d", var_game->score->scores[SC_GRAVITY]);
    uint t = get_time_counter() / 1000;
    sprintf(menus_buffer, "%dm %ds", t / 60, t % 60);
    render_text(var_uiboxes[BX_PUZZLE].x + 16, var_uiboxes[BX_PUZZLE].y + 140, "Time:", Lunera28, WHITE, ANC_CW);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w - 16, var_uiboxes[BX_PUZZLE].y + 140, menus_buffer, Lunera28, WHITE, ANC_CE);

    render_ui_box_color(BX_DETONATE, COL_BLUE, UITYPE_LINE);
    display_button(BT_SOLVER, "Solve", Lunera28, flags[NOPLAYER] ? COL_GRAY : COL_BLUE, UITYPE_SHADE, WHITE);
    render_color_frame((var_buttons[BT_SOLCL].x + var_buttons[BT_SOLCR].x + 32) / 2, var_buttons[BT_SOLCL].y + 16, HUD_BWIDTH, HUD_BHEIGHT + 20, COL_GRAY, UITYPE_EDGE, true, ANC_CC);
    switch (var_game_solver_mode)
    {
    case SOLVER_BFS:
        display_selector_buttons(BT_SOLCL, BT_SOLCR, "BFS", COL_BLUE);
        break;
    case SOLVER_DFS:
        display_selector_buttons(BT_SOLCL, BT_SOLCR, "DFS", COL_BLUE);
        break;
    case SOLVER_ASTAR:
        display_selector_buttons(BT_SOLCL, BT_SOLCR, "A*", COL_BLUE);
        break;
    default:
        break;
    }
    display_button(BT_SAVE, "Save", Lunera28, flags[NOPLAYER] ? COL_GRAY : COL_BLUE, UITYPE_SHADE, WHITE);
    display_button(BT_MENU, "Menu", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
}

static void render_puzzle_summary(void)
{
    render_ui_box_color(BX_PUZZLESUMMARY, COL_YELLOW, UITYPE_EDGE);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 48, "Puzzle solved! Congratulations!", Lunera36, BLACK, ANC_NC);
    sprintf(menus_buffer, "Swaps: %d   Refills: %d   Gravity changes: %d", var_game->score->scores[SC_SWAPS], var_game->score->refills, var_game->score->scores[SC_GRAVITY]);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 104, menus_buffer, Lunera28, BLACK, ANC_NC);
    sprintf(menus_buffer, "Time: %dm %ds", var_game->score->scores[SC_TIME] / 60, var_game->score->scores[SC_TIME] % 60);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 152, menus_buffer, Lunera28, BLACK, ANC_NC);
    display_button(BT_PUZZLEQUIT, "Menu", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_PUZZLERETRY, "Retry", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_PUZZLEREGEN, "New puzzle", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
}

static void render_puzzle_solver(void)
{
    render_ui_box_color(BX_PUZZLESUMMARY, COL_YELLOW, UITYPE_EDGE);
    if (var_game_solver_result == SOLVER_SOLVED)
    {
        render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 48, "Solution found!", Lunera36, BLACK, ANC_NC);
        sprintf(menus_buffer, "Number of moves in the solution: %d", var_game_solver_nb_moves);
        render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 152, menus_buffer, Lunera28, BLACK, ANC_NC);
    }
    else if (var_game_solver_result == SOLVER_UNSOLVABLE)
    {
        render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 48, "Unsolvable game (solver exhausted all configurations)", Lunera36, BLACK, ANC_NC);
    }
    else
    {
        render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 48, "No solution found (solver timed out).", Lunera36, BLACK, ANC_NC);
    }

    sprintf(menus_buffer, "Game configurations explored: %d", var_game_solver_nb_configs);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 104, menus_buffer, Lunera28, BLACK, ANC_NC);

    display_button(BT_PUZZLEBACKSOLVER, "Back to Game", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_PUZZLEIMPLEMENT, "Implement solution", Lunera28, var_game_solver_result == SOLVER_SOLVED ? COL_YELLOW : COL_GRAY, UITYPE_SHADE, BLACK);
}

static void render_puzzle_menu(void)
{
    render_ui_box_color(BX_PUZZLESUMMARYLARGE, COL_YELLOW, UITYPE_EDGE);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 48, "Puzzle in progress. Moves so far:", Lunera36, BLACK, ANC_NC);
    sprintf(menus_buffer, "Swaps: %d   Refills: %d   Gravity changes: %d", var_game->score->scores[SC_SWAPS], var_game->score->refills, var_game->score->scores[SC_GRAVITY]);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 104, menus_buffer, Lunera28, BLACK, ANC_NC);
    sprintf(menus_buffer, "Time: %dm %ds", var_game->score->scores[SC_TIME] / 60, var_game->score->scores[SC_TIME] % 60);
    render_text(var_uiboxes[BX_PUZZLESUMMARY].x + var_uiboxes[BX_PUZZLESUMMARY].w / 2, var_uiboxes[BX_PUZZLESUMMARY].y + 152, menus_buffer, Lunera28, BLACK, ANC_NC);
    display_button(BT_PUZZLEQUIT, "Menu", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_PUZZLERETRY, "Retry", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_PUZZLEREGEN, "New puzzle", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
    display_button(BT_PUZZLEBACKMENU, "Back to game", Lunera28, COL_YELLOW, UITYPE_SHADE, BLACK);
}

void render_puzzle(void)
{
    render_puzzle_hud();
    render_gravity_wheel();

    if (var_current_puzzle_mode != PUZZLE_GAMEPLAY)
    {
        // Rendering the gray layer.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 127);
        SDL_RenderFillRect(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));

        switch (var_current_puzzle_mode)
        {
        case PUZZLE_SOLVER:
            render_puzzle_solver();
            break;
        case PUZZLE_SUMMARY:
            render_puzzle_summary();
            break;
        case PUZZLE_MENU:
            render_puzzle_menu();
            break;
        default:
            break;
        }
    }
    if (var_edit_mode == ED_SAVE)
    {
        render_textbox_menu("Enter the filename to save the game.", ADV_DARK_GREY, WHITE, COL_RED, WHITE);
    }
}

/********************/
/*+ Test Mode menu +*/
/********************/

static void render_testmode_hud(void)
{
    render_ui_box_color(BX_PUZZLE, COL_BLUE, UITYPE_PLAIN);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w / 2, var_uiboxes[BX_PUZZLE].y + 24, "Test mode", Lunera28, WHITE, ANC_NC);
    sprintf(menus_buffer, "%d", var_game->score->scores[SC_CONSUMED]);
    render_text(var_uiboxes[BX_PUZZLE].x + 16, var_uiboxes[BX_PUZZLE].y + 80, "Killed:", Lunera28, WHITE, ANC_CW);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w - 16, var_uiboxes[BX_PUZZLE].y + 80, menus_buffer, Lunera28, WHITE, ANC_CE);
    sprintf(menus_buffer, "%d", var_game->score->scores[SC_SWAPS]);
    render_text(var_uiboxes[BX_PUZZLE].x + 16, var_uiboxes[BX_PUZZLE].y + 120, "Swaps:", Lunera28, WHITE, ANC_CW);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w - 16, var_uiboxes[BX_PUZZLE].y + 120, menus_buffer, Lunera28, WHITE, ANC_CE);
    uint t = get_time_counter() / 1000;
    sprintf(menus_buffer, "%dm %ds", t / 60, t % 60);
    render_text(var_uiboxes[BX_PUZZLE].x + 16, var_uiboxes[BX_PUZZLE].y + 160, "Time:", Lunera28, WHITE, ANC_CW);
    render_text(var_uiboxes[BX_PUZZLE].x + var_uiboxes[BX_PUZZLE].w - 16, var_uiboxes[BX_PUZZLE].y + 160, menus_buffer, Lunera28, WHITE, ANC_CE);

    // Other buttons
    render_ui_box_color(BX_DETONATE, COL_BLUE, UITYPE_LINE);

    if (var_game && !var_game->puzzle && !var_game->nodelay)
    {
        display_button(BT_DETONATE, "Detonate", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
    }
    else
    {
        display_button(BT_DETONATE, "Detonate", Lunera28, COL_GRAY, UITYPE_SHADE, WHITE);
    }

    if (var_game && !var_game->puzzle)
    {
        display_button(BT_HINT, "Hint", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
    }
    else
    {
        display_button(BT_HINT, "Hint", Lunera28, COL_GRAY, UITYPE_SHADE, WHITE);
    }

    display_button(BT_SAVE, "Save", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
    display_button(BT_MENU, "Menu", Lunera28, COL_BLUE, UITYPE_SHADE, WHITE);
}

void render_testmodemenu(void)
{
    render_gravity_wheel();
    render_testmode_hud();
    if (var_game && !var_game->puzzle)
    {
        render_ui_box_advanced(BX_TESTPARSCLASSIC, ADV_DARK_GREY);
    }
    else
    {
        render_ui_box_advanced(BX_TESTPARSPUZZLE, ADV_DARK_GREY);
    }
    // render_advanced_frame(var_screen_width - 16, 624, 320, 260, ADV_RED, ANC_NE);
    display_double_choice_buttons(BT_SELECMODEL, "Lines", BT_SELECMODER, "SCCs", NULL, var_game->lines, COL_RED);
    display_boolean_toggle(BT_UPGRADES, "Upgrades", var_game->upgrades, COL_RED);

    if (var_game && !var_game->puzzle)
    {
        display_boolean_toggle(BT_NODELAY, "No kill delay", var_game->nodelay, COL_RED);
        display_boolean_toggle(BT_EASY, "Easy mode", var_game->easy, COL_RED);
        display_boolean_toggle(BT_WRAP, "Gravity gems", var_game->gravity_gems, COL_RED);
        display_boolean_toggle(BT_BOMB, "Bombs", var_game->bombs, COL_RED);
    }
    // display_boolean_toggle(BT_BYPASS, "Bypass gravity", var_bypass_gravity, COL_RED);

    render_ui_box_advanced(BX_TESTREGEN, ADV_DARK_GREY);

    if (var_edit_mode == ED_TESTWIDTH)
    {
        print_text_buffer_center();
    }
    else
    {
        sprintf(menus_buffer, "%d", var_test_game_hsize);
    }
    display_editor_button(BT_TESTWIDTH, "W:", menus_buffer, COL_RED, WHITE, WHITE);

    if (var_edit_mode == ED_TESTHEIGHT)
    {
        print_text_buffer_center();
    }
    else
    {
        sprintf(menus_buffer, "%d", var_test_game_vsize);
    }
    display_editor_button(BT_TESTHEIGHT, "H:", menus_buffer, COL_RED, WHITE, WHITE);

    display_button(BT_TESTGENCLASSIC, "New classic game", Lunera28, COL_RED, UITYPE_SHADE, WHITE);
    display_button(BT_TESTGENPUZZLE, "New puzzle game", Lunera28, COL_RED, UITYPE_SHADE, WHITE);
    display_boolean_toggle(BT_TESTBALANCED, "Balanced start", var_test_game_balanced, COL_RED);

    render_text(var_buttons[BT_TESTSTGEMS].x, var_buttons[BT_TESTSTGEMS].y - 32, "Storage:", Lunera28, WHITE, ANC_NW);
    display_triple_choice_buttons(BT_TESTSTGEMS, "Gems", BT_TESTSTBOMBS, "Bombs", BT_TESTSTNONE, "None", NULL, var_test_game_refill_mode, COL_RED);
    if (var_test_game_refill_mode == 1)
    {
        if (var_edit_mode == ED_TESTSTORAGEINI)
        {
            print_text_buffer_center();
        }
        else
        {
            sprintf(menus_buffer, "%d", var_test_game_storage_initial);
        }
        display_editor_button(BT_TESTSTINIT, "Storage size:", menus_buffer, COL_RED, WHITE, WHITE);
    }
    display_button(BT_TESTLOAD, "Load game", Lunera28, COL_RED, UITYPE_SHADE, WHITE);

    if (var_edit_mode == ED_SAVE)
    {
        render_textbox_menu("Enter the filename to save the game.", ADV_DARK_GREY, WHITE, COL_RED, WHITE);
    }
    else if (var_edit_mode == ED_LOAD)
    {
        render_textbox_menu("Enter the filename to load a game.", ADV_DARK_GREY, WHITE, COL_RED, WHITE);
    }
}

/***************/
/*+ Save/Load +*/
/***************/

void render_textbox_menu(const char *message, ui_advframe backframe, SDL_Color message_color, ui_color box_color, SDL_Color edit_color)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 127);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));

    render_ui_box_advanced(BX_FILENAME, backframe);
    render_text(var_uiboxes[BX_FILENAME].x + var_uiboxes[BX_FILENAME].w / 2, var_uiboxes[BX_FILENAME].y + 32, message, Lunera28, message_color, ANC_NC);
    print_text_buffer_center();
    display_editor_button(BT_FILENAME, " ", menus_buffer, box_color, edit_color, edit_color);
}
