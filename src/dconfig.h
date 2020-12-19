#ifndef _DCONFIG_H_
#define _DCONFIG_H_

#include "global.h"

#define config DaemonConfig::instance()

class DaemonConfig {
    private:
        DaemonConfig() {};
        DaemonConfig(DaemonConfig const& copy) {};
        DaemonConfig& operator=(DaemonConfig const& copy) {};
        std::string m_conf_file;
        Json::Value m_config;
    public:
        static DaemonConfig& instance(); 
        bool load_config(const char *conf_file);
        const Json::Value& get_config() {return m_config;}
};
#endif //_DCONFIG_H_
