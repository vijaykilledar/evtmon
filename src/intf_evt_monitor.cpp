#include "intf_evt_monitor.h"

bool IntfEvtMonitor::init() {
    bool ret = false;
    m_mnlsock = mnl_socket_open2(NETLINK_ROUTE,  SOCK_NONBLOCK);
    if(m_mnlsock == NULL) {
	    std::cout<<"Error: mnl_socket_open()\n";
        return ret;
    }
    int rv = mnl_socket_bind(m_mnlsock, RTMGRP_LINK, MNL_SOCKET_AUTOPID);
    if(rv < 0) {
        //logger::log("Error: mnl_socket_bind");
        return ret;
    }
    m_mnl_fd = mnl_socket_get_fd(m_mnlsock);
    ret = true;
    return ret;
}

bool IntfEvtMonitor::init_interface_data() {
    request_link_info();
    request_link_ipaddr(AF_INET);
    request_link_ipaddr(AF_INET6);
}

bool IntfEvtMonitor::request_link_info() {
    char buf[MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh;
    struct rtgenmsg *rt;
    unsigned int seq;

    nlh = mnl_nlmsg_put_header(buf);
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq = seq = time(NULL);

    rt = (struct rtgenmsg *)mnl_nlmsg_put_extra_header(nlh, sizeof(struct rtgenmsg));
    rt->rtgen_family = AF_PACKET;

    bool ret = true;

    if (mnl_socket_sendto(m_mnlsock, nlh, nlh->nlmsg_len) < 0) {
        ret = false;
    }
    return ret;
}
 
bool IntfEvtMonitor::request_link_ipaddr(unsigned char rtgen_family) {
    char buf[MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh;
    struct rtgenmsg *rt;
    unsigned int seq;

    nlh = mnl_nlmsg_put_header(buf);
    nlh->nlmsg_type = RTM_GETADDR;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq = seq = time(NULL);

    rt = (struct rtgenmsg *)mnl_nlmsg_put_extra_header(nlh, sizeof(struct rtgenmsg));

    rt->rtgen_family = rtgen_family;

    bool ret = true;
   
    if (mnl_socket_sendto(m_mnlsock, nlh, nlh->nlmsg_len) < 0) {
        ret = false;
    }
    
    return ret;
}

int IntfEvtMonitor::parse_link_attr(const struct nlattr *attr, void *data)
{
     const struct nlattr **tb  = (const struct nlattr **)data;
     int type = mnl_attr_get_type(attr);
     //skip unsupported attribute in user-space
     if(mnl_attr_type_valid(attr, IFLA_MAX) < 0) {
         return MNL_CB_OK;
     }
     switch(type) {
         case IFLA_IFNAME:
             if(mnl_attr_validate(attr, MNL_TYPE_STRING) < 0) {
                 return MNL_CB_ERROR;
             }
         break;
         case IFLA_ADDRESS:
             if(mnl_attr_validate(attr, MNL_TYPE_BINARY) < 0) {
                 return MNL_CB_ERROR;
             }
         break;
     }
     tb[type] = attr;
     return MNL_CB_OK;
}

int IntfEvtMonitor::parse_link_status_msg(const struct nlmsghdr *nlh, void *data)
{
     struct nlattr *tb[IFLA_MAX+1] = {};
     struct ifinfomsg *ifm = (struct ifinfomsg *)mnl_nlmsg_get_payload(nlh);
     mnl_attr_parse(nlh, sizeof(*ifm), IntfEvtMonitor::parse_link_attr, tb);
 
     std::string intf;
     if(tb[IFLA_IFNAME]) {
         intf = mnl_attr_get_str(tb[IFLA_IFNAME]);
         std::cout<<intf<<"\n";
     } else {
         return MNL_CB_OK;
     }
 
     if(ifm->ifi_flags & IFF_RUNNING) {
     } else {
     }
 
     return MNL_CB_OK;
}

void netinterface_start() {
    IntfEvtMonitor::instance().init();
    IntfEvtMonitor::instance().start();
}

bool IntfEvtMonitor::start () {
	std::cout<<__FUNCTION__<<"\n";
    fd_set rfds;
    init_interface_data();
    while(1) {
	std::cout<<"in while"<<"\n";
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
        if(FD_ISSET(m_mnl_fd, &rfds)) {
            char buf[MNL_SOCKET_BUFFER_SIZE];
            int ret;
            ret = mnl_socket_recvfrom(m_mnlsock, buf, sizeof(buf));
            while(ret > 0) {
                ret = mnl_cb_run(buf, ret, 0, 0, IntfEvtMonitor::parse_link_status_msg, NULL);
                if(ret <= 0) {
                    break;
                }
                ret = mnl_socket_recvfrom(m_mnlsock, buf, sizeof(buf));
            }
        }
    }
}
