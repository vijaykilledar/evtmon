#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

#include <iostream>
#include <map>
#include <arpa/inet.h>
#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>


#include "evt_monitor.h"
#include "net_interface.h"

void netinterface_start(Json::Value conf);

class IntfEvtMonitor: public EventMonitor {
    private:
        struct mnl_socket *m_mnlsock;
        int m_mnl_fd;
        IntfEvtMonitor(): m_mnlsock{NULL}, m_mnl_fd {-1} {}
        IntfEvtMonitor(IntfEvtMonitor const& copy);
        IntfEvtMonitor& operator=(IntfEvtMonitor const& copy);
    public:
        std::map<std::string, NetInterface *> m_intf;
        void new_interface(std::string intf) {
            m_intf[intf] = new NetInterface(intf);
        }
        virtual void handle_event(){};
        virtual bool init();
        virtual bool start();
        bool configure(Json::Value conf) {}
        static int parse_link_status_msg(const struct nlmsghdr *nlh, void *data);
        static int parse_link_attr(const struct nlattr *attr, void *data);
        bool request_link_info();
        bool request_link_ipaddr(unsigned char rtgen_family);
        bool init_interface_data();
        static IntfEvtMonitor& instance() {
            static IntfEvtMonitor instance;
            return instance;
        }
};

#endif // _MONITOR_INTF_H_
