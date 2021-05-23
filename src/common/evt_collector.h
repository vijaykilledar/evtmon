#ifndef _EVT_COLLECTOR_H_
#define _EVT_COLLECTOR_H_

#include "global.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class EventCollector {
    private:

    protected:
        Json::Value m_conf;
        std::queue<std::string> m_event_q;
        std::mutex m_mutex;
        std::condition_variable m_cond;
    public:
        EventCollector() {}
        void add(std::string event_str) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_event_q.push(event_str);
            lock.unlock();
            m_cond.notify_all();
        }
        std::string collect_event() {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]() {return m_event_q.size() > 0; });
            std::string ret = m_event_q.front();
            m_event_q.pop();
            lock.unlock();
            return ret;
        }
        virtual bool init() = 0;
        virtual bool start() = 0;
        virtual bool configure(Json::Value conf) {
            m_conf = conf;
            return true;
        }
        ~EventCollector() {
    
        }
};

#endif // _EVT_COLLECTOR_H_
