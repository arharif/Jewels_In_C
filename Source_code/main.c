#include "main.h"
#include "gui_gamevars.h"
#include "project_parameters.h"
#include "project_tools.h"

int main(void)
{
  atexit(shutdown_SDL);

  // Initializing the random number generator.
  srand((unsigned int)(time(NULL)));

  // Initializing the interface dimensions.
  var_screen_height = WINDOW_SCREENHEIGHT;
  var_screen_width = WINDOW_SCREENWIDTH;

  // Initializing the window and graphics.
  init_SDL();

  // Starting the event loop.
  event_loop();

  exit(EXIT_SUCCESS);
}
