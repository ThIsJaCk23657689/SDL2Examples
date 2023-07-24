#include "Util/Logger.hpp"

#include <SDL.h>
#include <glad/glad.h>

#include <iostream>
#include <ctime>
#include <chrono>
#include <mutex>

void Logger::ShowMe() {
    std::cout << std::endl;
    std::cout << "Created by: " << std::endl;
    std::cout << "\t\t  ________    ____         __      ________  " << std::endl;
    std::cout << "\t\t /_  __/ /_  /  _/____    / /___ _/ ____/ /__" << std::endl;
    std::cout << "\t\t  / / / __ \\ / // ___/_  / / __ `/ /   / //_/" << std::endl;
    std::cout << "\t\t / / / / / // /(__  ) /_/ / /_/ / /___/ ,<   " << std::endl;
    std::cout << "\t\t/_/ /_/ /_/___/____/\\____/\\__,_/\\____/_/|_|  " << std::endl;
    std::cout << std::endl;
}

void Logger::Message(LogLevel log_level, const std::string& message) {
    switch (log_level) {
        case LogLevel::Debug:
            std::cout << GetTimestamp() << "[DEBUG] " << message << std::endl;
            break;
        case LogLevel::Info:
            std::cout << GetTimestamp() << "[INFO] " << message << std::endl;
            break;
        case LogLevel::Warning:
            std::cout << GetTimestamp() << "[WARNING] " << message << std::endl;
            break;
        case LogLevel::Error:
            std::cerr << GetTimestamp() << "[ERROR] " << message << std::endl;
            break;
        case LogLevel::SDLError:
            std::cerr << GetTimestamp() << "[SDL ERROR] " << message << SDL_GetError() << std::endl;
            break;
        default:
            std::cerr << GetTimestamp() << "[UNKNOWN] " << "Undefined [LogLevel] in Logger.Message()" << std::endl;
            break;
    }
}

void Logger::ShowGLInfo() {
    std::cout << GetTimestamp() << "[INFO] OpenGL Version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << GetTimestamp() << "[INFO] GLSL Version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << GetTimestamp() << "[INFO] GPU Renderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << GetTimestamp() << "[INFO] GPU Vendor:\t\t" << glGetString(GL_VENDOR) << std::endl;
}

// TODO: 記得改成 chrono
std::string Logger::GetTimestamp() {
    std::time_t now = std::time(0);
    std::tm tm_struct {};
#if defined(__unix__)
    localtime_r(&now, &tm_struct);
#elif defined(_MSC_VER)
    localtime_s(&tm_struct, &now);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    tm_struct = *std::localtime(&now);
#endif
    char string_buffer[80];
    std::strftime(string_buffer, sizeof(string_buffer), "%F %T ", &tm_struct);
    return string_buffer;
}