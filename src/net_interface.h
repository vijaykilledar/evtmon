#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include <iostream>

enum class NetIntfStatus {
    down    = 0,
    up      = 1,
    invalid = 2
};

class NetInterface {
    private:
        std::string m_ipv4_addr;
        std::string m_ipv6_addr;
        NetIntfStatus m_link_status;
        NetIntfStatus m_link_admin_status;
    public:
        const std::string& get_ipv4_addr();
        const std::string& get_ipv6_addr();
        const NetIntfStatus& get_link_status();
        const NetIntfStatus& get_link_admin_status();
};

#endif // NET_INTERFACE_H
