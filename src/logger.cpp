#include<stdarg.h>
#include "logger.h"
#include "utils.h"

const std::string Logger::m_priority_names[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
};

void Logger::write(LOG pri, const char  *msg, ...) {
    if(pri >= m_def_log) {
        va_list arg;
        int done;
        char buffer[256];
        va_start (arg, msg);
        vsnprintf (buffer, 255, msg, arg);
        std::string new_msg = Util::CurrentDateTime() + ":" + m_priority_names[static_cast<int>(pri)] + ": " + buffer + "\n";
        done = fprintf (stdout, new_msg.c_str(), arg);
        va_end (arg);
    }
}
