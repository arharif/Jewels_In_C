/**
 * @file gui_rendering.h
 * @brief
 * Rendering of the game board. DO NOT MODIFY.
 *
 * @attention
 * This part is already implemented. It should not be modified.
 */

#ifndef _GUI_RENDERING_H_
#define _GUI_RENDERING_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <dirent.h>

#include "game.h"
#include "gui_gamevars.h"
#include "gui_init_graphic.h"

/*  ____                _           _              */
/* |  _ \ ___ _ __   __| | ___ _ __(_)_ __   __ _  */
/* | |_) / _ \ '_ \ / _` |/ _ \ '__| | '_ \ / _` | */
/* |  _ <  __/ | | | (_| |  __/ |  | | | | | (_| | */
/* |_| \_\___|_| |_|\__,_|\___|_|  |_|_| |_|\__, | */
/*                                          |___/  */

/*****************************/
/*+ Preliminary functions   **/
/*****************************/

/**
 * @brief Returns a Boolean indicating whether the a cell is outside the screen.
 */
bool render_outofview(int, //!< Horizontal coordinate of the cell in the board.
                      int  //!< Vertical coordinate of the cell in the board.
);

/**
 * @brief
 * Converts the coordinates of a cell in the board to the coordinates on the screen.
 *
 * @remark
 * The coordinates are centered on the cell.
 */
void render_coordinates_screen_to_board(int x,  //!< Horizontal coordinate of the cell in the screen.
                                        int y,  //!< Vertical coordinate of the cell in the screen.
                                        int *i, //!< Pointer to store horizontal coordinate of the cell in the board.
                                        int *j  //!< Pointer to store vertical coordinate of the cell in the board.
);

/************************/
/*+ Displaying objects **/
/************************/

/**
 * @brief
 * Renders an object at a given position on the screen.
 *
 * @remark
 * The object will be centered on the position (x,y) on the screen.
 */
void render_object(int x,         //!< Horizontal coordinate on the screen.
                   int y,         //!< Vertical coordinate on the screen.
                   int zoomlevel, //!< Zoom level to use for rendering (0: small, 1: medium, 2: large).
                   cell *thecell, //!< Specification of the object to render.
                   bool spin,     //!< Whether the content cell is spinning (Works only for unpowered gems and wrapped gems).
                   bool shrink    //!< Whether the content cell is shrinking (shrink kill effect).
);

/**
 * @brief
 * Renders an explosion animation at a given position on the screen.
 *
 * @remark
 * The explosion will be centered on the position (x,y) on the screen.
 */
void render_explode(int x,     //!< Horizontal coordinate on the screen.
                    int y,     //!< Vertical coordinate on the screen.
                    int frame, //!< Frame of the explosion animation.
                    bool ring  //!< Whether to display the ring effect (for the cell starting the explosion).
);

/**
 * @brief
 * Renders a shatter animation at a given position on the screen.
 *
 * @remark
 * The shatter will be centered on the position (x,y) on the screen.
 */
void render_shatter(int x,    //!< Horizontal coordinate on the screen.
                    int y,    //!< Vertical coordinate on the screen.
                    int frame //!< Frame of the shatter animation.
);

/**
 * @brief
 * Renders the frame around a picked cell.
 *
 * @remark
 * The frame will be centered on the cell (i,j) of the board.
 */
void render_picked_frame(int i, //!< Horizontal coordinate of the cell in the board.
                         int j  //!< Vertical coordinate of the cell in the board.
);

/**
 * @brief
 * Render a single cell of the board in idle mode.
 */
void render_cell_idle(game *g, //!< Pointer to the game structure.
                      int i,   //!< Horizontal coordinate of the cell in the board.
                      int j    //!< Vertical coordinate of the cell in the board.
);

/**
 * @brief
 * Render a single cell of the board in movement mode.
 */
void render_cell_move(game *g,       //!< Pointer to the game structure.
                      int i,         //!< Horizontal coordinate of the cell in the board.
                      int j,         //!< Vertical coordinate of the cell in the board.
                      direction dir, //!< Direction of the movement.
                      int distance   //!< Distance of the movement (in number of cells).
);

/**
 * @brief
 * Render a single cell of the board in kill mode.
 */
void render_cell_kill(game *g,          //!< Pointer to the game structure.
                      int i,            //!< Horizontal coordinate of the cell in the board.
                      int j,            //!< Vertical coordinate of the cell in the board.
                      gem_kill killtype //!< Type of the kill effect.
);

/**
 * @brief
 * Renders the background of the game.
 */
void render_background(int w, //!< Width of the board (in number of cells).
                       int h  //!< Height of the board (in number of cells).
);

/**
 * @brief
 * Renders a hint animation at a given position on the screen.
 *
 * @remark
 * The hint will be centered on the position (hinti, hintj) on the board.
 */
void render_hint(int hinti, //!< Horizontal coordinate of the cell in the board.
                 int hintj  //!< Vertical coordinate of the cell in the board.
);

/**
 * @brief
 * Renders the entire game.
 */
void render_game(game *g //!< Pointer to the game structure.
);

/**
 * @brief
 * Renders a progress bar at a given position on the screen.
 *
 * @remark
 * The progress bar will be centered on the position (x,y) on the screen.
 */
void render_progress_bar(int x,          //!< Horizontal coordinate on the screen.
                         int y,          //!< Vertical coordinate on the screen.
                         double progress //!< Progress value (filling of the bar, between 0 and 1)
);

#endif // _GUI_RENDERING_H_
