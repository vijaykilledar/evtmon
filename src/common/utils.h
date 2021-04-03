#ifndef _UTILIS_H
#define _UTILIS_H
 
#include <iostream>
#include <string>
#include <ctime>
 
namespace Util
{
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string CurrentDateTime()
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [80];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer,80,"%d-%m-%Y-%H:%M:%S",timeinfo);
        return buffer;
    }
}
#endif // _UTILIS_H
