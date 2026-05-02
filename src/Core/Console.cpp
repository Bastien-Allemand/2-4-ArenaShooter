#include "Console.h"

#include <windows.h>

namespace gce {


void Console::Init()
{
    if ( s_wasInit ) return;
    s_wasInit = true;
    
    AllocConsole();
    FILE* fp;
    freopen_s( &fp, "CONOUT$", "w", stdout );
    freopen_s( &fp, "CONOUT$", "w", stderr );
    freopen_s( &fp, "CONIN$", "r", stdin );

    HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
    DWORD dwMode = 0;
    GetConsoleMode( hOut, &dwMode );
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode( hOut, dwMode );
}

void Console::UnInit()
{
    if ( s_wasInit == false ) return;
    s_wasInit = false;

    FreeConsole();
}


}