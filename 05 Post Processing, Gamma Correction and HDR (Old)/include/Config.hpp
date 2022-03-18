#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

enum ScreenMode : unsigned int {
    Divide = 0,
    OrthoX = 1,
    OrthoY = 2,
    OrthoZ = 3,
    Prspct = 4,
};

struct Config {
    // Window Settings
    bool is_fullscreen = false;
    int width = 800;
    int height = 600;
    std::string title = "SDL2 Examples - 04 Texture, Billboard and Fog Effect";
    ScreenMode screen_mode = ScreenMode::Prspct;

    // OpenGL Settings
    int opengl_major_version = 3;
    int opengl_minor_version = 3;
    std::string glsl_version = "#version 330";
};

#endif