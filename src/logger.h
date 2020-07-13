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
        private:
            Logger() {};
            Logger(const Logger&) {};
            Logger& operator=(const Logger&) {};
            static const std::string m_priority_names[];
            static void write(m_priority pri, ... );
   };

#endif //_LOGGER_H_

