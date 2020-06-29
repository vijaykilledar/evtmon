#ifndef _MONITOR_INTF_H_
#define _MONITOR_INTF_H_

#include<iostream>
#include <libmnl/libmnl.h>
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>


#include "evt_monitor.h"

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
        static IntfEvtMonitor& instance() {
            static IntfEvtMonitor instance;
            return instance;
        }
};

#endif // _MONITOR_INTF_H_
