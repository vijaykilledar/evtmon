#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

#include<iostream>

#include "evt_monitor.h"

class IntfEvtMonitor: public EventMonitor {
    private:
        IntfEvtMonitor();
        IntfEvtMonitor(IntfEvtMonitor const& copy);
        IntfEvtMonitor& operator=(IntfEvtMonitor const& copy);

    public:
        virtual void handle_event();
        static IntfEvtMonitor& get_instance() {
            static IntfEvtMonitor instance;
            return instance;
        }
};

#endif // _MONITOR_INTF_H_
