#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

enum LogLevel : unsigned int {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    SDLError = 4,
};

struct Logger {
    static void ShowMe();
    static void Message(LogLevel log_level, const std::string& message);
    static void ShowGLInfo();

    static std::string GetTimestamp();
};

#endif