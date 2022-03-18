#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <memory>

#include "Config.hpp"
#include "Game.hpp"

struct Application {
    explicit Application(const Config& config);
    ~Application();

    void Initialize();
    void Run();

    float current_time = 0.0f;
    float delta_time = 0.0f;
    float last_time = 0.0f;
    const Config& my_config;
    std::unique_ptr<Game> game;
};

#endif
