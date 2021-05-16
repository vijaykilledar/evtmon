#include "event.h"

std::string event_type_tostring(EventType et) {
    std::string ret;
    switch (et) {
        //netintfmon
        case EventType::LINK_INIT:          ret = "LINK_INIT";          break;
        case EventType::LINK_STATUS_EVENT:  ret = "LINK_STATUS_EVENT";  break;
        case EventType::IP_CHANGE_EVENT:    ret = "IP_CHANGE_EVENT";    break;
        //default
        default:                            ret = "INVALID";            break; 
    }
    return ret;
}
