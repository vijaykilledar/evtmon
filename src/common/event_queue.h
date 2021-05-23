#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_

#include "global.h"
#include "event.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <syslog.h>
#include <unistd.h>
#include "evt_collector.h"

class EventQueue {
    private:
        std::queue<Event *> m_event_q;
        std::mutex m_mu;
        static EventQueue *m_instance;
        static std::mutex mymutex;
        std::condition_variable cond;
        EventQueue() = default;
        EventQueue& operator=(const EventQueue&) = delete;
        EventQueue(const EventQueue&) = delete;
        ~EventQueue() {
        
        }
    public:
        static EventQueue* get_instance2() {
            std::lock_guard<std::mutex> mylock(mymutex);
            if (!m_instance) {
                m_instance = new EventQueue();
            }
            return m_instance;
        }
        void add(Event *e) {
            std::unique_lock<std::mutex> lock(m_mu);
            m_event_q.push(e);
            lock.unlock();
            cond.notify_all();
        }
        void processor(const std::map<std::string, EventCollector *> &evt_collectors) {
            while(true) {
                std::unique_lock<std::mutex> lock(m_mu);
                cond.wait(lock, [this]() { return m_event_q.size() > 0; });
                Event *e = m_event_q.front();
                for(auto collector =  evt_collectors.begin(); collector != evt_collectors.end(); collector++) {
                    EventCollector *evt_collector = collector->second;
                    evt_collector->add(e->serialize());
                }
                delete e;
                m_event_q.pop();
                lock.unlock();
            }
        }
};


#endif // _EVENT_QUEUE_H_
