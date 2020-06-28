#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

#include<iostream.h>

#include "evt_monitor.h"

class IntfEvtMonitor: public EventMonitor {
    
    public:
        IntfEvtMonitor();
        virtual void handle_event();

};

#endif // _MONITOR_INTF_H_
