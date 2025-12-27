#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "gui_init_graphic.h"
#include "gui_menus.h"

/***********/
/*+ Fonts +*/
/***********/

TTF_Font *Lunera18, *Lunera20, *Lunera24, *Lunera28, *Lunera36, *Lunera72, *Diamond, *Noto;
void init_fonts(void)
{
  if (TTF_Init() < 0)
  {
    FATAL("TTF_Init: %s\n", TTF_GetError());
  }

  if (!(Lunera18 = TTF_OpenFont("../Fonts/Lunera.ttf", 18)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Lunera20 = TTF_OpenFont("../Fonts/Lunera.ttf", 20)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Lunera24 = TTF_OpenFont("../Fonts/Lunera.ttf", 24)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Lunera28 = TTF_OpenFont("../Fonts/Lunera.ttf", 28)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Lunera36 = TTF_OpenFont("../Fonts/Lunera.ttf", 36)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Lunera72 = TTF_OpenFont("../Fonts/Lunera.ttf", 72)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }

  if (!(Diamond = TTF_OpenFont("../Fonts/Diamond.ttf", 80)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }
  if (!(Noto = TTF_OpenFont("../Fonts/Noto.ttf", 48)))
  {
    FATAL("TTF_OpenFont: %s\n", TTF_GetError());
  }
}

/*************/
/*+ Sprites +*/
/*************/

void init_image(void)
{
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
  {
    FATAL("IMG_Init: %s\n", IMG_GetError());
  }
}

SDL_Texture *txt_ui[COL_SIZE];
SDL_Texture *txt_cursors;
SDL_Texture *txt_icons;
SDL_Texture *txt_advanced;
SDL_Texture *txt_bigarrows;
SDL_Texture *txt_gems[3][7];
SDL_Texture *txt_gems_shadows[3][7];
SDL_Texture *txt_gems_flames[3][7];
SDL_Texture *txt_gems_stars[3][7];
SDL_Texture *txt_gems_effect_star[3];
SDL_Texture *txt_selector[3];
SDL_Texture *txt_hypercube[3];
SDL_Texture *txt_gems_explosion[3];
SDL_Texture *txt_gems_explosion_circle[3];
SDL_Texture *txt_gems_shatter[3];
SDL_Texture *txt_hint_circle[3];
SDL_Texture *txt_hint_arrows[3];
SDL_Texture *txt_rock;
SDL_Texture *txt_bomb;

SDL_Rect clp_gauges[GAUGE_SIZE];
SDL_Rect clp_misc_large[MISC_SIZE];
SDL_Rect clp_misc_small[MISC_SIZE];
SDL_Rect clp_round_buttons[UITYPE_SIZE];
SDL_Rect clp_square_buttons[UITYPE_SIZE];
SDL_Rect clp_frames[UITYPE_SIZE][UICA_SIZE];
SDL_Rect clp_cursors[CUR_SIZE];
SDL_Rect clp_icons[UICON_SIZE];
SDL_Rect clp_circles[CIRCLE_SIZE];
SDL_Rect clp_advanced[ADV_FRAMES_SIZE][UICA_SW_OFF];
SDL_Rect clp_bigarrows[CUR_CROSS_BIG];
SDL_Rect clp_gems_frames[3][NBFRAMES_GEMS];
SDL_Rect clp_flame_frames[3][NBFRAMES_FLAMES];
SDL_Rect clp_star_frames[3][NBFRAMES_STARS];
SDL_Rect clp_star_effect_frames[3][NBFRAMES_STAREFFECT];
SDL_Rect clp_selector[3];
SDL_Rect clp_hypercube_frames[3][NBFRAMES_HYPERCUBE];
SDL_Rect clp_gems_explosion_frames[3][NBFRAMES_EXPLODE];
SDL_Rect clp_gems_explosion_circle[3];
SDL_Rect clp_gems_shatter_frames[3][NBFRAMES_SHATTER];
SDL_Rect clp_hint_circle[3][NBFRAMES_HINT];
SDL_Rect clp_hint_arrows[3][SIZE_DIRECTION];
SDL_Rect clp_rock;
SDL_Rect clp_bomb[12];

static void load_one_sheet(const char *path, SDL_Texture **texture)
{
  SDL_Surface *image = IMG_Load(path);
  if (!image)
  {
    printf("Error when loading sprites :  %s", IMG_GetError());
    return;
  }
  *texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_FreeSurface(image);
}

void init_load_sprites(void)
{

  //  SDL_Surface* image = NULL;

  load_one_sheet("../Sprites/40_gem_amber.png", &txt_gems[0][AMBER]);
  load_one_sheet("../Sprites/40_gem_amethyst.png", &txt_gems[0][AMETHYST]);
  load_one_sheet("../Sprites/40_gem_diamond.png", &txt_gems[0][DIAMOND]);
  load_one_sheet("../Sprites/40_gem_emerald.png", &txt_gems[0][EMERALD]);
  load_one_sheet("../Sprites/40_gem_ruby.png", &txt_gems[0][RUBY]);
  load_one_sheet("../Sprites/40_gem_sapphire.png", &txt_gems[0][SAPPHIRE]);
  load_one_sheet("../Sprites/40_gem_topaz.png", &txt_gems[0][TOPAZ]);

  load_one_sheet("../Sprites/40_gem_amber_shadow.png", &txt_gems_shadows[0][AMBER]);
  load_one_sheet("../Sprites/40_gem_amethyst_shadow.png", &txt_gems_shadows[0][AMETHYST]);
  load_one_sheet("../Sprites/40_gem_diamond_shadow.png", &txt_gems_shadows[0][DIAMOND]);
  load_one_sheet("../Sprites/40_gem_emerald_shadow.png", &txt_gems_shadows[0][EMERALD]);
  load_one_sheet("../Sprites/40_gem_ruby_shadow.png", &txt_gems_shadows[0][RUBY]);
  load_one_sheet("../Sprites/40_gem_sapphire_shadow.png", &txt_gems_shadows[0][SAPPHIRE]);
  load_one_sheet("../Sprites/40_gem_topaz_shadow.png", &txt_gems_shadows[0][TOPAZ]);

  load_one_sheet("../Sprites/80_gem_amber.png", &txt_gems[1][AMBER]);
  load_one_sheet("../Sprites/80_gem_amethyst.png", &txt_gems[1][AMETHYST]);
  load_one_sheet("../Sprites/80_gem_diamond.png", &txt_gems[1][DIAMOND]);
  load_one_sheet("../Sprites/80_gem_emerald.png", &txt_gems[1][EMERALD]);
  load_one_sheet("../Sprites/80_gem_ruby.png", &txt_gems[1][RUBY]);
  load_one_sheet("../Sprites/80_gem_sapphire.png", &txt_gems[1][SAPPHIRE]);
  load_one_sheet("../Sprites/80_gem_topaz.png", &txt_gems[1][TOPAZ]);

  load_one_sheet("../Sprites/80_gem_amber_shadow.png", &txt_gems_shadows[1][AMBER]);
  load_one_sheet("../Sprites/80_gem_amethyst_shadow.png", &txt_gems_shadows[1][AMETHYST]);
  load_one_sheet("../Sprites/80_gem_diamond_shadow.png", &txt_gems_shadows[1][DIAMOND]);
  load_one_sheet("../Sprites/80_gem_emerald_shadow.png", &txt_gems_shadows[1][EMERALD]);
  load_one_sheet("../Sprites/80_gem_ruby_shadow.png", &txt_gems_shadows[1][RUBY]);
  load_one_sheet("../Sprites/80_gem_sapphire_shadow.png", &txt_gems_shadows[1][SAPPHIRE]);
  load_one_sheet("../Sprites/80_gem_topaz_shadow.png", &txt_gems_shadows[1][TOPAZ]);

  load_one_sheet("../Sprites/160_gem_amber.png", &txt_gems[2][AMBER]);
  load_one_sheet("../Sprites/160_gem_amethyst.png", &txt_gems[2][AMETHYST]);
  load_one_sheet("../Sprites/160_gem_diamond.png", &txt_gems[2][DIAMOND]);
  load_one_sheet("../Sprites/160_gem_emerald.png", &txt_gems[2][EMERALD]);
  load_one_sheet("../Sprites/160_gem_ruby.png", &txt_gems[2][RUBY]);
  load_one_sheet("../Sprites/160_gem_sapphire.png", &txt_gems[2][SAPPHIRE]);
  load_one_sheet("../Sprites/160_gem_topaz.png", &txt_gems[2][TOPAZ]);

  load_one_sheet("../Sprites/160_gem_amber_shadow.png", &txt_gems_shadows[2][AMBER]);
  load_one_sheet("../Sprites/160_gem_amethyst_shadow.png", &txt_gems_shadows[2][AMETHYST]);
  load_one_sheet("../Sprites/160_gem_diamond_shadow.png", &txt_gems_shadows[2][DIAMOND]);
  load_one_sheet("../Sprites/160_gem_emerald_shadow.png", &txt_gems_shadows[2][EMERALD]);
  load_one_sheet("../Sprites/160_gem_ruby_shadow.png", &txt_gems_shadows[2][RUBY]);
  load_one_sheet("../Sprites/160_gem_sapphire_shadow.png", &txt_gems_shadows[2][SAPPHIRE]);
  load_one_sheet("../Sprites/160_gem_topaz_shadow.png", &txt_gems_shadows[2][TOPAZ]);

  for (int i = 0; i < NBFRAMES_GEMS; i++)
  {
    clp_gems_frames[0][i].h = 40;
    clp_gems_frames[0][i].w = 40;
    clp_gems_frames[0][i].x = 40 * (i % 5);
    clp_gems_frames[0][i].y = 40 * (i / 5);
    clp_gems_frames[1][i].h = 80;
    clp_gems_frames[1][i].w = 80;
    clp_gems_frames[1][i].x = 80 * (i % 5);
    clp_gems_frames[1][i].y = 80 * (i / 5);
    clp_gems_frames[2][i].h = 160;
    clp_gems_frames[2][i].w = 160;
    clp_gems_frames[2][i].x = 160 * (i % 5);
    clp_gems_frames[2][i].y = 160 * (i / 5);
  }

  load_one_sheet("../Sprites/40_gem_amber_flame.png", &txt_gems_flames[0][AMBER]);
  load_one_sheet("../Sprites/40_gem_amethyst_flame.png", &txt_gems_flames[0][AMETHYST]);
  load_one_sheet("../Sprites/40_gem_diamond_flame.png", &txt_gems_flames[0][DIAMOND]);
  load_one_sheet("../Sprites/40_gem_emerald_flame.png", &txt_gems_flames[0][EMERALD]);
  load_one_sheet("../Sprites/40_gem_ruby_flame.png", &txt_gems_flames[0][RUBY]);
  load_one_sheet("../Sprites/40_gem_sapphire_flame.png", &txt_gems_flames[0][SAPPHIRE]);
  load_one_sheet("../Sprites/40_gem_topaz_flame.png", &txt_gems_flames[0][TOPAZ]);

  load_one_sheet("../Sprites/80_gem_amber_flame.png", &txt_gems_flames[1][AMBER]);
  load_one_sheet("../Sprites/80_gem_amethyst_flame.png", &txt_gems_flames[1][AMETHYST]);
  load_one_sheet("../Sprites/80_gem_diamond_flame.png", &txt_gems_flames[1][DIAMOND]);
  load_one_sheet("../Sprites/80_gem_emerald_flame.png", &txt_gems_flames[1][EMERALD]);
  load_one_sheet("../Sprites/80_gem_ruby_flame.png", &txt_gems_flames[1][RUBY]);
  load_one_sheet("../Sprites/80_gem_sapphire_flame.png", &txt_gems_flames[1][SAPPHIRE]);
  load_one_sheet("../Sprites/80_gem_topaz_flame.png", &txt_gems_flames[1][TOPAZ]);

  load_one_sheet("../Sprites/160_gem_amber_flame.png", &txt_gems_flames[2][AMBER]);
  load_one_sheet("../Sprites/160_gem_amethyst_flame.png", &txt_gems_flames[2][AMETHYST]);
  load_one_sheet("../Sprites/160_gem_diamond_flame.png", &txt_gems_flames[2][DIAMOND]);
  load_one_sheet("../Sprites/160_gem_emerald_flame.png", &txt_gems_flames[2][EMERALD]);
  load_one_sheet("../Sprites/160_gem_ruby_flame.png", &txt_gems_flames[2][RUBY]);
  load_one_sheet("../Sprites/160_gem_sapphire_flame.png", &txt_gems_flames[2][SAPPHIRE]);
  load_one_sheet("../Sprites/160_gem_topaz_flame.png", &txt_gems_flames[2][TOPAZ]);

  for (int i = 0; i < NBFRAMES_FLAMES; i++)
  {
    clp_flame_frames[0][i].h = 56;
    clp_flame_frames[0][i].w = 56;
    clp_flame_frames[0][i].x = 56 * i;
    clp_flame_frames[0][i].y = 0;
    clp_flame_frames[1][i].h = 112;
    clp_flame_frames[1][i].w = 112;
    clp_flame_frames[1][i].x = 112 * i;
    clp_flame_frames[1][i].y = 0;
    clp_flame_frames[2][i].h = 224;
    clp_flame_frames[2][i].w = 224;
    clp_flame_frames[2][i].x = 224 * i;
    clp_flame_frames[2][i].y = 0;
  }

  load_one_sheet("../Sprites/40_gem_amber_star.png", &txt_gems_stars[0][AMBER]);
  load_one_sheet("../Sprites/40_gem_amethyst_star.png", &txt_gems_stars[0][AMETHYST]);
  load_one_sheet("../Sprites/40_gem_diamond_star.png", &txt_gems_stars[0][DIAMOND]);
  load_one_sheet("../Sprites/40_gem_emerald_star.png", &txt_gems_stars[0][EMERALD]);
  load_one_sheet("../Sprites/40_gem_ruby_star.png", &txt_gems_stars[0][RUBY]);
  load_one_sheet("../Sprites/40_gem_sapphire_star.png", &txt_gems_stars[0][SAPPHIRE]);
  load_one_sheet("../Sprites/40_gem_topaz_star.png", &txt_gems_stars[0][TOPAZ]);

  load_one_sheet("../Sprites/80_gem_amber_star.png", &txt_gems_stars[1][AMBER]);
  load_one_sheet("../Sprites/80_gem_amethyst_star.png", &txt_gems_stars[1][AMETHYST]);
  load_one_sheet("../Sprites/80_gem_diamond_star.png", &txt_gems_stars[1][DIAMOND]);
  load_one_sheet("../Sprites/80_gem_emerald_star.png", &txt_gems_stars[1][EMERALD]);
  load_one_sheet("../Sprites/80_gem_ruby_star.png", &txt_gems_stars[1][RUBY]);
  load_one_sheet("../Sprites/80_gem_sapphire_star.png", &txt_gems_stars[1][SAPPHIRE]);
  load_one_sheet("../Sprites/80_gem_topaz_star.png", &txt_gems_stars[1][TOPAZ]);

  load_one_sheet("../Sprites/160_gem_amber_star.png", &txt_gems_stars[2][AMBER]);
  load_one_sheet("../Sprites/160_gem_amethyst_star.png", &txt_gems_stars[2][AMETHYST]);
  load_one_sheet("../Sprites/160_gem_diamond_star.png", &txt_gems_stars[2][DIAMOND]);
  load_one_sheet("../Sprites/160_gem_emerald_star.png", &txt_gems_stars[2][EMERALD]);
  load_one_sheet("../Sprites/160_gem_ruby_star.png", &txt_gems_stars[2][RUBY]);
  load_one_sheet("../Sprites/160_gem_sapphire_star.png", &txt_gems_stars[2][SAPPHIRE]);
  load_one_sheet("../Sprites/160_gem_topaz_star.png", &txt_gems_stars[2][TOPAZ]);

  for (int i = 0; i < NBFRAMES_STARS; i++)
  {
    clp_star_frames[0][i].h = 60;
    clp_star_frames[0][i].w = 60;
    clp_star_frames[0][i].x = 60 * i;
    clp_star_frames[0][i].y = 0;
    clp_star_frames[1][i].h = 120;
    clp_star_frames[1][i].w = 120;
    clp_star_frames[1][i].x = 120 * i;
    clp_star_frames[1][i].y = 0;
    clp_star_frames[2][i].h = 240;
    clp_star_frames[2][i].w = 240;
    clp_star_frames[2][i].x = 240 * i;
    clp_star_frames[2][i].y = 0;
  }

  load_one_sheet("../Sprites/40_gem_effect_star.png", &txt_gems_effect_star[0]);
  load_one_sheet("../Sprites/80_gem_effect_star.png", &txt_gems_effect_star[1]);
  load_one_sheet("../Sprites/160_gem_effect_star.png", &txt_gems_effect_star[2]);
  SDL_SetTextureBlendMode(txt_gems_effect_star[0], SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(txt_gems_effect_star[0], 160);
  SDL_SetTextureBlendMode(txt_gems_effect_star[1], SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(txt_gems_effect_star[1], 160);
  SDL_SetTextureBlendMode(txt_gems_effect_star[2], SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(txt_gems_effect_star[2], 160);

  for (int i = 0; i < NBFRAMES_STAREFFECT; i++)
  {
    clp_star_effect_frames[0][i].h = 80;
    clp_star_effect_frames[0][i].w = 80;
    clp_star_effect_frames[0][i].x = 80 * i;
    clp_star_effect_frames[0][i].y = 0;
    clp_star_effect_frames[1][i].h = 160;
    clp_star_effect_frames[1][i].w = 160;
    clp_star_effect_frames[1][i].x = 160 * i;
    clp_star_effect_frames[1][i].y = 0;
    clp_star_effect_frames[2][i].h = 320;
    clp_star_effect_frames[2][i].w = 320;
    clp_star_effect_frames[2][i].x = 320 * i;
    clp_star_effect_frames[2][i].y = 0;
  }

  load_one_sheet("../Sprites/40_gem_explode.png", &txt_gems_explosion[0]);
  load_one_sheet("../Sprites/80_gem_explode.png", &txt_gems_explosion[1]);
  load_one_sheet("../Sprites/160_gem_explode.png", &txt_gems_explosion[2]);

  for (int i = 0; i < NBFRAMES_EXPLODE; i++)
  {
    clp_gems_explosion_frames[0][i].h = 64;
    clp_gems_explosion_frames[1][i].h = 128;
    clp_gems_explosion_frames[2][i].h = 256;
    clp_gems_explosion_frames[0][i].w = 64;
    clp_gems_explosion_frames[1][i].w = 128;
    clp_gems_explosion_frames[2][i].w = 256;
    clp_gems_explosion_frames[0][i].x = 64 * i;
    clp_gems_explosion_frames[1][i].x = 128 * i;
    clp_gems_explosion_frames[2][i].x = 256 * i;
    clp_gems_explosion_frames[0][i].y = 0;
    clp_gems_explosion_frames[1][i].y = 0;
    clp_gems_explosion_frames[2][i].y = 0;
  }

  load_one_sheet("../Sprites/40_gem_explode_ring.png", &txt_gems_explosion_circle[0]);
  load_one_sheet("../Sprites/80_gem_explode_ring.png", &txt_gems_explosion_circle[1]);
  load_one_sheet("../Sprites/160_gem_explode_ring.png", &txt_gems_explosion_circle[2]);
  clp_gems_explosion_circle[0].h = 120;
  clp_gems_explosion_circle[0].w = 120;
  clp_gems_explosion_circle[0].x = 0;
  clp_gems_explosion_circle[0].y = 0;
  clp_gems_explosion_circle[1].h = 240;
  clp_gems_explosion_circle[1].w = 240;
  clp_gems_explosion_circle[1].x = 0;
  clp_gems_explosion_circle[1].y = 0;
  clp_gems_explosion_circle[2].h = 480;
  clp_gems_explosion_circle[2].w = 480;
  clp_gems_explosion_circle[2].x = 0;
  clp_gems_explosion_circle[2].y = 0;

  // SDL_SetTextureBlendMode(txt_gems_explosion_circle[0], SDL_BLENDMODE_BLEND);
  // SDL_SetTextureAlphaMod(txt_gems_explosion_circle[0], 220);
  // SDL_SetTextureBlendMode(txt_gems_explosion_circle[1], SDL_BLENDMODE_BLEND);
  // SDL_SetTextureAlphaMod(txt_gems_explosion_circle[1], 220);
  // SDL_SetTextureBlendMode(txt_gems_explosion_circle[2], SDL_BLENDMODE_BLEND);
  // SDL_SetTextureAlphaMod(txt_gems_explosion_circle[2], 220);

  load_one_sheet("../Sprites/40_gem_shatter.png", &txt_gems_shatter[0]);
  load_one_sheet("../Sprites/80_gem_shatter.png", &txt_gems_shatter[1]);
  load_one_sheet("../Sprites/160_gem_shatter.png", &txt_gems_shatter[2]);

  for (int i = 0; i < NBFRAMES_SHATTER; i++)
  {
    clp_gems_shatter_frames[0][i].h = 50;
    clp_gems_shatter_frames[0][i].w = 50;
    clp_gems_shatter_frames[0][i].x = 50 * i;
    clp_gems_shatter_frames[0][i].y = 0;
    clp_gems_shatter_frames[1][i].h = 100;
    clp_gems_shatter_frames[1][i].w = 100;
    clp_gems_shatter_frames[1][i].x = 100 * i;
    clp_gems_shatter_frames[1][i].y = 0;
    clp_gems_shatter_frames[2][i].h = 200;
    clp_gems_shatter_frames[2][i].w = 200;
    clp_gems_shatter_frames[2][i].x = 200 * i;
    clp_gems_shatter_frames[2][i].y = 0;
  }

  load_one_sheet("../Sprites/40_gem_selector.png", &txt_selector[0]);
  load_one_sheet("../Sprites/80_gem_selector.png", &txt_selector[1]);
  load_one_sheet("../Sprites/160_gem_selector.png", &txt_selector[2]);
  clp_selector[0].h = 40;
  clp_selector[0].w = 40;
  clp_selector[0].x = 0;
  clp_selector[0].y = 0;
  clp_selector[1].h = 80;
  clp_selector[1].w = 80;
  clp_selector[1].x = 0;
  clp_selector[1].y = 0;
  clp_selector[2].h = 160;
  clp_selector[2].w = 160;
  clp_selector[2].x = 0;
  clp_selector[2].y = 0;

  load_one_sheet("../Sprites/40_gem_hypercube.png", &txt_hypercube[0]);
  load_one_sheet("../Sprites/80_gem_hypercube.png", &txt_hypercube[1]);
  load_one_sheet("../Sprites/160_gem_hypercube.png", &txt_hypercube[2]);

  for (int i = 0; i < NBFRAMES_HYPERCUBE; i++)
  {
    clp_hypercube_frames[0][i].h = 40;
    clp_hypercube_frames[0][i].w = 38;
    clp_hypercube_frames[0][i].x = 38 * (i % 10);
    clp_hypercube_frames[0][i].y = 40 * (i / 10);
    clp_hypercube_frames[1][i].h = 80;
    clp_hypercube_frames[1][i].w = 76;
    clp_hypercube_frames[1][i].x = 76 * (i % 10);
    clp_hypercube_frames[1][i].y = 80 * (i / 10);
    clp_hypercube_frames[2][i].h = 160;
    clp_hypercube_frames[2][i].w = 152;
    clp_hypercube_frames[2][i].x = 152 * (i % 10);
    clp_hypercube_frames[2][i].y = 160 * (i / 10);
  }

  load_one_sheet("../Sprites/40_hint_circle.png", &txt_hint_circle[0]);
  load_one_sheet("../Sprites/80_hint_circle.png", &txt_hint_circle[1]);
  load_one_sheet("../Sprites/160_hint_circle.png", &txt_hint_circle[2]);

  for (int i = 0; i < NBFRAMES_HINT; i++)
  {
    clp_hint_circle[0][i].h = 84;
    clp_hint_circle[0][i].w = 84;
    clp_hint_circle[0][i].x = 84 * i;
    clp_hint_circle[0][i].y = 0;
    clp_hint_circle[1][i].h = 168;
    clp_hint_circle[1][i].w = 168;
    clp_hint_circle[1][i].x = 168 * i;
    clp_hint_circle[1][i].y = 0;
    clp_hint_circle[2][i].h = 336;
    clp_hint_circle[2][i].w = 336;
    clp_hint_circle[2][i].x = 336 * i;
    clp_hint_circle[2][i].y = 0;
  }

  load_one_sheet("../Sprites/40_hint_arrows.png", &txt_hint_arrows[0]);
  load_one_sheet("../Sprites/80_hint_arrows.png", &txt_hint_arrows[1]);
  load_one_sheet("../Sprites/160_hint_arrows.png", &txt_hint_arrows[2]);

  for (int i = 0; i < SIZE_DIRECTION; i++)
  {
    clp_hint_arrows[0][i].h = 24;
    clp_hint_arrows[0][i].w = 24;
    clp_hint_arrows[0][i].x = 24 * i;
    clp_hint_arrows[0][i].y = 0;
    clp_hint_arrows[1][i].h = 48;
    clp_hint_arrows[1][i].w = 48;
    clp_hint_arrows[1][i].x = 48 * i;
    clp_hint_arrows[1][i].y = 0;
    clp_hint_arrows[2][i].h = 96;
    clp_hint_arrows[2][i].w = 96;
    clp_hint_arrows[2][i].x = 96 * i;
    clp_hint_arrows[2][i].y = 0;
  }

  // Colored UI elements

  load_one_sheet("../Sprites/ui_blue.png", &txt_ui[COL_BLUE]);
  load_one_sheet("../Sprites/ui_green.png", &txt_ui[COL_GREEN]);
  load_one_sheet("../Sprites/ui_grey.png", &txt_ui[COL_GRAY]);
  load_one_sheet("../Sprites/ui_red.png", &txt_ui[COL_RED]);
  load_one_sheet("../Sprites/ui_yellow.png", &txt_ui[COL_YELLOW]);

  // Gauges
  clp_gauges[GAUGE_LEFT].h = 51;
  clp_gauges[GAUGE_LEFT].w = 25;
  clp_gauges[GAUGE_LEFT].x = 0;
  clp_gauges[GAUGE_LEFT].y = 0;
  clp_gauges[GAUGE_CENTER].h = 51;
  clp_gauges[GAUGE_CENTER].w = 1;
  clp_gauges[GAUGE_CENTER].x = 25;
  clp_gauges[GAUGE_CENTER].y = 0;
  clp_gauges[GAUGE_RIGHT].h = 51;
  clp_gauges[GAUGE_RIGHT].w = 25;
  clp_gauges[GAUGE_RIGHT].x = 77;
  clp_gauges[GAUGE_RIGHT].y = 0;

  // Buttons
  for (int i = 0; i < UITYPE_SIZE; i++)
  {
    clp_round_buttons[i].h = 128;
    clp_round_buttons[i].w = 128;
    clp_round_buttons[i].x = i * 256;
    clp_round_buttons[i].y = 115;

    clp_square_buttons[i].h = 128;
    clp_square_buttons[i].w = 128;
    if (i < 2)
    {
      clp_square_buttons[i].x = (UITYPE_SIZE + 2 * i) * 128;
      clp_square_buttons[i].y = 115;
    }
    else
    {
      clp_square_buttons[i].x = (2 * i - 4) * 128;
      clp_square_buttons[i].y = 243;
    }
  }

  // for (int i = 0; i < UITYPE_SIZE; i++) {
  //   clp_round_buttons[i].h = 256;
  //   clp_round_buttons[i].w = 256;
  //   clp_round_buttons[i].x = 2 * i * 256;
  //   clp_round_buttons[i].y = 230;

  //   clp_square_buttons[i].h = 256;
  //   clp_square_buttons[i].w = 256;
  //   if (i < 2) {
  //     clp_square_buttons[i].x = (UITYPE_SIZE + 2 * i) * 256;
  //     clp_square_buttons[i].y = 230;
  //   }
  //   else {
  //     clp_square_buttons[i].x = (2 * i - 4) * 256;
  //     clp_square_buttons[i].y = 486;
  //   }
  // }

  // Frames
  for (int i = 0; i < UITYPE_SIZE; i++)
  {
    for (int j = 0; j < UICA_SIZE; j++)
    {
      clp_frames[i][j].h = 16;
      clp_frames[i][j].w = 16;
    }

    clp_frames[i][UICA_NW].x = 0 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_NW].y = 371 + ((2 * i) / 4) * 128;

    clp_frames[i][UICA_N].x = 16 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_N].y = 371 + ((2 * i) / 4) * 128;
    clp_frames[i][UICA_N].w = 352;

    clp_frames[i][UICA_NE].x = 368 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_NE].y = 371 + ((2 * i) / 4) * 128;

    clp_frames[i][UICA_W].x = 0 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_W].y = 387 + ((2 * i) / 4) * 128;
    clp_frames[i][UICA_W].h = 96;

    clp_frames[i][UICA_C].x = 16 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_C].y = 387 + ((2 * i) / 4) * 128;
    clp_frames[i][UICA_C].w = 352;
    clp_frames[i][UICA_C].h = 96;

    clp_frames[i][UICA_E].x = 368 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_E].y = 387 + ((2 * i) / 4) * 128;
    clp_frames[i][UICA_E].h = 96;

    clp_frames[i][UICA_SW].x = 0 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_SW].y = 483 + ((2 * i) / 4) * 128;

    clp_frames[i][UICA_S].x = 16 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_S].y = 483 + ((2 * i) / 4) * 128;
    clp_frames[i][UICA_S].w = 352;

    clp_frames[i][UICA_SE].x = 368 + ((2 * i) % 4) * 384;
    clp_frames[i][UICA_SE].y = 483 + ((2 * i) / 4) * 128;

    clp_frames[i][UICA_SW_OFF].x = 0 + ((2 * i + 1) % 4) * 384;
    clp_frames[i][UICA_SW_OFF].y = 475 + ((2 * i + 1) / 4) * 128;
    clp_frames[i][UICA_SW_OFF].h = 24;

    clp_frames[i][UICA_S_OFF].x = 16 + ((2 * i + 1) % 4) * 384;
    clp_frames[i][UICA_S_OFF].y = 475 + ((2 * i + 1) / 4) * 128;
    clp_frames[i][UICA_S_OFF].w = 352;
    clp_frames[i][UICA_S_OFF].h = 24;

    clp_frames[i][UICA_SE_OFF].x = 368 + ((2 * i + 1) % 4) * 384;
    clp_frames[i][UICA_SE_OFF].y = 475 + ((2 * i + 1) / 4) * 128;
    clp_frames[i][UICA_SE_OFF].h = 24;
  }

  for (int i = 0; i < MISC_SIZE; i++)
  {
    clp_misc_large[i].h = 64;
    clp_misc_large[i].w = 64;
    clp_misc_large[i].x = i * 64;
    clp_misc_large[i].y = 51;
    clp_misc_small[i].h = 32;
    clp_misc_small[i].w = 32;
    clp_misc_small[i].x = i * 32;
    clp_misc_small[i].y = 755;
  }

  load_one_sheet("../Sprites/ui_cursors.png", &txt_cursors);

  for (int i = 0; i < CUR_SIZE; i++)
  {
    clp_cursors[i].h = 64;
    clp_cursors[i].w = 64;
    clp_cursors[i].x = (i % 8) * 64;
    clp_cursors[i].y = (i / 8) * 64;
  }

  load_one_sheet("../Sprites/ui_icons.png", &txt_icons);
  clp_icons[UICON_UP].h = 50;
  clp_icons[UICON_UP].w = 50;
  clp_icons[UICON_UP].x = 400;
  clp_icons[UICON_UP].y = 0;
  clp_icons[UICON_CROSS].h = 50;
  clp_icons[UICON_CROSS].w = 50;
  clp_icons[UICON_CROSS].x = 400;
  clp_icons[UICON_CROSS].y = 400;

  load_one_sheet("../Sprites/ui_bigarrows.png", &txt_bigarrows);

  for (int i = 0; i < CUR_CROSS_BIG; i++)
  {
    clp_bigarrows[i].h = 192;
    clp_bigarrows[i].w = 192;
    clp_bigarrows[i].x = i * 192;
    clp_bigarrows[i].y = 0;
  }

  load_one_sheet("../Sprites/ui_advanced.png", &txt_advanced);

  for (int i = 0; i < CIRCLE_SIZE; i++)
  {
    clp_circles[i].h = 256;
    clp_circles[i].w = 256;
    clp_circles[i].x = i * 256;
    clp_circles[i].y = 0;
  }

  for (int i = 0; i < ADV_FRAMES_SIZE; i++)
  {
    for (int j = 0; j < UICA_SW_OFF; j++)
    {
      clp_advanced[i][j].h = 32;
      clp_advanced[i][j].w = 32;
    }

    clp_advanced[i][UICA_NW].x = 0 + (i % 6) * 128;
    clp_advanced[i][UICA_NW].y = 256 + (i / 6) * 128;
    clp_advanced[i][UICA_N].x = 32 + (i % 6) * 128;
    clp_advanced[i][UICA_N].y = 256 + (i / 6) * 128;
    clp_advanced[i][UICA_NE].x = 96 + (i % 6) * 128;
    clp_advanced[i][UICA_NE].y = 256 + (i / 6) * 128;

    clp_advanced[i][UICA_W].x = 0 + (i % 6) * 128;
    clp_advanced[i][UICA_W].y = 288 + (i / 6) * 128;
    clp_advanced[i][UICA_C].x = 32 + (i % 6) * 128;
    clp_advanced[i][UICA_C].y = 288 + (i / 6) * 128;
    clp_advanced[i][UICA_E].x = 96 + (i % 6) * 128;
    clp_advanced[i][UICA_E].y = 288 + (i / 6) * 128;

    clp_advanced[i][UICA_SW].x = 0 + (i % 6) * 128;
    clp_advanced[i][UICA_SW].y = 352 + (i / 6) * 128;
    clp_advanced[i][UICA_S].x = 32 + (i % 6) * 128;
    clp_advanced[i][UICA_S].y = 352 + (i / 6) * 128;
    clp_advanced[i][UICA_SE].x = 96 + (i % 6) * 128;
    clp_advanced[i][UICA_SE].y = 352 + (i / 6) * 128;
  }

  // Rock
  load_one_sheet("../Sprites/obj_rock.png", &txt_rock);
  clp_rock.h = 84;
  clp_rock.w = 84;
  clp_rock.x = 0;
  clp_rock.y = 0;

  // Bomb
  load_one_sheet("../Sprites/obj_bomb.png", &txt_bomb);
  clp_bomb[10].h = 84;
  clp_bomb[10].w = 84;
  clp_bomb[10].x = 4;
  clp_bomb[10].y = 39;
  for (int i = 0; i < 10; i++)
  {
    clp_bomb[i].h = 34;
    clp_bomb[i].w = 23;
    clp_bomb[i].x = 23 * i;
    clp_bomb[i].y = 0;
  }
}

/*****************/
/*+ Fenêtre SDL +*/
/*****************/

SDL_Window *window;
SDL_Renderer *renderer;

bool init_SDL(void)
{

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
  {
    FATAL("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
  }

  // Chargement des fonts
  init_fonts();

  // Initialisation des PNGs
  init_image();

  // var_init_maskstab();

  SDL_DisplayMode DM;
  SDL_GetDesktopDisplayMode(0, &DM);
  //  printf("Screen width: %d, Screen height: %d\n", DM.w, DM.h);
  if (var_screen_width <= 0 || DM.w < var_screen_width)
  {
    var_screen_width = DM.w;
  }
  if (var_screen_height <= 0 || DM.h < var_screen_height)
  {
    var_screen_height = DM.h;
  }
  printf("Screen width: %d, Screen height: %d\n", var_screen_width, var_screen_height);

  var_playarea_width = var_screen_width - var_column_width;

  // Création de la fenêtre
  window = SDL_CreateWindow("Jewels in C",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            var_screen_width, var_screen_height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
                                SDL_WINDOW_RESIZABLE);
  if (window == NULL)
  {
    FATAL("Could not create window: %s\n", SDL_GetError());
  }

  SDL_SetWindowResizable(window, SDL_FALSE);

  // Création de l'affichage
  renderer = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED |
                                    SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL)
  {
    FATAL("Could not create renderer: %s\n", SDL_GetError());
  }

  // Initialisation des sprites
  init_load_sprites();

  // Chargement des boutons
  init_uiboxes();
  init_all_buttons();

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, COLOR_TUPLE(WHITE));
  SDL_RenderClear(renderer);

  return true;
}

void shutdown_SDL(void)
{
  if (renderer)
  {
    SDL_DestroyRenderer(renderer);
  }

  if (window)
  {
    SDL_DestroyWindow(window);
  }
  //  var_free_maskstab();
  SDL_Quit();
}
