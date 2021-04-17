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
        std::string m_intf_name;
        std::string m_ipv4_addr;
        std::string m_ipv6_addr;
        NetIntfStatus m_link_status;
        NetIntfStatus m_link_admin_status;
    public:
        NetInterface() {}
        NetInterface(std::string intf_name) {
            m_intf_name = intf_name;
        }
        const std::string& get_ipv4_addr();
        const std::string& get_intf_name();
        const std::string& get_ipv6_addr();
        const NetIntfStatus& get_link_status();
        std::string get_link_status_to_str();
        const NetIntfStatus& get_link_admin_status();
        void set_intf_name(const std::string &val);
        void set_ipv6_addr(const std::string &val);
        void set_ipv4_addr(const std::string &val);
        void set_link_status(NetIntfStatus val);
};

#endif // NET_INTERFACE_H
