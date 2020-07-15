#ifndef _LOGGER_H_
#define _LOGGER_H_
#include "global.h"


    class Logger {
        public:
            enum m_priority {
                DEBUG,
                INFO,
                WARNING,
                ERROR
            };
            static Logger& instance() {
                static Logger insta;
                return insta;
            }
            static const std::string m_priority_names[];
            void write(m_priority pri,const char *, ... );
        private:
            Logger() {};
            Logger(const Logger&) {};
            Logger& operator=(const Logger&) {};
           
           
   };

#define LOG(PRI, MSG, ...) Logger::instance().write(PRI, MSG,  ##__VA_ARGS__)

#endif //_LOGGER_H_

