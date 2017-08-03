#if ! defined(COLOUR_H)
# define COLOUR_H

typedef struct col_rgb_s col_rgb;
typedef struct col_hsl_s col_hsl;

struct col_rgb_s
{
    unsigned char r, g, b, a;
};

struct col_hsl_s
{
    unsigned char h, s, l, a;
};

col_hsl colour_to_hsl(col_rgb rgb);

col_rgb colour_to_rgb(col_hsl hsl);

#endif
