#ifndef _INTF_EVT_MONITOR_H_
#define _INTF_EVT_MONITOR_H_

#include<iostream.h>

#include "evt_monitor.h"

class IntfEvtMonitor: public EventMonitor {
    
    public:
        IntfEvtMonitor();
        virtual void handle_event();

};

#endif // _INTF_EVT_MONITOR_H_
