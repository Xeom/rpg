#if ! defined(TEXTBOX_H)
# define TEXTBOX_H

# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>

# include "util.h"

/* Attributes for text in textboxes */
typedef enum
{
    bold   = 1,
    italic = 2,
    under  = 4
} text_attrs;

/* Attributes for textboxes themselves, also for borders */
typedef enum
{
    border_top = 1,
    border_bottom = 2,
    border_left = 4,
    border_right = 8,
    no_margin = 16,
    no_border = 32
} textbox_attrs;

/* border.h requires textbox_attrs, so is included after its definition */
#include "border.h"

typedef struct textbox_s textbox;

struct textbox_s
{
    border      *bord;
    TTF_Font    *font;
    SDL_Surface *surf;
    SDL_Rect     area;

    SDL_Rect     textarea;

    int           vislines;
    int           scroll;
    textbox_attrs attrs;
    int           lnspacing;

    int    numlines;
    char **text;
};

extern float textbox_line_spacing;
extern int   textbox_vert_margin;
extern int   textbox_hori_margin;

int textbox_init(
    textbox *t, border *b, TTF_Font *font, SDL_Surface *surf, SDL_Rect *area
);

void textbox_resize(textbox *t, SDL_Rect *newsize);

void textbox_draw(textbox *t);

#endif /* TEXTBOX_H */
