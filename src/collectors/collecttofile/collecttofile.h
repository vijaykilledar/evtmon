#ifndef _COLLECTTOFILE_H_
#define _COLLECTTOFILE_H_

#include "global.h"
#include "evt_collector.h"

class CollectToFile: public EventCollector {
    private:
        std::string m_name;
    public:
        CollectToFile(std::string name) {
            m_name = name;
        }
        virtual bool init() {
            return true;
        }
        virtual bool start();
};
#endif // _COLLECTTOFILE_H_
