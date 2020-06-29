#ifndef _EVT_MONITOR_H
#define _EVT_MONITOR_H

class EventMonitor {
    public:
        virtual void operator() () {};
        virtual void handle_event() {};
        virtual bool init() {};
        virtual bool start() {};
        virtual ~EventMonitor() {};
};

#endif // _EVT_MONITOR_H
