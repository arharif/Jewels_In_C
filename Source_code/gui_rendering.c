#include "gui_rendering.h"
#include "gui_controller.h"
#include "gui_gamevars.h"
#include "gui_menus.h"

SDL_Texture *txt_anims;

static SDL_Rect center_pos;

static int SDL_RenderCopyCenter(SDL_Renderer *l_renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)
{
  if (!dstrect)
  {
    return -1;
  }
  center_pos.w = dstrect->w;
  center_pos.h = dstrect->h;
  center_pos.x = dstrect->x - center_pos.w / 2;
  center_pos.y = dstrect->y - center_pos.h / 2;
  return SDL_RenderCopy(l_renderer, texture, srcrect, &center_pos);
}

static int SDL_RenderCopyCenterEx(SDL_Renderer *l_renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle, const SDL_Point *center, const SDL_RendererFlip flip)
{
  if (!dstrect)
  {
    return -1;
  }
  center_pos.w = dstrect->w;
  center_pos.h = dstrect->h;
  center_pos.x = dstrect->x - center_pos.w / 2;
  center_pos.y = dstrect->y - center_pos.h / 2;
  return SDL_RenderCopyEx(l_renderer, texture, srcrect, &center_pos, angle, center, flip);
}

/*****************************/
/*+ Fonctions préliminaires **/
/*****************************/

bool render_outofview(int i, int j)
{
  if (i + 1 < var_startx || j + 1 < var_starty || i - 1 > var_endx || j - 1 > var_endy)
  {
    return true;
  }
  return false;
}

void render_coordinates_screen_to_board(int x, int y, int *i, int *j)
{
  if (i)
  {
    *i = x - var_topleftx - (var_tile_size / 2) + var_startx * var_tile_size;
    if (*i >= 0)
    {
      *i /= var_tile_size;
    }
    else
    {
      *i = (*i / var_tile_size) - 1;
    }
  }
  if (j)
  {
    *j = y - var_toplefty - (var_tile_size / 2) + var_starty * var_tile_size;
    if (*j >= 0)
    {
      *j /= var_tile_size;
    }
    else
    {
      *j = (*j / var_tile_size) - 1;
    }
  }
}

/************************/
/*+ Displaying objects **/
/************************/

static void render_shift_position(int *x, int *y, direction dir, int distance)
{
  switch (dir)
  {
  case UP:
    *y -= distance * var_tile_size / NBFRAMES_MOVE;
    break;
  case DOWN:
    *y += distance * var_tile_size / NBFRAMES_MOVE;
    break;
  case LEFT:
    *x -= distance * var_tile_size / NBFRAMES_MOVE;
    break;
  case RIGHT:
    *x += distance * var_tile_size / NBFRAMES_MOVE;
    break;
  default:
    break;
  }
}

void render_object(int x, int y, int zoomlevel, cell *thecell, bool spin, bool shrink)
{
  if (!thecell)
  {
    fprintf(stderr, "Null cell pointer in render_object\n");
    exit(EXIT_FAILURE);
  }

  SDL_Rect npos;
  npos.h = clp_gems_frames[zoomlevel][0].h;
  npos.w = clp_gems_frames[zoomlevel][0].w;
  npos.x = x;
  npos.y = y;

  // Case of a gem.
  if (thecell->object >= AMBER && thecell->object <= TOPAZ)
  {
    // The spin frame  (used only when the power is GEM_BASIC or GEM_GRAVITY).
    int frame = spin ? var_spin_get_frame() : 0;

    // Case when the gem is shrinking (drops flamed power and star effect).
    if (shrink)
    {
      npos.h -= 4 * (zoomlevel + 1) * var_kill_get_frame();
      npos.w -= 4 * (zoomlevel + 1) * var_kill_get_frame();
      if (thecell->power == GEM_GRAVITY)
      {
        SDL_RenderCopyCenter(renderer, txt_gems_shadows[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
        SDL_RenderCopyCenter(renderer, txt_gems[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
      }
      else
      {
        SDL_RenderCopyCenter(renderer, txt_gems_shadows[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
        SDL_RenderCopyCenter(renderer, txt_gems[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
      }
      return;
    }

    // When the gem is not shrinking, rendering depends on the power of the gem.
    switch (thecell->power)
    {
    case GEM_BASIC:
      SDL_RenderCopyCenter(renderer, txt_gems_shadows[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
      SDL_RenderCopyCenter(renderer, txt_gems[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
      break;
    case GEM_FLAME:
      SDL_RenderCopyCenter(renderer, txt_gems_shadows[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
      npos.h = clp_flame_frames[zoomlevel][0].h;
      npos.w = clp_flame_frames[zoomlevel][0].w;
      npos.y -= var_flamed_yshift;
      SDL_RenderCopyCenter(renderer, txt_gems_flames[zoomlevel][thecell->object], &clp_flame_frames[zoomlevel][var_flame_get_frame()], &npos);
      break;
    case GEM_STAR:
      SDL_RenderCopyCenter(renderer, txt_gems_shadows[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos);
      npos.h = clp_star_frames[zoomlevel][0].h;
      npos.w = clp_star_frames[zoomlevel][0].w;
      SDL_RenderCopyCenter(renderer, txt_gems_stars[zoomlevel][thecell->object], &clp_star_frames[zoomlevel][var_star_get_frame()], &npos);
      npos.h = clp_star_effect_frames[zoomlevel][0].h;
      npos.w = clp_star_effect_frames[zoomlevel][0].w;
      SDL_RenderCopyCenter(renderer, txt_gems_effect_star[zoomlevel], &clp_star_effect_frames[zoomlevel][var_effect_star_get_frame()], &npos);
      break;
    case GEM_GRAVITY:
      SDL_RenderCopyCenterEx(renderer, txt_gems_shadows[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos, -var_wrap_get_frame() * (360 / NBFRAMES_ROTATION), NULL, SDL_FLIP_NONE);
      SDL_RenderCopyCenterEx(renderer, txt_gems[zoomlevel][thecell->object], &clp_gems_frames[zoomlevel][frame], &npos, -var_wrap_get_frame() * (360 / NBFRAMES_ROTATION), NULL, SDL_FLIP_NONE);
      break;
    default:
      break;
    }
    return;
  }

  // Case of a hypercube.
  if (thecell->object == HYPERCUBE)
  {
    npos.w = clp_hypercube_frames[zoomlevel][0].w;
    if (shrink)
    {
      npos.h -= 4 * (zoomlevel + 1) * var_kill_get_frame();
      npos.w -= 4 * (zoomlevel + 1) * var_kill_get_frame();
    }
    SDL_RenderCopyCenter(renderer, txt_hypercube[zoomlevel], &clp_hypercube_frames[zoomlevel][var_hypercube_get_frame()], &npos);
    return;
  }

  if (thecell->object == ROCK)
  {
    if (shrink)
    {
      npos.h -= 4 * (zoomlevel + 1) * var_kill_get_frame();
      npos.w -= 4 * (zoomlevel + 1) * var_kill_get_frame();
    }
    SDL_RenderCopyCenter(renderer, txt_rock, &clp_rock, &npos);
    return;
  }

  if (thecell->object == BOMB)
  {
    if (shrink)
    {
      npos.h -= 4 * (zoomlevel + 1) * var_kill_get_frame();
      npos.w -= 4 * (zoomlevel + 1) * var_kill_get_frame();
      SDL_RenderCopyCenter(renderer, txt_bomb, &clp_bomb[10], &npos);
    }
    else
    {
      SDL_RenderCopyCenter(renderer, txt_bomb, &clp_bomb[10], &npos);
      npos.h /= 2;
      npos.w /= 3;
      if (thecell->countdown < 10)
      {
        int num = thecell->countdown < 0 ? 0 : thecell->countdown;
        SDL_RenderCopyCenter(renderer, txt_bomb, &clp_bomb[num], &npos);
      }
      else
      {
        npos.x -= npos.w / 2;
        SDL_RenderCopyCenter(renderer, txt_bomb, &clp_bomb[thecell->countdown / 10], &npos);
        npos.x += npos.w;
        SDL_RenderCopyCenter(renderer, txt_bomb, &clp_bomb[thecell->countdown % 10], &npos);
      }
    }
  }
}

void render_explode(int x, int y, int frame, bool ring)
{
  SDL_Rect pos;
  pos.x = x;
  pos.y = y;
  if (ring && frame <= 12)
  {
    pos.h = (frame + 4) * clp_gems_explosion_circle[var_render_zoom_level].h / 16;
    pos.w = (frame + 4) * clp_gems_explosion_circle[var_render_zoom_level].w / 16;
    SDL_RenderCopyCenter(renderer, txt_gems_explosion_circle[var_render_zoom_level], &clp_gems_explosion_circle[var_render_zoom_level], &pos);
  }
  pos.h = clp_gems_explosion_frames[var_render_zoom_level][frame].h;
  pos.w = clp_gems_explosion_frames[var_render_zoom_level][frame].w;
  SDL_RenderCopyCenter(renderer, txt_gems_explosion[var_render_zoom_level], &clp_gems_explosion_frames[var_render_zoom_level][frame], &pos);
}

void render_shatter(int x, int y, int frame)
{
  SDL_Rect pos;
  pos.x = x;
  pos.y = y;
  pos.h = clp_gems_shatter_frames[var_render_zoom_level][frame].h;
  pos.w = clp_gems_shatter_frames[var_render_zoom_level][frame].w;
  SDL_RenderCopyCenter(renderer, txt_gems_shatter[var_render_zoom_level], &clp_gems_shatter_frames[var_render_zoom_level][frame], &pos);
}

void render_picked_frame(int i, int j)
{
  SDL_Rect pos;
  pos.h = var_tile_size;
  pos.w = var_tile_size;
  pos.x = var_tile_size + (i - var_startx) * var_tile_size + var_topleftx;
  pos.y = var_tile_size + (j - var_starty) * var_tile_size + var_toplefty;
  SDL_RenderCopyCenter(renderer, txt_selector[var_render_zoom_level], &clp_selector[var_render_zoom_level], &pos);
}

void render_cell_idle(game *g, int i, int j)
{

  // If the cell is out of view, we skip rendering it.
  if (render_outofview(i, j))
  {
    return; // Skip rendering if the cell is out of view.
  }

  // Retrieving information on the cell.
  cell thecell;
  board_get_cell(g->b, i, j, &thecell);
  // Rendering the frame around a picked cell (if applicable).
  bool picked = false;
  int ip, jp;
  if (board_get_picked(g->b, &ip, &jp) && ip == i && jp == j)
  {
    picked = true;
    render_picked_frame(i, j);
  }

  // Calculating the position of the cell on the screen.
  int x = var_tile_size + (i - var_startx) * var_tile_size + var_topleftx;
  int y = var_tile_size + (j - var_starty) * var_tile_size + var_toplefty;

  // Rendering the cell.
  render_object(x, y, var_render_zoom_level, &thecell, picked, false);
}

void render_cell_move(game *g, int i, int j, direction dir, int distance)
{

  // Retrieving information on the cell.
  cell thecell;
  board_get_cell(g->b, i, j, &thecell);

  // Shifting the cell to the correct one.
  if (distance - var_cell_movement_done > 1)
  {
    switch (dir)
    {
    case UP:
      j += distance - var_cell_movement_done - 1;
      break;
    case DOWN:
      j -= distance - var_cell_movement_done - 1;
      break;
    case LEFT:
      i += distance - var_cell_movement_done - 1;
      break;
    case RIGHT:
      i -= distance - var_cell_movement_done - 1;
      break;
    default:
      break;
    }
    if (i < -1 || i > board_get_width(g->b) || j < -1 || j > board_get_height(g->b))
    {
      // Out of bounds
      return;
    }
  }

  // If the cell is out of view, we skip rendering it.
  if (render_outofview(i, j))
  {
    return; // Skip rendering if the cell is out of view.
  }

  // Calculating the position of the cell on the screen.
  int x = var_tile_size + (i - var_startx) * var_tile_size + var_topleftx;
  int y = var_tile_size + (j - var_starty) * var_tile_size + var_toplefty;

  // The element is moving and we shift its exact screen position for the animation.
  if (distance - var_cell_movement_done > 0)
  {
    render_shift_position(&x, &y, (dir + 2) % 4, NBFRAMES_MOVE - var_move_get_frame());
  }

  // Rendering the cell.
  render_object(x, y, var_render_zoom_level, &thecell, false, false);
}

void render_cell_kill(game *g, int i, int j, gem_kill killtype)
{

  // If the cell is out of view, we skip rendering it.
  if (render_outofview(i, j))
  {
    return; // Skip rendering if the cell is out of view.
  }

  // Retrieving information on the cell.
  cell thecell;
  board_get_cell(g->b, i, j, &thecell);

  // Calculating the position of the cell on the screen.
  int x = var_tile_size + (i - var_startx) * var_tile_size + var_topleftx;
  int y = var_tile_size + (j - var_starty) * var_tile_size + var_toplefty;

  switch (killtype)
  {
  case KILL_NOKILL:
    render_object(x, y, var_render_zoom_level, &thecell, false, false);
    return;
  case KILL_SHRINK:
    render_object(x, y, var_render_zoom_level, &thecell, false, true);
    return;
  case KILL_EXPLODE:
    render_explode(x, y, var_kill_get_frame() / 2, false);
    return;
  case KILL_EXPLODE_RING:
    render_explode(x, y, var_kill_get_frame() / 2, true);
    return;
  case KILL_SHATTER:
    render_shatter(x, y, var_kill_get_frame() / 2);
    return;
  default:
    break;
  }
}

void render_background(int w, int h)
{
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      if (render_outofview(i, j))
      {
        continue; // Skip rendering if the cell is out of view.
      }
      SDL_Rect pos;
      pos.h = var_tile_size;
      pos.w = var_tile_size;
      pos.x = var_tile_size / 2 + (i - var_startx) * var_tile_size + var_topleftx;
      pos.y = var_tile_size / 2 + (j - var_starty) * var_tile_size + var_toplefty;
      if ((i + j) % 2 == 0)
      {
        SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(GRAY));
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(LIGHT_GRAY));
      }
      SDL_RenderFillRect(renderer, &pos);
      SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
    }
  }
}

void render_hint(int hinti, int hintj)
{
  SDL_Rect pos;
  int x = var_tile_size + (hinti - var_startx) * var_tile_size + var_topleftx;
  int y = var_tile_size + (hintj - var_starty) * var_tile_size + var_toplefty;
  pos.h = clp_hint_circle[var_render_zoom_level]->h;
  pos.w = clp_hint_circle[var_render_zoom_level]->w;
  pos.x = x;
  pos.y = y;
  SDL_RenderCopyCenter(renderer, txt_hint_circle[var_render_zoom_level], &clp_hint_circle[var_render_zoom_level][var_hint_get_frame()], &pos);
  int shift = 2 * (var_render_zoom_level + 1) * (4 + NBFRAMES_HINT - var_hint_get_frame());

  pos.h = clp_hint_arrows[var_render_zoom_level][DOWN].h;
  pos.w = clp_hint_arrows[var_render_zoom_level][DOWN].w;
  pos.y = y - shift;
  SDL_RenderCopyCenter(renderer, txt_hint_arrows[var_render_zoom_level], &clp_hint_arrows[var_render_zoom_level][DOWN], &pos);
  pos.y = y + shift;
  SDL_RenderCopyCenter(renderer, txt_hint_arrows[var_render_zoom_level], &clp_hint_arrows[var_render_zoom_level][UP], &pos);
  pos.y = y;
  pos.x = x + shift;
  SDL_RenderCopyCenter(renderer, txt_hint_arrows[var_render_zoom_level], &clp_hint_arrows[var_render_zoom_level][LEFT], &pos);
  pos.x = x - shift;
  SDL_RenderCopyCenter(renderer, txt_hint_arrows[var_render_zoom_level], &clp_hint_arrows[var_render_zoom_level][RIGHT], &pos);
}

void render_game(game *g)
{
  if (!g)
  {
    return;
  }

  // If we are in hint animation mode, we scroll to the hint if it is out of view.
  int hinti, hintj;
  if (var_anim_type == ANIM_HINT && game_retrieve_hint(g, &hinti, &hintj) && render_outofview(hinti, hintj))
  {
    scroll_to_coords(hinti, hintj);
  }

  // Retrieving the dimensions of the board.
  int w = board_get_width(g->b);
  int h = board_get_height(g->b);

  // Rendering the background.
  render_background(w, h);

  if (ctl_fillbuttons_used && var_game->puzzle && var_game->refill_mode != 2)
  {

    int ytop = var_tile_size + (-1 - var_starty) * var_tile_size + var_toplefty;
    int ybot = var_tile_size + (h - var_starty) * var_tile_size + var_toplefty;
    if (var_render_zoom_level == 0)
    {
      ybot += 8;
    }
    direction gdir = board_get_gravity(g->b);

    bool nonempty = board_refill_size(g->b) > 0;

    for (int i = 0; i < w; i++)
    {
      int x = var_tile_size + (i - var_startx) * var_tile_size + var_topleftx;

      render_color_frame(x, ytop, var_tile_size >> 1, var_tile_size >> 1, nonempty && (gdir == DOWN) ? COL_GREEN : COL_GRAY, UITYPE_SHADE, ctl_fillbuttons[UP][i], ANC_CC);
      render_color_frame(x, ybot, var_tile_size >> 1, var_tile_size >> 1, nonempty && (gdir == UP) ? COL_GREEN : COL_GRAY, UITYPE_SHADE, ctl_fillbuttons[DOWN][i], ANC_CC);
    }

    int xleft = var_tile_size + (-1 - var_startx) * var_tile_size + var_topleftx;
    int xright = var_tile_size + (w - var_startx) * var_tile_size + var_topleftx;
    for (int j = 0; j < h; j++)
    {
      int y = var_tile_size + (j - var_starty) * var_tile_size + var_toplefty;
      render_color_frame(xleft, y, var_tile_size >> 1, var_tile_size >> 1, nonempty && (gdir == RIGHT) ? COL_GREEN : COL_GRAY, UITYPE_SHADE, ctl_fillbuttons[LEFT][j], ANC_CC);
      render_color_frame(xright, y, var_tile_size >> 1, var_tile_size >> 1, nonempty && (gdir == LEFT) ? COL_GREEN : COL_GRAY, UITYPE_SHADE, ctl_fillbuttons[RIGHT][j], ANC_CC);
    }
  }

  // Rendering the elements depending on the animation type.
  switch (var_anim_type)
  {
  case ANIM_IDLE:
  case ANIM_HINT:
    // Idle or hint animation: we render all the elements in idle mode.
    for (int i = 0; i < w; i++)
    {
      for (int j = 0; j < h; j++)
      {
        render_cell_idle(g, i, j);
      }
    }
    break;
  case ANIM_MOVE_BADSWAP:
  case ANIM_MOVE_GOODSWAP:
  case ANIM_MOVE_ROLLBACK:
  case ANIM_MOVE_FALL:
  {
    int distance = 0;
    direction dir = UP;
    // Movement animation: we render all the elements in movement mode.
    for (int i = 0; i < w; i++)
    {
      for (int j = 0; j < h; j++)
      {
        dir = game_get_cell_move(g, i, j, &distance);
        render_cell_move(g, i, j, dir, distance);
      }
    }
  }
  break;
  case ANIM_KILL:
    // Kill animation: we render all the elements in kill mode.
    gem_kill killtype = KILL_NOKILL;
    for (int i = 0; i < w; i++)
    {
      for (int j = 0; j < h; j++)
      {
        killtype = game_get_cell_kill(g, i, j);
        if (killtype != KILL_EXPLODE_RING)
        {
          render_cell_kill(g, i, j, killtype);
        }
      }
    }
    // Second pass to display the rings of explosions above the surrounding gems.
    for (int i = 0; i < w; i++)
    {
      for (int j = 0; j < h; j++)
      {
        killtype = game_get_cell_kill(g, i, j);
        if (killtype == KILL_EXPLODE_RING)
        {
          render_cell_kill(g, i, j, killtype);
        }
      }
    }
  default:
    break;
  }

  if (var_anim_type == ANIM_HINT && game_retrieve_hint(g, &hinti, &hintj))
  {
    render_hint(hinti, hintj);
  }

  // Drawing the storage queue (if there is one).
  if (g->puzzle && g->refill_mode != 2 && board_refill_size(g->b) >= 0)
  {

    int step = 80;
    int x = 88;
    int y = 8;

    render_advanced_frame(x, y, 160, 640, ADV_LIGHT_GREY, ANC_NC);
    cell c;
    y += step;

    for (int i = 0; i < board_refill_size(g->b) && i < 6; i++)
    {
      board_read_refill(g->b, i, &c);
      render_object(x, y, 1, &c, false, false);
      y += step;
    }

    SDL_Rect pos;
    pos.h = 50;
    pos.w = 50;
    pos.x = x;
    pos.y = y;
    if (board_refill_size(g->b) > 6)
    {
      SDL_RenderCopyCenter(renderer, txt_icons, &clp_icons[UICON_UP], &pos);
    }
    else
    {
      SDL_RenderCopyCenter(renderer, txt_icons, &clp_icons[UICON_CROSS], &pos);
    }

    y = 656;
    render_advanced_frame(x, y, 160, 160, ADV_LIGHT_GREY, ANC_NC);

    char buffer[10];
    snprintf(buffer, 10, "%d", board_refill_size(g->b));
    render_text(x, y + 88, buffer, Lunera36, WHITE, ANC_CC);
  }
}

#define PROGRESS_BAR_WIDTH 512
void render_progress_bar(int x, int y, double progress)
{
  if (progress > 1.0)
  {
    progress = 1.0;
  }
  SDL_Rect pos;
  pos.x = x - (PROGRESS_BAR_WIDTH >> 1) - clp_gauges[GAUGE_LEFT].w;
  pos.y = y;
  pos.w = clp_gauges[GAUGE_LEFT].w;
  pos.h = clp_gauges[GAUGE_LEFT].h;
  SDL_RenderCopy(renderer, txt_ui[COL_BLUE], &clp_gauges[GAUGE_LEFT], &pos);
  pos.x += pos.w;
  if (progress >= 0)
  {
    pos.w = PROGRESS_BAR_WIDTH * progress;
    SDL_RenderCopy(renderer, txt_ui[COL_BLUE], &clp_gauges[GAUGE_CENTER], &pos);
  }
  pos.x += pos.w;

  if (progress < 1.0)
  {
    pos.w = PROGRESS_BAR_WIDTH - pos.w;
    SDL_RenderCopy(renderer, txt_ui[COL_GRAY], &clp_gauges[GAUGE_CENTER], &pos);
    pos.x += pos.w;
    pos.w = clp_gauges[GAUGE_RIGHT].w;
    SDL_RenderCopy(renderer, txt_ui[COL_GRAY], &clp_gauges[GAUGE_RIGHT], &pos);
  }
  else
  {
    pos.w = clp_gauges[GAUGE_RIGHT].w;
    SDL_RenderCopy(renderer, txt_ui[COL_BLUE], &clp_gauges[GAUGE_RIGHT], &pos);
  }
}
