#include <SDL2/SDL.h>
#include <math.h>

#include "colour.h"

SDL_Color colour_to_sdl(col_rgb rgb)
{
    SDL_Color rtn;

    rtn.r = rgb.r;
    rtn.g = rgb.g;
    rtn.b = rgb.b;

    return rtn;
}

col_hsl colour_to_hsl(col_rgb rgb)
{
    unsigned char maxval, minval;
    float hue, light, sat;
    col_hsl hsl;

    /* Get the maximum and minimum colours */
    if (rgb.r < rgb.g)
    {
        maxval = (rgb.b < rgb.g) ? rgb.g : rgb.b;
        minval = (rgb.b < rgb.r) ? rgb.b : rgb.r;
    }
    else
    {
        maxval = (rgb.b < rgb.r) ? rgb.r : rgb.b;
        minval = (rgb.b < rgb.g) ? rgb.b : rgb.g;
    }

    /* The lightness is calculated easily (range 0-255) */
    light = (float)(maxval + minval) / 2.0f;

    /* If the colour is grey, our job is easy */
    if (maxval == minval)
    {
        hsl.h = 0;
        hsl.s = 0;
        hsl.l = (unsigned char)round(light);

        return hsl;
    }

    /* Do the math for the hue (range 0-6) */
    if (maxval == rgb.r)
    {
        hue  = (float)rgb.g - (float)rgb.b;
        hue /= (float)rgb.r - minval;
        if (hue < 0) hue += 6.0f;
    }
    else if (maxval == rgb.g)
    {
        hue  = (float)rgb.b - (float)rgb.r;
        hue /= (float)rgb.g - minval;
        hue += 2.0f;
    }
    else /* maxval == rgb.b */
    {
        hue  = (float)rgb.r - (float)rgb.g;
        hue /= (float)rgb.b - minval;
        hue += 4.0f;
    }

    /* Fix the hue's range */
    hue *= 255.0f / 6.0f;

    /* Saturation math */
    if (light < 127.5f)
        sat = (float)(maxval - minval) / (2.0f * light);
    else
        sat = (float)(maxval - minval) / (510.0f - 2.0f * light);

    /* Fix saturation range */
    sat *= 255.0f;

    hsl.h = (unsigned char)round(hue);
    hsl.s = (unsigned char)round(sat);
    hsl.l = (unsigned char)round(light);

    return hsl;

}

col_rgb colour_to_rgb(col_hsl hsl)
{
    float delta, minval, maxval;
    float red, green, blue;
    col_rgb rgb;

    if (hsl.s == 0)
    {
        rgb.r = hsl.l;
        rgb.g = hsl.l;
        rgb.b = hsl.l;

        return rgb;
    }

    delta = (float)hsl.s / 255.0f;

    if (hsl.l < 128)
        delta *= (2.0f * (float)hsl.l);
    else
        delta *= (510.0f - 2.0f * (float)hsl.l);

    maxval = (float)hsl.l + (delta / 2);
    minval = (float)hsl.l - (delta / 2);

    if (hsl.h < 43) /* r > g > b */
    {
        red   = maxval;
        blue  = minval;
        green = (hsl.h * 6.0f / 255.0f) * (red - blue) + blue;
    }
    else if (hsl.h < 85) /* g > r > b */
    {
        green = maxval;
        blue  = minval;
        red   = (hsl.h * 6.0f / 255.0f - 2) * (blue - green) + blue;
    }
    else if (hsl.h < 128) /* g > b > r */
    {
        green = maxval;
        red   = minval;
        blue  = (hsl.h * 6.0f / 255.0f - 2) * (green - red) + red;
    }
    else if (hsl.h < 170) /* b > g > r */
    {
        blue  = maxval;
        red   = minval;
        green = (hsl.h * 6.0f / 255.0f - 4) * (red - blue) + red;
    }
    else if (hsl.h < 213) /* b > r > g */
    {
        blue  = maxval;
        green = minval;
        red   = (hsl.h * 6.0f / 255.0f - 4) * (blue - green) + green;
    }
    else /* r > b > g */
    {
        red   = maxval;
        green = minval;
        blue  = (hsl.h * 6.0f / 255.0f - 6) * (green - red) + green;
    }

    rgb.r = (unsigned char)round(red);
    rgb.g = (unsigned char)round(green);
    rgb.b = (unsigned char)round(blue);

    return rgb;
}
