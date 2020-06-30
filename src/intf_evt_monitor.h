#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

#include<iostream>
#include<map>
#include <arpa/inet.h>
#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>


#include "evt_monitor.h"
#include "net_interface.h"

class IntfEvtMonitor: public EventMonitor {
    private:
        struct mnl_socket **m_mnlsock;
        int m_mnl_fd;
        IntfEvtMonitor(): m_mnlsock{NULL}, m_mnl_fd {-1} {}
        IntfEvtMonitor(IntfEvtMonitor const& copy);
        IntfEvtMonitor& operator=(IntfEvtMonitor const& copy);
    public:
        virtual void operator() ();
        virtual void handle_event(){};
        virtual bool init();
        virtual bool start(){};
        static int parse_link_status_msg(const struct nlmsghdr *nlh, void *data);
        static int parse_link_attr(const struct nlattr *attr, void *data);
        static IntfEvtMonitor& instance() {
            static IntfEvtMonitor instance;
            return instance;
        }
};

#endif // _MONITOR_INTF_H_
