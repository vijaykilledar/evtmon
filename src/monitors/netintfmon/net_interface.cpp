#include "net_interface.h"

const std::string& NetInterface::get_mac_addr() {
    return m_mac_addr;
}
const std::string& NetInterface::get_ipv4_addr() {
    return m_ipv4_addr;
}

const std::string& NetInterface::get_intf_name() {
    return m_intf_name;
}

const std::string& NetInterface::get_ipv6_addr() {
    return m_ipv6_addr;
}

const NetIntfStatus& NetInterface::get_link_status() {
    return m_link_status;
}

std::string NetInterface::get_link_status_to_str() {
    std::string ret;
    if(m_link_status == NetIntfStatus::down)
            ret = "DOWN";
    else if(m_link_status == NetIntfStatus::up)
            ret = "UP";
    else
           ret = "INVALID";
    
    return ret;
}

const NetIntfStatus& NetInterface::get_link_admin_status() {
    return m_link_admin_status;
}

void NetInterface::set_intf_name(const std::string &val) {
    m_intf_name = val;
}

void NetInterface::set_ipv6_addr(const std::string &val) {
    m_ipv6_addr = val;
}

void NetInterface::set_ipv4_addr(const std::string &val) {
    m_ipv4_addr = val;
}

void NetInterface::set_link_status(NetIntfStatus val) {
    m_link_status = val;
}

void NetInterface::set_mac_addr(const std::string &val) {
    m_mac_addr = val;
}
