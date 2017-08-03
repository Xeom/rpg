#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/terrain.h"


#define for_square_odd(t)                                \
    for (x = (t)->hsize; x < (t)->size; x += (t)->ssize) \
    for (y = 0;          y < (t)->size; y += (t)->ssize)


#define for_square_even(t)                               \
    for (x = 0;          x < (t)->size; x += (t)->ssize) \
    for (y = (t)->hsize; y < (t)->size; y += (t)->ssize)

#define for_diamond(t)                                   \
    for (x = (t)->hsize; x < (t)->size; x += (t)->ssize) \
    for (y = (t)->hsize; y < (t)->size; y += (t)->ssize)

static void terrain_set_n(terrain *t, int n);

static void terrain_square(terrain *t, int x, int y);

static void terrain_diamond(terrain *t, int x, int y);

static int terrain_get_funct_choices(int a, int b, int c, int d, int *choices)
{
    int num;

    num = 0;

    if (a != -1) choices[num++] = a;
    if (b != -1) choices[num++] = b;
    if (c != -1) choices[num++] = c;
    if (d != -1) choices[num++] = d;

    return num;
}

int terrain_funct_avg(terrain *t, int a, int b, int c, int d)
{
    int sum, num;

    sum = 0;
    num = 0;

    if (a != -1) { sum += a; num++; }
    if (b != -1) { sum += b; num++; }
    if (c != -1) { sum += c; num++; }
    if (d != -1) { sum += d; num++; }

    return sum / num;
}

int terrain_funct_choice(terrain *t, int a, int b, int c, int d)
{
    int num;
    int choices[4];

    num = terrain_get_funct_choices(a, b, c, d, choices);

    return choices[rand() % num];
}

int terrain_funct_elev(terrain *t, int a, int b, int c, int d)
{
    int res;
    int randomness;

    randomness = 3000 >> t->n;

    if (terrain_funct_choice(t, a, b, c, d) == 0) return 0;

    res = terrain_funct_avg(t, a, b, c, d);

    if (randomness)
        res += (rand() % randomness) - randomness / 2;

    if (res >= 1000) res = 1000;
    if (res <= 0) res = 1;

    return res;
}

int terrain_funct_cloud(terrain *t, int a, int b, int c, int d)
{
    int res;
    int randomness;

    randomness = (3000 >> t->n);

    res = terrain_funct_avg(t, a, b, c, d);

    if (randomness)
        res += (rand() % randomness) - randomness / 2;

    if (res >= 1000) res = 1000;
    if (res  < 0)    res = 0;

    return res;
}

int terrain_init(terrain *t, int sizelvl, terrainfunct funct)
{
    t->size = (1 << sizelvl) + 1;
    t->data = malloc((size_t)(t->size * t->size) * sizeof(int));

    if (t->data == NULL)
    {
        fputs("[!] Could not allocate terrain data\n", stderr);
        return -1;
    }

    memset(t->data, 0, (size_t)(t->size * t->size) * sizeof(int));
    t->funct = funct;
    terrain_set_n(t, 0);

    return 0;
}

void terrain_kill(terrain *t)
{
    free(t->data);
}

static void terrain_set_n(terrain *t, int n)
{
    t->n = n;
    t->ssize = (t->size - 1) >> n;
    t->hsize = t->ssize / 2;
}

void terrain_preload(terrain *t, int preloadn, const int *data)
{
    int n, x, y;

    terrain_set_n(t, preloadn);

    n = 0;

    for (y = 0; y < t->size; y += t->ssize)
        for (x = 0; x < t->size; x += t->ssize)
        {
            terrain_index(t, x, y) = data[n];
            n++;
        }

}

static void terrain_diamond(terrain *t, int x, int y)
{
    int a, b, c, d;

    a = terrain_try_get(t, x - t->hsize, y - t->hsize);
    b = terrain_try_get(t, x - t->hsize, y + t->hsize);
    c = terrain_try_get(t, x + t->hsize, y - t->hsize);
    d = terrain_try_get(t, x + t->hsize, y + t->hsize);

    terrain_index(t, x, y) = t->funct(t, a, b, c, d);
}

static void terrain_square(terrain *t, int x, int y)
{
    int a, b, c, d;

    a = terrain_try_get(t, x + t->hsize, y           );
    b = terrain_try_get(t, x,            y + t->hsize);
    c = terrain_try_get(t, x - t->hsize, y           );
    d = terrain_try_get(t, x,            y - t->hsize);

    terrain_index(t, x, y) = t->funct(t, a, b, c, d);
}

void terrain_iter(terrain *t)
{
    int x, y;

    for_diamond(t)
        terrain_diamond(t, x, y);

    for_square_odd(t)
        terrain_square(t, x, y);

    for_square_even(t)
        terrain_square(t, x, y);

    terrain_set_n(t, t->n + 1);
}
