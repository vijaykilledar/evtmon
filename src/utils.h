#ifndef _UTILIS_H
#define _UTILIS_H
 
#include <iostream>
#include <string>
#include <time.h>
 
namespace Util
{
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string CurrentDateTime()
    {
        time_t     now = time(NULL);
        struct tm  tstruct;
        char       buf[80];
        localtime_s(&tstruct, &now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        return buf;
    }
}
#endif // _UTILIS_H
