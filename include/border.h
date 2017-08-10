#if ! defined(BORDER_H)
# define BORDER_H

typedef struct border_s border;

# include "textbox.h"

struct border_s
{
    char *filename;

    int tilew, tileh;

    SDL_Surface *centre;

    SDL_Surface *tl_corner;
    SDL_Surface *tr_corner;
    SDL_Surface *bl_corner;
    SDL_Surface *br_corner;

    SDL_Surface *t_edge;
    SDL_Surface *b_edge;
    SDL_Surface *l_edge;
    SDL_Surface *r_edge;

};

int border_init(border *b, const char *filename);

void border_kill(border *b);

void border_draw(border *b, SDL_Surface *s, SDL_Rect *r, textbox_attrs edges);

#endif /* BORDER_H */
