#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL.h>

struct Window {
    int width = 0;
    int height = 0;
    bool should_close = false;
    SDL_Window* handler = nullptr;
};

#endif