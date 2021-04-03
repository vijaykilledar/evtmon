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
        char *buffer;
        va_start (arg, msg);
        size_t sz = vasprintf (&buffer, msg, arg);
        if(sz) {
            std::string new_msg = Util::CurrentDateTime() + ":" + m_priority_names[static_cast<int>(pri)] + ": " + std::string(buffer) + "\n";
            done = fprintf (stdout, new_msg.c_str(), arg);
            free(buffer);
        }
        va_end (arg);
    }
}
