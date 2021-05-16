#include "netintfmon.h"
#include "logger.h"
#include<syslog.h>
static std::string parse_mac_addr(uint8_t *hwaddr, int payload_len)
{
    char byte_buf[48];
    std::string mac_addr = "";
    int index;
    for(index = 0; index < payload_len; index++) {
        snprintf(byte_buf, sizeof(byte_buf), "%.2x", hwaddr[index] & 0xff);
        mac_addr = mac_addr + byte_buf + ":";
    }
    if(mac_addr.size()) {
        mac_addr = mac_addr.substr(0, mac_addr.size() - 1);
    }
    return mac_addr;
}

extern "C" EventMonitor* create_monitor() {
    return new NetintfMon("netintfmon");
}

Json::Value NetintfMon::link_init_event_data(NetInterface *intf) {
    Json::Value data;
    data["interface"] = intf->get_intf_name();
    data["mac_address"] = intf->get_mac_addr();
    data["link_status"] = intf->get_link_status_to_str();
    return data;
}

Json::Value NetintfMon::link_status_event_data(NetInterface *intf) {
    Json::Value data;
    data["interface"] = intf->get_intf_name();
    data["link_status"] = intf->get_link_status_to_str();
    return data;
}

bool NetintfMon::init_netlink() {
    bool ret = false;
    m_mnlsock = mnl_socket_open2(NETLINK_ROUTE,  SOCK_NONBLOCK);
    if(m_mnlsock == NULL) {
        return ret;
    }
    int rv = mnl_socket_bind(m_mnlsock, RTMGRP_LINK, MNL_SOCKET_AUTOPID);
    if(rv < 0) {
        return ret;
    }
    m_mnl_fd = mnl_socket_get_fd(m_mnlsock);
    ret = true;
    return ret;
}

bool NetintfMon::dump_network_interfaces() {
    struct mnl_socket *nl;
    char buf[MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh;
    struct rtgenmsg *rt;
    int ret;
    unsigned int seq, portid;
    bool ret_val = false;

    nlh = (struct nlmsghdr *)mnl_nlmsg_put_header((void *)buf);
    nlh->nlmsg_type = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq = seq = time(NULL);
    size_t sz = sizeof(struct rtgenmsg);
    rt = (struct rtgenmsg *)mnl_nlmsg_put_extra_header(nlh, sz);
    rt->rtgen_family = AF_PACKET;

    nl = mnl_socket_open(NETLINK_ROUTE);
    if (nl == NULL) {
        return ret_val;    
    }
    if (mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID) < 0) {
        return ret_val;
    }

    portid = mnl_socket_get_portid(nl);

    if (mnl_socket_sendto(nl, nlh, nlh->nlmsg_len) < 0) {
        return ret_val;
    }
    ret = mnl_socket_recvfrom(nl, buf, sizeof(buf));
    while (ret > 0) {
        ret = mnl_cb_run(buf, ret, seq, portid, parse_link_status_msg, static_cast<void *>(this));
        if (ret <= MNL_CB_STOP) {
                break;
        }
        ret = mnl_socket_recvfrom(nl, buf, sizeof(buf));
    }
    if (ret == -1) {
        return ret_val;
    }
    mnl_socket_close(nl);
    ret_val = true;
    return ret_val;
}

int parse_link_attr(const struct nlattr *attr, void *data)
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

int parse_link_status_msg(const struct nlmsghdr *nlh, void *data)
{
     struct nlattr *tb[IFLA_MAX+1] = {};
     struct ifinfomsg *ifm = (struct ifinfomsg *)mnl_nlmsg_get_payload(nlh);
     mnl_attr_parse(nlh, sizeof(*ifm), parse_link_attr, tb);

     NetintfMon *netintfmon_obj = static_cast<NetintfMon *>(data);

     std::string intf;
     if (tb[IFLA_IFNAME]) {
         intf = mnl_attr_get_str(tb[IFLA_IFNAME]);
         if (!netintfmon_obj->found(intf)) {
             netintfmon_obj->new_interface(intf);
             netintfmon_obj->queue_event(intf, EventType::LINK_INIT);
         } else {         
            netintfmon_obj->get_interface(intf)->set_intf_name(intf);
         }
         if (tb[IFLA_ADDRESS]) {
                uint8_t *hwaddr = (uint8_t *)mnl_attr_get_payload(tb[IFLA_ADDRESS]);
                int payload_len = mnl_attr_get_payload_len(tb[IFLA_ADDRESS]);
                std::string mac_addr;
                mac_addr = parse_mac_addr(hwaddr, payload_len);
                netintfmon_obj->get_interface(intf)->set_mac_addr(mac_addr);
        }
     } else {
         return MNL_CB_OK;
     }
     if (!netintfmon_obj->found(intf)){
         netintfmon_obj->new_interface(intf);
         netintfmon_obj->queue_event(intf, EventType::LINK_INIT);
     }
     NetIntfStatus old_status = netintfmon_obj->get_interface(intf)->get_link_status();
     if (ifm->ifi_flags & IFF_RUNNING) {
         netintfmon_obj->get_interface(intf)->set_link_status(NetIntfStatus::up);
     } else {
         netintfmon_obj->get_interface(intf)->set_link_status(NetIntfStatus::down);
     }
     if (old_status != netintfmon_obj->get_interface(intf)->get_link_status()) {
        netintfmon_obj->queue_event(intf, EventType::LINK_STATUS_EVENT);
     }
     return MNL_CB_OK;
}

bool NetintfMon::update_link_status() {
    char buf[MNL_SOCKET_BUFFER_SIZE];
    int ret;
    ret = mnl_socket_recvfrom(m_mnlsock, buf, sizeof(buf));
    while(ret > 0) {
        ret = mnl_cb_run(buf, ret, 0, 0, parse_link_status_msg, static_cast<void *>(this));
        if(ret <= 0) {
            break;
        }
        ret = mnl_socket_recvfrom(m_mnlsock, buf, sizeof(buf));
    }
    return true;
} 


bool NetintfMon::init()
{
    bool ret = false;
    if(init_netlink()) {
        ret = dump_network_interfaces();
    }
    for (auto intf = m_intf_map.begin(); intf != m_intf_map.end(); intf++) {
        std::cout<<"intf - "<<intf->first<<"\n";
    }
    return ret;
}

bool NetintfMon::start () {
    syslog(LOG_INFO, "starting netintfmon thread");
    fd_set rfds;
    int sock_fd = m_mnl_fd;
    while(1) {
        for (auto intf = m_intf_map.begin(); intf != m_intf_map.end(); intf++) {
            std::string intf_name  = intf->first;
            NetInterface *ptr = intf->second;
            while (!m_intf_event_map[intf_name].empty()) {
                EventType e_type = m_intf_event_map[intf_name].front();
                Json::Value e_data;
                if(e_type == EventType::LINK_INIT) {
                    e_data = link_init_event_data(ptr);
                } else if (e_type == EventType::LINK_STATUS_EVENT) {
                    e_data = link_status_event_data(ptr);
                }
                publish_event(m_name, e_type, e_data);
                m_intf_event_map[intf_name].pop();
            }
        }

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
            update_link_status();
        }
    }
    return true;
}
