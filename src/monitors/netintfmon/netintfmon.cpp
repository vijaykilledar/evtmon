#include "netintfmon.h"
#include "logger.h"
#include<syslog.h>
extern "C" EventMonitor* create_monitor() {
    return new NetintfMon("netintfmon");
}

bool NetintfMon::configure(Json::Value conf) {
    return true;
}

bool NetintfMon::init()
{
    bool ret = false;
    m_mnl_sock = new (std::nothrow) NetlinkSocket();
    if(m_mnl_sock && m_mnl_sock->init()) {
        ret = m_mnl_sock->init();
        ret = m_mnl_sock->dump_network_interfaces(m_intf_map);
    }
    for (auto intf = m_intf_map.begin(); intf != m_intf_map.end(); intf++) {
        std::cout<<"intf - "<<intf->first<<"\n";
    }
    return ret;
}

bool NetintfMon::start () {
    syslog(LOG_INFO, "starting netintfmon thread");
    fd_set rfds;
    int sock_fd = this->m_mnl_sock->get_sock_fd();
    while(1) {
        FD_ZERO(&rfds);
        FD_SET(sock_fd, &rfds);
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0; 

        int retval = select(sock_fd+1, &rfds, NULL, NULL, &tv);
        if(retval < 0) {
            if (errno == EBADF || errno == EBADFD) {
            break;
        }
        continue;
        } else if(retval == 0) {
            continue;
        }
        if(FD_ISSET(sock_fd, &rfds)) {
            m_mnl_sock->update_link_status(this->m_intf_map);
            for (auto intf = m_intf_map.begin(); intf != m_intf_map.end(); intf++) {
                std::string intf_name  = intf->first;
                NetInterface *ptr = intf->second;
                std::string intf_status = ptr->get_link_status_to_str();
                syslog(LOG_INFO, "intf - %s status %s",intf_name.c_str(), intf_status.c_str());
            }
        }
    }
}
