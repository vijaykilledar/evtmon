#ifndef _EVENT_H_
#define _EVENT_H_

#include "global.h"
#include <ctime>

enum class EventType {
    INVALID,
    //netintfmon
    LINK_INIT,
    LINK_STATUS_EVENT,
    IP_CHANGE_EVENT
};

std::string event_type_tostring(EventType et);

class Event {
    private:
        EventType m_type;
        std::string m_name;
        Json::Value m_data;
        std::time_t m_timestamp;
        std::string m_source;
    public:
        Event(std::string src, EventType e_type, Json::Value data) {
            m_type = e_type;
            m_name = event_type_tostring(e_type);
            m_data = data;
            m_source = src;
            std::time_t t = std::time(nullptr);
            m_timestamp = std::mktime(std::gmtime(&t));
        }
        std::string get_name() const {
            return m_name;
        }
        EventType get_type() const {
            return m_type;
        }
        void set_source(const std::string &src) {
            m_source = src;
        }
        std::string serialize() {
            Json::Value event_json;
            event_json["timestamp"] = static_cast<unsigned int> (m_timestamp);
            event_json["data"] = m_data;
            event_json["type"] = m_name;
            event_json["source"] = m_source;
            Json::FastWriter fastWriter;
            std::string ret  = fastWriter.write(event_json);
            return ret;
        }
};

#endif // _EVENT_H_
