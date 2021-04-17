#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

#include <iostream>
#include <arpa/inet.h>
#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>
#include <unordered_map>

#include "evt_monitor.h"
#include "net_interface.h"
#include "netlink_socket.h"

class NetintfMon: public EventMonitor {
    private:
        std::string m_name;
        NetlinkSocket *m_mnl_sock;
    public:
        NetintfMon(const std::string &name) {
            m_name = name;
        }
        std::unordered_map<std::string, NetInterface *> m_intf_map;
        void new_interface(std::string intf) {
            m_intf_map[intf] = new NetInterface(intf);
        }
        virtual void handle_event(){};
        virtual bool init();
        virtual bool start();
        bool configure(Json::Value conf);
};


#endif // _MONITOR_INTF_H_
