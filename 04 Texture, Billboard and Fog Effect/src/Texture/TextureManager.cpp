#include "Texture/TextureManager.hpp"

#include <stb_image.h>

#include <utility>
#include "Util/Logger.hpp"

std::map<std::string, Texture2D> TextureManager::texture2Ds;

void TextureManager::Initialize() {
    // Create Textures (Note: All the textures which be used must create in this function)
    TextureManager::CreateTexture2D("rickroll.png", "RickRoll");
    TextureManager::CreateTexture2D("wall.jpg", "Wall");
    TextureManager::CreateTexture2D("awesomeface.png", "Face");

    // TextureManager::GetTexture2D("Rick Roll").SetWrapParameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    // TextureManager::GetTexture2D("Rick Roll").SetFilterParameters(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);

    // TextureManager::CreateTexture2D("earth.jpg", "Earth");
    // TextureManager::GetTexture2D("Earth").SetWrapParameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    // TextureManager::GetTexture2D("Earth").SetFilterParameters(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
}

Texture2D &TextureManager::CreateTexture2D(const std::string &file_name, const std::string &texture_name) {
    std::string file_path = "assets/textures/" + file_name;
    texture2Ds[texture_name] = std::move(LoadTexture2DFromFile(file_path));
    return texture2Ds[texture_name];
}

Texture2D &TextureManager::GetTexture2D(const std::string &texture_name) {
    if (texture2Ds.find(texture_name) != texture2Ds.end()) {
        return texture2Ds[texture_name];
    } else {
        Logger::Message(LogLevel::Error, "The texture <" + texture_name + "> which is not loaded in the application.");
        Logger::Message(LogLevel::Error, "Maybe you forget to use CreateTexture2D() in the TextureManager::Initialize().");
        exit(-1);
    }
}

void TextureManager::Clear() {
    for (auto &iteration : texture2Ds) {
        iteration.second.Clear();
    }
}

Texture2D TextureManager::LoadTexture2DFromFile(const std::string &file_path) {
    Texture2D texture;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

    if (image) {
        GLint internal_format(-1);
        GLenum format(-1);

        switch (nrChannels) {
            case 1:
                internal_format = GL_R8;
                format = GL_RED;
                break;
            case 3:
                internal_format = GL_RGB8;
                format = GL_RGB;
                break;
            case 4:
                internal_format = GL_RGBA8;
                format = GL_RGBA;
                break;
            default:
                Logger::Message(LogLevel::Error, "The Images File format is not supported yet!, Path: " + file_path);
                break;
        }

        // Create texture and binding
        texture.Generate(internal_format, format, width, height, image, false);
    } else {
        Logger::Message(LogLevel::Error, "Failed to load image at path: " + file_path);
        exit(-42069);
    }

    stbi_image_free(image);
    return texture;
}