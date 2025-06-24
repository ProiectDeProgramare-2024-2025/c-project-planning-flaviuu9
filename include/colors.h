#ifndef COLORS_H
#define COLORS_H

#include <string>

namespace Colors {
    // ANSI escape codes for colors
    const std::string RESET        = "\033[0m";
    const std::string RED          = "\033[31m";
    const std::string GREEN        = "\033[32m";
    const std::string YELLOW       = "\033[33m";
    const std::string BLUE         = "\033[34m";
    const std::string MAGENTA      = "\033[35m";
    const std::string CYAN         = "\033[36m";
    const std::string WHITE        = "\033[37m";

    const std::string BRIGHT_RED   = "\033[91m";
    const std::string BRIGHT_GREEN = "\033[92m";
    const std::string BRIGHT_YELLOW= "\033[93m";
    const std::string BRIGHT_BLUE  = "\033[94m";
    const std::string BRIGHT_MAGENTA="\033[95m";
    const std::string BRIGHT_CYAN  = "\033[96m";
    const std::string BRIGHT_WHITE = "\033[97m";

    // Utility functions for colorized output
    inline std::string colorize(const std::string& text, const std::string& color) {
        return color + text + RESET;
    }

    inline std::string success(const std::string& msg) { return colorize(msg, GREEN); }
    inline std::string error(const std::string& msg)   { return colorize(msg, RED); }
    inline std::string warning(const std::string& msg) { return colorize(msg, YELLOW); }
    inline std::string info(const std::string& msg)    { return colorize(msg, CYAN); }
    inline std::string highlight(const std::string& msg) { return colorize(msg, BRIGHT_WHITE); }
    inline std::string title(const std::string& msg)   { return colorize(msg, BRIGHT_MAGENTA); }
}

#endif // COLORS_H