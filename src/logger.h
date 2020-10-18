#ifndef _LOGGER_H_
#define _LOGGER_H_
#include "global.h"

enum class logtype {
    DEBUG   = 0,
    INFO    = 1,
    WARNING = 2,
    ERROR   = 3
};

class Logger {
    public:
        static Logger& instance() {
            static Logger insta;
            return insta;
        }
        static const std::string m_priority_names[];
        void write(logtype pri,const char *, ... );
    private:
        Logger() {};
        Logger(const Logger&) {};
        Logger& operator=(const Logger&) {};
};

#define LOG(PRI, MSG, ...) Logger::instance().write(PRI, MSG,  ##__VA_ARGS__)

#endif //_LOGGER_H_

