#if ! defined(DISPLAY_H)
# define DISPLAY_H

# include <SDL2/SDL.h>
/* Things to initialize and manage our SDL window */

extern SDL_Window   *display_window;
extern SDL_Renderer *display_renderer;
extern SDL_Surface  *display_surface;

extern int display_height, display_width;

extern int display_alive;

void display_init(void);
void display_kill(void);
void display_poll(void);

#endif /* DISPLAY_H */
