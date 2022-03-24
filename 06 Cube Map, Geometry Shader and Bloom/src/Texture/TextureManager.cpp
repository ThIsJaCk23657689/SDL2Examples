#include "Texture/TextureManager.hpp"

#include <stb_image.h>

#include <utility>

#include "State.hpp"
#include "Util/Logger.hpp"

std::map<std::string, Texture2D> TextureManager::texture2Ds;

void TextureManager::Initialize() {
    // Create Textures (Note: All the textures which be used must create in this function)
    TextureManager::CreateTexture2D("rickroll.png", "RickRoll", true);
    TextureManager::CreateTexture2D("awesomeface.png", "Awesome Face", true);
    TextureManager::CreateTexture2D("earth.jpg", "Earth", true);
    TextureManager::CreateTexture2D("sun.jpg", "Sun", true);
    TextureManager::CreateTexture2D("moon.jpg", "Moon", true);
    TextureManager::CreateTexture2D("wood.png", "Wooden Floor", true);

    // Create Texture for Post Processing
    TextureManager::CreateTexture2D(state.window->width, state.window->height, "PostProcessing");
    TextureManager::CreateTexture2D(state.window->width, state.window->height, "Bloom");
    TextureManager::CreateTexture2D(state.window->width, state.window->height, "GaussianBlur0");
    TextureManager::CreateTexture2D(state.window->width, state.window->height, "GaussianBlur1");
}

Texture2D &TextureManager::CreateTexture2D(const std::string &file_name, const std::string &texture_name, bool is_srgb) {
    std::string file_path = "assets/textures/" + file_name;
    texture2Ds[texture_name] = std::move(LoadTexture2DFromFile(file_path, is_srgb));
    return texture2Ds[texture_name];
}

Texture2D& TextureManager::CreateTexture2D(const int width, const int height, const std::string& texture_name) {
    Texture2D texture = Texture2D();
    texture.Generate(GL_RGB16F, GL_RGB, width, height, nullptr, false);
    texture.SetFilterParameters(GL_LINEAR, GL_LINEAR);
    texture.SetWrapParameters(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture2Ds[texture_name] = texture;
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

void TextureManager::Destroy() {
    for (auto &iteration : texture2Ds) {
        iteration.second.Destroy();
    }
}

Texture2D TextureManager::LoadTexture2DFromFile(const std::string &file_path, bool is_srgb) {
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
                // 只要寬不是 4 的倍數，就不使用 Alignment
                if (width % 4 != 0) {
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                }

                if (is_srgb) {
                    internal_format = GL_SRGB8;
                } else {
                    internal_format = GL_RGB8;
                }

                format = GL_RGB;
                break;
            case 4:
                if (is_srgb) {
                    internal_format = GL_SRGB_ALPHA;
                } else {
                    internal_format = GL_RGBA8;
                }

                format = GL_RGBA;
                break;
            default:
                Logger::Message(LogLevel::Error, "The Images File format is not supported yet!, Path: " + file_path);
                break;
        }

        // Create texture and binding
        texture.Generate(internal_format, format, width, height, image, true);
    } else {
        Logger::Message(LogLevel::Error, "Failed to load image at path: " + file_path);
        exit(-42069);
    }

    // 效率會比較好
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    stbi_image_free(image);
    return texture;
}