
typedef enum
{
    border_top = 1,
    border_bottom = 2,
    border_left = 4,
    border_right = 8
} border_edge;

typedef struct border_s border;

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

void border_blit(border *b, SDL_Surface *s, SDL_Rect *r, border_edge edges);
