#include <stdio.h>
#include <SDL2/SDL.h>

#include "border.h"

static SDL_Surface *border_load_texture(border *b);

int border_init(border *b, const char *filename)
{
    SDL_Surface *loaded;
    SDL_Surface *sections[3][3];
    SDL_Rect     srcrect, dstrect;
    int          xind, yind;
    int          tilew, tileh;

    printf("[i] Loading borders from %s ...\n", filename);

    b->filename = malloc(strlen(filename) + 1);
    strcpy(b->filename, filename);

    loaded = border_load_texture(b);

    if (!loaded)
        return -1;

    tilew = loaded->w / 3;
    tileh = loaded->h / 3;

    b->tilew = tilew;
    b->tileh = tileh;

    srcrect.w = tilew;
    srcrect.h = tileh;

    dstrect.x = 0;
    dstrect.y = 0;

    for (xind = 0; xind < 3; xind++)
    {
        for (yind = 0; yind < 3; yind++)
        {
            SDL_Surface *s;
            srcrect.x = xind * tilew;
            srcrect.y = yind * tileh;

            s = SDL_CreateRGBSurface(0, tilew, tileh, 32, 0, 0, 0, 0);

            SDL_BlitSurface(loaded, &srcrect, s, &dstrect);

            sections[xind][yind] = s;
        }
    }

    b->centre = sections[1][1];

    b->tl_corner = sections[0][0];
    b->tr_corner = sections[2][0];
    b->bl_corner = sections[0][2];
    b->br_corner = sections[2][2];

    b->t_edge    = sections[1][0];
    b->b_edge    = sections[1][2];
    b->l_edge    = sections[0][1];
    b->r_edge    = sections[2][1];

    SDL_FreeSurface(loaded);

    return 0;
}

void border_kill(border *b)
{
    SDL_FreeSurface(b->tl_corner);
    SDL_FreeSurface(b->tr_corner);
    SDL_FreeSurface(b->bl_corner);
    SDL_FreeSurface(b->br_corner);

    SDL_FreeSurface(b->t_edge);
    SDL_FreeSurface(b->b_edge);
    SDL_FreeSurface(b->l_edge);
    SDL_FreeSurface(b->r_edge);

    SDL_FreeSurface(b->centre);

    free(b->filename);
}

static SDL_Surface *border_load_texture(border *b)
{
    SDL_Surface *rtn;

    rtn = SDL_LoadBMP(b->filename);

    if (rtn == NULL)
    {
        fprintf(stderr, "[!] Error loading borders: '%s'\n", SDL_GetError());

    }
    else
        puts("[i] Loaded border texture\n");

    if (rtn && (rtn->w % 3 || rtn->h % 3))
    {
        rtn = NULL;
        fputs("[!] Border texture has no 3-divisible dimensions\n", stderr);
    }

    if (!rtn)
    {
        rtn = SDL_CreateRGBSurface(0, 3, 3, 32, 0, 0, 0, 0);

        if (!rtn)
            fprintf(
                stderr,
                "[!] Could not make placeholder surface: '%s'\n",
                SDL_GetError()
            );

        else
            puts("[i] Created placeholder surface for missing border\n");
    }

    return rtn;
}

void border_draw(border *b, SDL_Surface *s, SDL_Rect *r, border_edge edges)
{
    SDL_Rect srcrect, dstrect;
    SDL_Surface *toblit;
    int xoff, yoff;

    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = b->tilew;
    srcrect.h = b->tileh;
    dstrect.w = 16;
    dstrect.h = 16;

    printf("%d %d\n", b->tilew, b->tileh);

    for (xoff = b->tilew; xoff + b->tilew < r->w; xoff += b->tilew)
    {
        for (yoff = b->tileh; yoff + b->tileh < r->h; yoff += b->tileh)
        {
            SDL_Rect dstrect;

            dstrect.x = r->x + xoff;
            dstrect.y = r->y + yoff;

            SDL_BlitSurface(b->centre, &srcrect, s, &dstrect);
        }
    }

    for (xoff = b->tilew; xoff + b->tilew < r->w; xoff += b->tilew)
    {
        dstrect.x = r->x + xoff;
        dstrect.y = r->y;

        toblit = (edges & border_top) ? (b->t_edge) : (b->centre);
        SDL_BlitSurface(toblit, &srcrect, s, &dstrect);

        dstrect.y = r->y + r->h - b->tileh;

        toblit = (edges & border_bottom) ? (b->b_edge) : (b->centre);
        SDL_BlitSurface(toblit, &srcrect, s, &dstrect);
    }

    for (yoff = b->tilew; yoff + b->tilew < r->w; yoff += b->tilew)
    {
        dstrect.x = r->x;
        dstrect.y = r->y + yoff;

        toblit = (edges & border_left) ? (b->l_edge) : (b->centre);
        SDL_BlitSurface(toblit, &srcrect, s, &dstrect);

        dstrect.x = r->x + r->w - b->tilew;

        toblit = (edges & border_right) ? (b->r_edge) : (b->centre);
        SDL_BlitSurface(toblit, &srcrect, s, &dstrect);
    }

    if ((edges & border_left) && (edges & border_top)) toblit = b->tl_corner;
    else if (edges & border_left)                      toblit = b->l_edge;
    else if (edges & border_top)                       toblit = b->t_edge;
    else                                               toblit = b->centre;

    dstrect.x = r->x;
    dstrect.y = r->y;

    SDL_BlitSurface(toblit, &srcrect, s, &dstrect);

    if ((edges & border_right) && (edges & border_top)) toblit = b->tr_corner;
    else if (edges & border_right)                      toblit = b->r_edge;
    else if (edges & border_top)                        toblit = b->t_edge;
    else                                                toblit = b->centre;

    dstrect.x = r->x + r->w - b->tilew;

    SDL_BlitSurface(toblit, &srcrect, s, &dstrect);

    if ((edges & border_left) && (edges & border_bottom)) toblit = b->bl_corner;
    else if (edges & border_left)                         toblit = b->l_edge;
    else if (edges & border_bottom)                       toblit = b->b_edge;
    else                                                  toblit = b->centre;

    dstrect.x = r->x;
    dstrect.y = r->y + r->h - b->tileh;

    SDL_BlitSurface(toblit, &srcrect, s, &dstrect);

    if ((edges & border_right) && (edges & border_bottom)) toblit = b->br_corner;
    else if (edges & border_right)                         toblit = b->r_edge;
    else if (edges & border_bottom)                        toblit = b->b_edge;
    else                                                   toblit = b->centre;

    dstrect.x = r->x + r->w - b->tilew;

    SDL_BlitSurface(toblit, &srcrect, s, &dstrect);
}
