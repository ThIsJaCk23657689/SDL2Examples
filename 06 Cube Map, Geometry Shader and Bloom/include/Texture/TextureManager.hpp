#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>
#include <glad/glad.h>

#include <map>
#include <string>

#include "Texture/Texture2D.hpp"
#include "Texture/CubeMap.hpp"

class TextureManager {
public:
    static std::map<std::string, Texture2D> texture2Ds;
    static std::map<std::string, CubeMap> cubemaps;

    static void Initialize();
    static void Destroy();

    static Texture2D &CreateTexture2D(const std::string &file_name, const std::string &texture_name, bool is_srgb = false);
    static Texture2D &CreateTexture2D(const int width, const int height, const std::string &texture_name);
    static Texture2D &GetTexture2D(const std::string &texture_name);

    static CubeMap &CreateCubeMap(const std::vector<std::string> &file_names, const std::string &texture_name, bool is_srgb = false);
    static CubeMap &GetCubeMap(const std::string &texture_name);


private:
    TextureManager() = default;
    static Texture2D LoadTexture2DFromFile(const std::string &file_path, bool is_srgb);
    static CubeMap LoadCubeMapFromFile(const std::vector<std::string> &file_path, bool is_srgb);
};

#endif
