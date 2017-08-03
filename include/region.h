#if ! defined(REGION_H)
# define REGION_H
# include "colour.h"

typedef struct tile_s tile;
typedef struct region_s region;


struct tile_s
{
    short vege;
    short elev;
    short temp;
//    location *loc;
};

struct location_s
{
    char biome[16];
    short temp_avg;
    short elev_avg;
    char name[16];
};

struct region_s
{
    int size;
    int sizelvl;
    tile *tiles;
//    vec  *locations;
};

#define region_index(r, x, y) \
    ((r)->tiles + (x) + (y) * (r)->size)

extern int region_elev_preset[9];

extern int region_temp_preset[9];

extern int   region_cold_hue;
extern float region_temp_hue_change;
extern int   region_temp_neutral;
extern int   region_temp_hue_min;
extern int   region_temp_hue_max;
extern float region_elev_sat_change;
extern int   region_elev_neutral;
extern float region_elev_light_change;

void region_init(region *r, int sizelvl);

void region_gen_vege(region *r);

void region_gen_elev(region *r);

void region_gen_temp(region *r);

col_rgb region_terrain_col(region *r, int x, int y, col_rgb orig);

#endif
