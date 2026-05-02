#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

#include <iostream>
#include "define.h"

namespace gce {


class Console
{
public:
    static void Init();
    static void UnInit();

    enum Color: char8 { BLACK = '0', RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE };
    enum LogType: uint8 { SUCCESS, WARNING, ERROR_, TEST, LOGTYPE_AMOUNT };

    template <typename T> static void Log( T const output = "", cstr const end = "\n" ) { std::cout << output << end; }
    template <typename T> static void Log( T const output, Color const foreground, cstr const end = "\n" ) { std::cout << "\033[0;3" << foreground << 'm' << output << "\033[0m" << end; }
    template <typename T> static void Log( T const output, LogType const logType, cstr const end = "\n" ) { std::cout << "\033[0;3" << LOG_TYPE_TO_COLOR[logType] << 'm' << '[' << LOG_TYPE_TO_STRING[logType] << "] " << output << "\033[0m" << end; }

private:
    static constexpr cstr LOG_TYPE_TO_STRING[LOGTYPE_AMOUNT] = { "SUCCESS", "WARNING", "ERROR", "TEST" };
    static constexpr Color LOG_TYPE_TO_COLOR[LOGTYPE_AMOUNT] = { GREEN, YELLOW, RED, WHITE };
    inline static bool s_wasInit = false;
};


}

#endif