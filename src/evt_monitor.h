#ifndef _EVT_MONITOR_H
#define _EVT_MONITOR_H

class EventMonitor {
    public:
        virtual void operator() () {};
        virtual void handle_event() {};
        virtual bool init() = 0;
        virtual bool start() = 0;
        virtual ~EventMonitor() {};
};

#endif // _EVT_MONITOR_H
