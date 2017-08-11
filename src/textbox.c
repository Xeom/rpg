#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

#include "util.h"
#include "border.h"
#include "colour.h"

#include "textbox.h"

float textbox_line_spacing = 1;
int textbox_vert_margin = 16;
int textbox_hori_margin = 16;

static SDL_Surface *textbox_draw_line(textbox *t, int lineno);
static SDL_Surface *textbox_draw_seg(
    char **text, col_rgb col, text_attrs attrs, TTF_Font *f
);
static void textbox_process_text_escapes(
    char **text, col_rgb *col, text_attrs *attrs
);
static SDL_Surface *textbox_join_surf_horiz(SDL_Surface *l, SDL_Surface *r);

int textbox_init(
    textbox *t, border *b, TTF_Font *font, SDL_Surface *surf, SDL_Rect *area
)
{
    t->font = font;
    t->surf = surf;
    t->bord = b;
    t->scroll = 0;

    textbox_resize(t, area);

    return 0;
}

void textbox_resize(textbox *t, SDL_Rect *newsize)
{
    float lnspacing;
    memcpy(&(t->area), newsize, sizeof(SDL_Rect));

    lnspacing = (float)TTF_FontHeight(t->font) * textbox_line_spacing;
    t->lnspacing = (int)round(lnspacing);

    if (t->attrs & no_margin)
        memcpy(&(t->textarea), &(t->area.x), sizeof(SDL_Rect));
    else
    {
        t->textarea.x = max(0, t->area.x + textbox_hori_margin);
        t->textarea.y = max(0, t->area.y + textbox_vert_margin);
        t->textarea.w = max(0, t->area.w - textbox_hori_margin * 2);
        t->textarea.h = max(0, t->area.h - textbox_vert_margin * 2);
    }

    t->vislines = max(1, t->textarea.h / t->lnspacing);
}

void textbox_draw(textbox *t)
{
    int linepos;

    if ( ! (t->attrs & no_border) )
        border_draw(t->bord, t->surf, &(t->area), t->attrs);

    for (linepos = 0; linepos < t->vislines; linepos++)
    {
        SDL_Rect linearea, dstrect;
        SDL_Surface *line;

        dstrect.x = t->textarea.x;
        dstrect.y = t->textarea.y + linepos * t->lnspacing;

        if (linepos + t->scroll >= t->numlines) break;

        line = textbox_draw_line(t, linepos + t->scroll);

        linearea.x = 0;
        linearea.y = 0;
        linearea.w = min(t->textarea.w, line->w);
        linearea.h = line->h;

        SDL_BlitSurface(line, &linearea, t->surf, &dstrect);

        SDL_FreeSurface(line);
    }
}

static void textbox_process_text_escapes(
    char **text, col_rgb *col, text_attrs *attrs
)
{
    if (*text == NULL || (*text)[0] == '\0')
    {
        *text = NULL;
        return;
    }

    if ((*text)[0] != '\\') return;

    switch ((*text)[1])
    {
    case 'c':
        if (!str_check_len(*text, 8))
            *text = NULL;
        else
            sscanf(*text, "\\c%2hhx%2hhx%2hhx", &(col->r), &(col->g), &(col->b));
        break;
    }
}

static SDL_Surface *textbox_draw_seg(
    char **text, col_rgb col, text_attrs attrs, TTF_Font *f
)
{
    SDL_Surface *rtn;
    char *nextseg;

    nextseg = strchr(*text, '\\');

    if (nextseg) *nextseg = '\0';
    rtn = TTF_RenderUTF8_Blended(f, *text, colour_to_sdl(col));
    if (nextseg) *nextseg = '\\';

    *text = nextseg;

    return rtn;
}


static SDL_Surface *textbox_join_surf_horiz(SDL_Surface *l, SDL_Surface *r)
{
    SDL_Surface *rtn;
    SDL_Rect     dstrect;

    rtn = surface_alpha(l->w + r->w, max(l->h, r->h));

    SDL_SetSurfaceBlendMode(l, SDL_BLENDMODE_NONE);
    SDL_SetSurfaceBlendMode(r, SDL_BLENDMODE_NONE);

    dstrect.y = 0;
    dstrect.x = 0;
    SDL_BlitSurface(l, NULL, rtn, &dstrect);
    dstrect.x = l->w;
    SDL_BlitSurface(r, NULL, rtn, &dstrect);

    SDL_FreeSurface(l);
    SDL_FreeSurface(r);

    return rtn;
}

static SDL_Surface *textbox_draw_line(textbox *t, int lineno)
{
    char *text;
    SDL_Surface *rtn, *seg;
    col_rgb col;

    rtn = NULL;

    col_set(col, 255, 255, 255);

    if (lineno >= t->numlines)
        return NULL;

    while (text)
    {
        textbox_process_text_escapes(&text, &col, NULL);
        if (!text) break;

        seg = textbox_draw_seg(&text, col, 0, t->font);
        if (!seg) break;

        if (rtn) rtn = textbox_join_surf_horiz(rtn, seg);
        else     rtn = seg;
    }

    return rtn;
}
/*
static SDL_Surface *textbox_draw_line(textbox *t, int lineno)
{
    SDL_Color col;
    char *text;
    char *dup, *seg, *cur;
    SDL_Surface *textseg, *rtn, *new;

    dup = strdup(t->text[lineno]);
    cur = dup;
    seg = strsep(&cur, "\\");
    rtn = NULL;

    col.r = 255;
    col.g = 255;
    col.b = 255;

    while (seg)
    {
        int width;
        textseg = TTF_RenderUTF8_Blended(t->font, seg, col);
        if (rtn)
        {
            SDL_Rect dstrect;
            new = SDL_CreateRGBSurface(
                0, textseg->w + rtn->w, textseg->h, 32, 0xff, 0xff00, 0xff0000, 0xff000000
            );
            SDL_FillRect(new, NULL, SDL_MapRGBA(new->format, 0, 0, 0, 0));

            dstrect.x = dstrect.y = 0;
            dstrect.h = textseg->h;

            dstrect.w = rtn->w;

            SDL_SetSurfaceBlendMode(rtn, SDL_BLENDMODE_NONE);
            SDL_BlitSurface(rtn, NULL, new, &dstrect);

            dstrect.w = textseg->w;
            dstrect.x = rtn->w;

            SDL_SetSurfaceBlendMode(textseg, SDL_BLENDMODE_NONE);
            SDL_BlitSurface(textseg, NULL, new, &dstrect);

            SDL_FreeSurface(rtn);
            SDL_FreeSurface(textseg);
            rtn = new;
        }
        else
        {
            rtn = textseg;
        }

        seg = strsep(&cur, "\\");
        
        if (seg)
            switch (*seg)
            {
            case 'c':
                printf("%s\n", seg);
                sscanf(seg, "c%2hhx%2hhx%2hhx", &(col.r), &(col.g), &(col.b));
                seg += 7;
                break;
            }
    }
    SDL_SetSurfaceBlendMode(rtn, SDL_BLENDMODE_BLEND);
    return rtn;
}

int main(void)
{
    border  b;
    textbox t;
    SDL_Window   *w;
    SDL_Renderer *r;
    SDL_Surface  *s;
    TTF_Font     *f;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    w = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        513,
        513,
        0
    );

    if (w == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    r = SDL_CreateRenderer(w, -1, 0);

    if (r == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    s = SDL_GetWindowSurface(w);

    if (s == NULL) {
        printf("Could not create surface: %s\n", SDL_GetError());
        return 1;
    }

    f = TTF_OpenFont("/home/xeom/dev/rpg/asset/font/LiberationMono-Regular.ttf", 12);
    border_init(&b, "/home/xeom/dev/rpg/asset/art/borders/stone_brick.bmp");
    SDL_Rect area;

    area.x = 0;
    area.y = 0;
    area.w = 128;
    area.h = 128;
    textbox_init(&t, &b, f, s, &area);
    char *text[] = {"Hello,", "\\cff0000W\\cff7700o\\cffff00r\\c00ff00l\\c0000ffd"};
    t.numlines = 2;
    t.text = text;
    textbox_draw(&t);
    SDL_UpdateWindowSurface(w);
    SDL_Delay(2000);

    SDL_DestroyWindow(w);
    SDL_Quit();
    return 0;
}
*/
