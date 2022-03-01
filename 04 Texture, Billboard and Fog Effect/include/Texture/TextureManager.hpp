#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>
#include <glad/glad.h>

#include <map>
#include <string>

#include "Texture/Texture2D.hpp"

class TextureManager {
public:
    static std::map<std::string, Texture2D> texture2Ds;
    static void Initialize();
    static Texture2D &CreateTexture2D(const std::string &file_name, const std::string &texture_name);
    static Texture2D &GetTexture2D(const std::string &texture_name);
    static void Clear();

private:
    TextureManager() = default;
    static Texture2D LoadTexture2DFromFile(const std::string &file_path);
};

#endif
