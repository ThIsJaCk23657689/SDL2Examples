#ifndef STATE_HPP
#define STATE_HPP

#include <SDL.h>

#include <memory>

#include "UI.hpp"
#include "Window.hpp"
#include "World/World.hpp"

struct State {
    SDL_GLContext context;
    std::unique_ptr<Window> window;
    std::unique_ptr<World> world;
    std::unique_ptr<UI> ui;
};

extern State state;

#endif
