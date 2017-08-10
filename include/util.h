#if ! defined(UTIL_H)
# define UTIL_H
# include <SDL2/SDL.h>

# if SDL_BYTEORDER == SDL_BIG_ENDIAN
#  define sdl_surf_masks 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
# else
#  define sdl_surf_masks 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
# endif

# define surface_default(w, h) SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0)
# define surface_alpha(w, h)   SDL_CreateRGBSurface(0, w, h, 32, sdl_surf_masks)

# define max(a, b) ( ((a) > (b)) ? (a) : (b) )
# define min(a, b) ( ((a) < (b)) ? (a) : (b) )

# define bound(lower, a, upper) max((lower), min((upper), (a)))

int str_check_len(char *str, int n);

#endif /* UTIL_H */
