#include <math.h>
#include <stdlib.h>

#include "../include/terrain.h"
#include "../include/colour.h"
#include "../include/region.h"

static float region_terrain_hue(region *r, int x, int y, float hue);

static float region_terrain_light(region *r, int x, int y, float light);

static float region_terrain_sat(region *r, int x, int y, float sat);

void region_init(region *r, int sizelvl)
{
    r->sizelvl = sizelvl;
    r->size = (1 << sizelvl) + 1;
    r->tiles = malloc(sizeof(tile) * (size_t)(r->size * r->size));
}

int region_vege_preset[9] = {
    500, 500,500,
    500, 500,500,
    500, 500, 500
};

void region_gen_vege(region *r)
{
    terrain t;
    int x, y;

    terrain_init(&t, r->sizelvl, terrain_funct_cloud);
    terrain_preload(&t, 1, region_vege_preset);

    while (t.n < r->sizelvl)
        terrain_iter(&t);

    for (x = 0; x < r->size; x++)
        for (y = 0; y < r->size; y++)
            region_index(r, x, y)->vege = (short)terrain_index(&t, x, y);

    terrain_kill(&t);
}

int region_elev_preset[9] = {
    0, 0, 0,
    0, 500, 0,
    0, 500, 0
};

void region_gen_elev(region *r)
{
    terrain t;
    int x, y;

    terrain_init(&t, r->sizelvl, terrain_funct_elev);
    terrain_preload(&t, 1, region_elev_preset);

    while (t.n < r->sizelvl)
        terrain_iter(&t);

    for (x = 0; x < r->size; x++)
        for (y = 0; y < r->size; y++)
            region_index(r, x, y)->elev = (short)terrain_index(&t, x, y);

    terrain_kill(&t);
}

int region_temp_preset[9] = {
    300, 300, 300,
    700, 700, 700,
    300, 300, 300
};

void region_gen_temp(region *r)
{
    terrain t;
    int x, y;

    terrain_init(&t, r->sizelvl, terrain_funct_cloud);
    terrain_preload(&t, 1, region_temp_preset);

    while (t.n < r->sizelvl)
        terrain_iter(&t);

    for (x = 0; x < r->size; x++)
        for (y = 0; y < r->size; y++)
            region_index(r, x, y)->temp = (short)terrain_index(&t, x, y);
}

#define max(a, b) ( ((a) > (b)) ? (a) : (b) )
#define min(a, b) ( ((a) < (b)) ? (a) : (b) )

#define bound(lower, a, upper) max((lower), min((upper), (a)))

float region_temp_hue_change = 40;
int region_temp_neutral    = 500;
int region_temp_hue_min    = 54;
int region_temp_hue_max    = 105;

static float region_terrain_hue(region *r, int x, int y, float hue)
{
    float change;

    if (hue > region_temp_hue_max || hue < region_temp_hue_min)
        return hue;

    change  = (float)region_temp_hue_change / 500.0f;
    change *= (float)(region_temp_neutral - region_index(r, x, y)->temp);
    hue    += change;

    return (float)(((int)round(hue) + 1024) % 256);
}

float region_elev_sat_change = 150;
int region_elev_neutral    = 200;

static float region_terrain_sat(region *r, int x, int y, float sat)
{
    float change;

    change  = (float)region_elev_sat_change / 500.0f;
    change *= (float)(region_elev_neutral - region_index(r, x, y)->elev);

    return bound(0, sat, 255);
}

float region_slope_light_change = 1.5;
float region_slope_max_change = 20;

static float region_terrain_light(region *r, int x, int y, float light)
{
    int slope;
    float change;

    if (y == 0)
        return light;

    slope  = region_index(r, x, y-1)->elev - region_index(r, x, y)->elev;
    change = region_slope_light_change * (float)slope;
    change = bound(-region_slope_max_change, change, region_slope_max_change);
    light += change;

    return bound(0, light, 255);
}

col_rgb region_terrain_col(region *r, int x, int y, col_rgb orig)
{
    col_hsl hsl;

    hsl = colour_to_hsl(orig);

    hsl.h = (unsigned char)round(region_terrain_hue  (r, x, y, hsl.h));
    hsl.s = (unsigned char)round(region_terrain_sat  (r, x, y, hsl.s));
    hsl.l = (unsigned char)round(region_terrain_light(r, x, y, hsl.l));

    return colour_to_rgb(hsl);
}
