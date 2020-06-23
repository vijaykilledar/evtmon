#ifndef NET_INTERFACE_H
#define NET_INTERFACE_H

#include <iostream>

enum class Net_Interface_Status {
    down    = 0,
    up      = 1,
    invalid = 2
};

class NetInterface {
    private:
    	std::string m_ipv4_addr;
    	std::string m_ipv6_addr;
    public:
};

#endif // NET_INTERFACE_H
