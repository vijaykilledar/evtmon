#ifndef __NETLINK_SOCKET_H__
#define __NETLINK_SOCKET_H__
extern "C" {
#include <libmnl/libmnl.h>
}
#include <linux/if.h>
#include <linux/if_link.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <unordered_map>

#include "net_interface.h"

class NetlinkSocket {
    private:
        struct mnl_socket *m_mnlsock;
        int m_mnl_fd;
    public:
        NetlinkSocket():m_mnlsock{NULL}, m_mnl_fd{-1} {}
        int get_sock_fd() const {return m_mnl_fd;}
        bool request_link_info();
        bool request_link_ipaddr(unsigned char rtgen_family);
        bool init();
        bool dump_network_interfaces(std::unordered_map<std::string, NetInterface *> &netintf_umap);
        bool update_link_status(std::unordered_map<std::string, NetInterface *> &netintf_umap);

};

#endif // __NETLINK_SOCKET_H__
