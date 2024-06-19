#include "logger.h"
#include <iostream>
#include <cstdarg>

static LOGGER getLogLevel (DEBUG_LEVEL dbg_lvl) {
    switch (dbg_lvl) {
        case DEBUG_LEVEL::SILENT:
            return LOGGER::OFF;
        case DEBUG_LEVEL::RELEASE:
            return LOGGER::ERROR;
        case DEBUG_LEVEL::STAGING:
            return LOGGER::INFO;
        case DEBUG_LEVEL::DEV:
            return LOGGER::DEBUG;
        case DEBUG_LEVEL::LOUD:
            return LOGGER::VERBOSE;
        default:
            return LOGGER::OFF;
    }
};

void setLogLevel(DEBUG_LEVEL level) 
    { LOG_LEVEL = getLogLevel(level); return; }


static bool letsGo(LOGGER level) 
    { return level <= LOG_LEVEL; }

void report(LOGGER log_level, const char* format, ...){
        std::va_list args;
        va_start(args, format);

        if (letsGo(log_level)) {
            switch(log_level) {
                case LOGGER::ILINE:
                case LOGGER::DLINE:
                case LOGGER::VLINE:
                    std::cout << " \t ";
                    break;
                case LOGGER::ERROR:
                    std::cout << " [ERROR]: ";
                    break;
                case LOGGER::INFO:
                    std::cout << " [INFO]: ";
                    break;
                case LOGGER::DEBUG:
                    std::cout << " [DEBUG]: ";
                    break;
                case LOGGER::VERBOSE:
                    std::cout << " [VERBOSE]: ";
                    break;
                default:
                    break;
            }

            vprintf(format, args);
            std::cout << std::endl;
        }
        va_end(args);

    return;
}