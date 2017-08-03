#if !defined(TERRAIN_H)
# define TERRAIN_H

typedef struct terrain_s terrain;

typedef int (*terrainfunct)(terrain *t, int a, int b, int c, int d);

struct terrain_s
{
    int n;
    int ssize;
    int hsize;
    int size;
    int *data;
    terrainfunct funct;
};

#define terrain_index(t, x, y) \
    (t)->data[(x) + (y) * (t)->size]

#define terrain_valid(t, x, y) \
    ((x) >= 0 && (x) < (t)->size && (y) >= 0 && (y) < (t)->size)

#define terrain_try_get(t, x, y) \
    terrain_valid(t, x, y) ? terrain_index(t, x, y) : -1


int terrain_funct_avg(terrain *t, int a, int b, int c, int d);

int terrain_funct_choice(terrain *t, int a, int b, int c, int d);

int terrain_funct_elev(terrain *t, int a, int b, int c, int d);

int terrain_funct_cloud(terrain *t, int a, int b, int c, int d);

int terrain_init(terrain *t, int size, terrainfunct funct);

void terrain_kill(terrain *t);

void terrain_preload(terrain *t, int preloadn, const int *data);

void terrain_iter(terrain *t);

#endif
