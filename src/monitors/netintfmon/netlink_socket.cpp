#include "netlink_socket.h"
#include<syslog.h>
/*TODO:Make this below method generic so that it can be used for other types of nelink
 * messages like network route change. Make config and class implementation different.
 */
bool NetlinkSocket::init() {
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

bool NetlinkSocket::request_link_info() {
    char buf[MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh;
    struct rtgenmsg *rt;
    unsigned int seq;

    nlh = mnl_nlmsg_put_header(buf);
    nlh->nlmsg_type  = RTM_GETLINK;
    nlh->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    nlh->nlmsg_seq   = seq = time(NULL);

    rt = (struct rtgenmsg *) mnl_nlmsg_put_extra_header(nlh, sizeof(struct rtgenmsg));
    rt->rtgen_family = AF_PACKET;

    bool ret = true;

    if (mnl_socket_sendto(m_mnlsock, nlh, nlh->nlmsg_len) < 0) {
        ret = false;
    }
    return ret;
}

bool NetlinkSocket::request_link_ipaddr(unsigned char rtgen_family) {

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
     std::unordered_map<std::string, NetInterface *> *m = static_cast<std::unordered_map<std::string, NetInterface *> *>(data);
     std::string intf;
     if(tb[IFLA_IFNAME]) {
         intf = mnl_attr_get_str(tb[IFLA_IFNAME]);
         if(m->find(intf) == m->end()) {
             NetInterface *intf_ptr = new NetInterface();
             (*m)[intf] = intf_ptr;
         } else {         
            (*m)[intf]->set_intf_name(intf);
         }
     } else {
         return MNL_CB_OK;
     }
     if(m->find(intf) == m->end()) {
        NetInterface *intf_ptr = new NetInterface();
        (*m)[intf] = intf_ptr;
     }
     if(ifm->ifi_flags & IFF_RUNNING) {
         (*m)[intf]->set_link_status(NetIntfStatus::up);
     } else {
         (*m)[intf]->set_link_status(NetIntfStatus::down);
     }
     return MNL_CB_OK;
}

bool NetlinkSocket::update_link_status(std::unordered_map<std::string, NetInterface *> &netintf_umap) {
    char buf[MNL_SOCKET_BUFFER_SIZE];
    int ret;
    ret = mnl_socket_recvfrom(m_mnlsock, buf, sizeof(buf));
    while(ret > 0) {
        ret = mnl_cb_run(buf, ret, 0, 0, parse_link_status_msg, static_cast<void *>(&netintf_umap));
        if(ret <= 0) {
            break;
        }
        ret = mnl_socket_recvfrom(m_mnlsock, buf, sizeof(buf));
    }
    return true;
} 

bool NetlinkSocket::dump_network_interfaces(std::unordered_map<std::string, NetInterface *> &netintf_umap) {
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
        ret = mnl_cb_run(buf, ret, seq, portid, parse_link_status_msg, static_cast<void *>(&netintf_umap));
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
