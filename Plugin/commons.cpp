#include <stdafx.h>
#include "common.h"

#include <stdio.h>
#include <stdarg.h>

void Assert(const char* error , const char* file  , int line , const char* function, ...)
{
    const size_t bufsize = 1024;
    char buffer[bufsize];
    ZeroMemory(buffer, sizeof(buffer));

    va_list argp;
    va_start(argp, function);
    vsprintf_s(buffer, bufsize, error, argp);
    va_end(argp);
        
    const size_t lbufsize = 6;
    char lbuffer[lbufsize];
    ZeroMemory(lbuffer, sizeof(lbuffer));
    _itoa_s(line, lbuffer, lbufsize, 10);

    std::string text;
    text += buffer;
    text += "\n\n";

    text += "File: ";
    text += file;
    text += "\n";

    text += "Line: ";
    text += lbuffer;
    text += "\n";

    text += "Function: ";
    text += function;
    text += "\n\n";

    text += "Ok = Continue, Cancel = Debug";

    MessageBoxA(0, text.c_str(), "Warning", MB_OKCANCEL | MB_ICONEXCLAMATION);
}
