#ifndef _LOGGER_H_
#define _LOGGER_H_
#include "global.h"

enum class LOG {
    DEBUG   = 0,
    INFO    = 1,
    WARN    = 2,
    ERROR   = 3
};

class Logger {
    public:
        LOG m_def_log;
        static Logger& instance() {
            static Logger insta;
            return insta;
        }
        static const std::string m_priority_names[];
        void write(LOG pri,const char *, ... );
        void set_log_level(LOG l_level) {
            m_def_log = l_level;
        }
    private:
        Logger() {
            m_def_log = LOG::ERROR;
        };
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
};

#define log(PRI, MSG, ...) Logger::instance().write(PRI, MSG,  ##__VA_ARGS__)

#endif //_LOGGER_H_

