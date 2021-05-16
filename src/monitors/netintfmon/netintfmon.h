#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

extern "C" {
#include <libmnl/libmnl.h>
}
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>

#include <iostream>
#include <unordered_map>
#include <queue>

#include "evt_monitor.h"
#include "net_interface.h"
#include "event.h"

int parse_link_status_msg(const struct nlmsghdr *nlh, void *data);

class NetintfMon: public EventMonitor {
    private:
        std::string m_name;
        std::unordered_map<std::string, NetInterface *> m_intf_map;
        std::unordered_map<std::string, std::queue<EventType>> m_intf_event_map;
        struct mnl_socket *m_mnlsock;
        int m_mnl_fd;
    public:
        virtual void handle_event(){};
        virtual bool init();
        virtual bool start();
        bool init_netlink();
        bool dump_network_interfaces();
        bool update_link_status();
        Json::Value link_init_event_data(NetInterface *intf);
        Json::Value link_status_event_data(NetInterface *intf);
        void queue_event(std::string intf_name, EventType e_type) {
            m_intf_event_map[intf_name].push(e_type);
        }
        bool found(std::string intf_name) {
            if(m_intf_map.find(intf_name) == m_intf_map.end()) {
                return false;
            } else {
                return true;
            }
        }
        NetInterface *get_interface(std::string intf_name) {
            return m_intf_map[intf_name];
        }
        NetintfMon(const std::string &name) {
            m_name = name;
            m_mnlsock = NULL;
            m_mnl_fd = -1;
        }
        void new_interface(std::string intf) {
            m_intf_map[intf] = new NetInterface(intf);
        }
        ~NetintfMon() {
            for(auto intf=m_intf_map.begin(); intf != m_intf_map.end(); intf++) {
                delete intf->second;
            } 
        }
};


#endif // _MONITOR_INTF_H_
