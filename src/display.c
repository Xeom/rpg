#include "pinfo.h"

#include "display.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window   *display_window;
SDL_Surface  *display_surface;

int display_height, display_width;

int display_alive = 0;

static void display_resize(void);

void (*display_on_keydown)(const char *key) = NULL;
void (*display_on_keyup)(const char *key)   = NULL;

void display_init(void)
{
    display_alive = 1;

    if (SDL_Init(SDL_INIT_VIDEO) == 0)
        pinfo("SDL Initialized");
    else
        perr("SDL Init failed: %s", SDL_GetError());

    if (TTF_Init() == 0)
        pinfo("TTF Initialized");
    else
        perr("TTF Init failed: %s", TTF_GetError());

    display_window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        512,
        512,
        SDL_WINDOW_MAXIMIZED
    );

    if (display_window)
        pinfo("Created window");
    else
        perr("Could not create window: %s", SDL_GetError());

    display_surface  = SDL_GetWindowSurface(display_window);

    if (display_surface)
        pinfo("Created window surface");
    else
        perr("Could not create window surface: %s", SDL_GetError());
}


void display_kill(void)
{
    display_alive = 0;
    SDL_FreeSurface(display_surface);
    SDL_DestroyWindow(display_window);
    SDL_Quit();
}

void display_poll(void)
{
    SDL_Event event;
    int w;

    SDL_UpdateWindowSurface(display_window);
    SDL_GetWindowMaximumSize(display_window, &w, NULL);

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            display_kill();
            break;

        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                display_width  = event.window.data1;
                display_height = event.window.data2;
                display_resize();
                break;
            }
            break;

        case SDL_KEYDOWN:
            if (display_on_keydown)
                display_on_keydown(SDL_GetKeyName(event.key.keysym.sym));
            break;

        case SDL_KEYUP:
            if (display_on_keyup)
                display_on_keyup(SDL_GetKeyName(event.key.keysym.sym));
            break;
        }

        SDL_UpdateWindowSurface(display_window);
    }
}

static void display_resize(void)
{
    printf("Display resized to %dx%d\n", display_width, display_height);
    display_surface  = SDL_GetWindowSurface(display_window);
}
