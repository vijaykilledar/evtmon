#ifndef _EVT_MONITOR_H
#define _EVT_MONITOR_H

#include "global.h"
#include "event_queue.h"

class EventMonitor {
    private:
        EventQueue *event_q;
        Json::Value conf;
    public:
        EventMonitor() {
            event_q = NULL;
        }
        virtual void operator() () {};
        virtual void publish_event(std::string src, EventType e_type, Json::Value data) {
            Event *event_ptr = new Event(src, e_type, data);
            event_q->add(event_ptr);
        }
        virtual bool init() = 0;
        virtual bool start() = 0;
        virtual bool configure(EventQueue *eq, Json::Value conf_val) {
            event_q = eq;
            conf = conf;
            return true;
        };
        virtual ~EventMonitor() {};
};

#endif // _EVT_MONITOR_H
