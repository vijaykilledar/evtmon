#ifndef _EVT_MONITOR_H
#define _EVT_MONITOR_H

class EventMonitor {
    public:
        virtual void handle_event() {};
        virtual ~EventMonitor() {};
};

#endif // _EVT_MONITOR_H
