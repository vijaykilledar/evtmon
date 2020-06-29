#include "intf_evt_monitor.h"

bool IntfEvtMonitor::init() {
    bool ret = false;
    *m_mnlsock = mnl_socket_open2(NETLINK_ROUTE,  SOCK_NONBLOCK);
    if(*m_mnlsock == NULL) {
        //logger::log("Error: mnl_socket_open()");
        return ret;
    }
    int rv = mnl_socket_bind(*m_mnlsock, RTMGRP_LINK, MNL_SOCKET_AUTOPID);
    if(rv < 0) {
        //logger::log("Error: mnl_socket_bind");
        return ret;
    }
    m_mnl_fd = mnl_socket_get_fd(*m_mnlsock);
    ret = true;
    return ret;
}

void IntfEvtMonitor::operator() () {
    fd_set rfds;
    while(1) {
        FD_ZERO(&rfds);
        FD_SET(m_mnl_fd, &rfds);
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0; 

        int retval = select(m_mnl_fd+1, &rfds, NULL, NULL, &tv);
        if(retval < 0) {
            if (errno == EBADF || errno == EBADFD) {
            break;
        }
        continue;
        } else if(retval == 0) {
            continue;
        }
//        check_msg_on_mnl(g_nlsock, g_sock_fds[MNL_SOCK_FDINDEX], &rfds);
    }
}
